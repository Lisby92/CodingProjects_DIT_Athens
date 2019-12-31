import csv
import datetime
import time
import requests
import random
import threading
import os
import numpy as np

SAVE_DIRECTORY = None  # directory where data should be saved, leave None to auto-fill
MARKETS = ['BTC_XMR']  # leave empty to pull data for all BTC markets, otherwise provide e.g. ['BTC_XMR', 'BTC_LTC'...]
START = '20/05/2014'  # d/m/y get data from
END = '02/06/2014'  # to
PERIOD = 1  # time period in seconds to take average bid/ask over ## is invalid method regardless the period tho it works

BASE_URL = 'https://poloniex.com/public?command=returnTradeHistory&currencyPair={}&start={}&end={}'

class Collector:
    def __init__(self):
        self.proxy_pool = []
        if not MARKETS:  # get list of all BTC markets
            raw_markets = requests.get('https://poloniex.com/public?command=return24hVolume', timeout=20).json()
            self.markets = [m for m in raw_markets if m.startswith('BTC')]
        else:
            self.markets = MARKETS
        self.num_markets = len(self.markets)
        self.download_logs = {m: 0 for m in self.markets}
        self.parse_logs = {m: False for m in self.markets}
        if not SAVE_DIRECTORY:
            os.mkdir('{} - {}'.format(START, END).replace('/', '.'))
            self.save_directory = '{} - {}'.format(START, END).replace('/', '.')
        start_stamp = int(datetime.datetime.strptime(START, "%d/%m/%Y").timestamp())  # to unix timestamp pairs
        end_stamp = int(datetime.datetime.strptime(END, "%d/%m/%Y").timestamp())
        stamps = range(start_stamp, end_stamp, 360)
        self.stamp_pairs = list(zip(stamps, stamps[1:]))
        self.num_periods = len(self.stamp_pairs)

    def get_trade_history(self, market, start, end, proxy=None):
        """
        Queries Poloniex for trade history of market within given time period.
        Cycles through free proxies from Gimmieproxy in the event of throttling.
        """
        request_url = BASE_URL.format(market, start, end)

        try:
            return requests.get(request_url, timeout=30, proxies=proxy).json()
        except:
            print('Throttled whilst collecting {}. Continuing with proxy.'.format(market))
            # failed with proxy, so delete it (if it exists - could be used by multiple threads)
            if proxy in self.proxy_pool:
                self.proxy_pool.remove(proxy)
            # try a proxy from the proxy pool if one exists
            if len(self.proxy_pool) > 0:
                random_proxy = self.proxy_pool[random.randint(0, len(self.proxy_pool) - 1)]
                return self.get_trade_history(market, start, end, proxy=random_proxy)
            else:
                try:
                    raw_proxy_response = requests.get('https://gimmeproxy.com/api/getProxy?post=true&supportsHttps=true'
                                                      '&maxCheckPeriod=1200&protocol=http&get=true', timeout=10).json()
                    new_proxy = {raw_proxy_response['type']: raw_proxy_response['curl']}
                    self.proxy_pool.append(new_proxy)
                    print('New proxy acquired :: {}'.format(raw_proxy_response['curl']))
                    return self.get_trade_history(market, start, end, proxy=new_proxy)
                except:
                    print('Couldn\'t get proxy for {}. Trying again without proxy.'.format(market))
                    return self.get_trade_history(market, start, end)


    """
    require same length for all dat files, purging entries that are invalid
    
    """

    def parse(self, market):
        """calculate average bid/ask for given period from trade history and write it to file."""
        reader = csv.reader(open('{}/RAW_{}.csv'.format(self.save_directory, market)))
        writer = csv.writer(open('{}/{}.csv'.format(self.save_directory, market), 'a'))
        last_bid = None
        last_ask = None
        last_tx = None
        last_row_to_write = None
        row_to_write = []
        window = []
        while True:
            try:
                row = next(reader)
                tx = int(datetime.datetime.strptime(row[0], '%Y-%m-%d %H:%M:%S').timestamp())  # to unix timestamp
                typ = row[1]
                rate = row[2]
                amount = row[3]
                '''
                    if new tx > last tx + PERIOD, then we need to fill the book with a sufficient number of the
                    previous row.
                '''
                if last_tx and last_row_to_write:
                    for _ in range(int(round((tx - last_tx) / PERIOD))):
                        if all(item is not None for item in last_row_to_write):
                            writer.writerow(last_row_to_write)
                if not window or tx < window[0][0] + PERIOD:  # variable period over which to take avg bid/ask
                    window.append((tx, typ, rate, amount))
                else:
                    amts = [float(p[3]) for p in window if p[1] == 'sell']
                    if sum([1 for p in window if p[1] == 'sell']) == 0 or sum(amts) == 0:  # avoid /0
                        bid = None
                    else:
                        bid = np.average([float(p[2]) for p in window if p[1] == 'sell'], weights=amts)
                    amts = [float(p[3]) for p in window if p[1] == 'buy']
                    if sum([1 for p in window if p[1] == 'buy']) == 0 or sum(amts) == 0:
                        ask = None
                    else:
                        ask = np.average([float(p[2]) for p in window if p[1] == 'buy'], weights=amts)

                    if bid is None:
                        row_to_write.append(last_bid)
                    else:
                        row_to_write.append(bid)
                        last_bid = bid

                    if ask is None:
                        row_to_write.append(last_ask)
                    else:
                        row_to_write.append(ask)
                        last_ask = ask

                    if len(row_to_write) == 2:
                        row_to_write.append(sum([float(v[3]) for v in window if v[1] == 'sell']))  # sell volume
                        row_to_write.append(sum([float(v[3]) for v in window if v[1] == 'buy']))  # buy volume
                        if all(item is not None for item in row_to_write):
                            writer.writerow(row_to_write)
                    last_row_to_write = row_to_write
                    row_to_write = []
                    window = [(tx, typ, rate, amount)]
                    last_tx = tx
            except StopIteration:
                break

    def worker(self, market):
        """ Thread collection of data from each individual market pair """
        with open('{}/RAW_{}.csv'.format(self.save_directory, market), 'a') as f:
            writer = csv.writer(f)
            # start, stop represents an hour window to retrieve data for
            for start, stop in self.stamp_pairs:
                response = self.get_trade_history(market, start, stop)
                for row in reversed(response):
                    writer.writerow([row['date'], row['type'], row['rate'], row['amount']])
                self.download_logs[market] += 1  # for reporting download progress
        self.parse(market)
        self.parse_logs[market] = True
        os.remove('{}/RAW_{}.csv'.format(self.save_directory, market))

    def report(self):
        """ reports progress for each download / parse every 30 sec """
        while True:
            for m in self.markets:
                if self.parse_logs[m]:
                    print('{} - Finished downloading and parsing.'.format(m))
                else:
                    pcc = int(self.download_logs[m] * 100 / self.num_periods)
                    print('{} [{}%]'.format(m, pcc))
            print()
            if all(v is True for v in self.parse_logs.values()):
                break
            time.sleep(30)

    def run(self):
        time_begin = time.time()
        reporter = threading.Thread(target=self.report)
        reporter.start()
        workers = []
        for market in self.markets:  # aaallllll the threads - you might have to increase your OS open file limit
            t = threading.Thread(target=self.worker, args=(market,))
            workers.append(t)
            t.start()
        for thread in workers:
            thread.join()
        reporter.join()
        print('Collected & parsed data on {} markets in {} minutes'.format(self.num_markets,
                                                                           int((time.time() - time_begin) / 60)))

if __name__ == '__main__':
    Collector().run()
