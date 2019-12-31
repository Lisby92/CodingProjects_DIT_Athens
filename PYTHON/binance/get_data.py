from binance.client import Client
client = Client('APIkey', 'APIsecret')#optional third parametre = dictionary parametres

## get market depth
##depth = client.get_order_book(symbol='BNBBTC')

## place a test market buy order, to place an actual order use the create_order function
##order = client.create_test_order(
    #symbol='BNBBTC',
#    #side=Client.SIDE_BUY,
#    #type=Client.ORDER_TYPE_MARKET,
#    #quantity=100)
#
## get all symbol prices
##prices = client.get_all_tickers()

## withdraw 100 ETH
## check docs for assumptions around withdrawals
#from binance.exceptions import BinanceAPIException, BinanceWithdrawException
##try:
#    #result = client.withdraw(
#        #asset='ETH',
#        #address='<eth_address>',
#        #amount=100)
#except BinanceAPIException as e:
#    print(e)
#except BinanceWithdrawException as e:
#    print(e)
#else:
#    print("Success")
#
## fetch list of withdrawals
#withdraws = client.get_withdraw_history()

## fetch list of ETH withdrawals
#eth_withdraws = client.get_withdraw_history(asset='ETH')

## get a deposit address for BTC
#address = client.get_deposit_address(asset='BTC')

## start aggregated trade websocket for BNBBTC
#def process_message(msg):
   # print("message type: {}".format(msg['e']))
#    print(msg)
#    # do something
#
#from binance.websockets import BinanceSocketManager
#bm = BinanceSocketManager(client)
#bm.start_aggtrade_socket('BNBBTC', process_message)
#bm.start()
#
## get historical kline data from any date range
#
# fetch 1 minute klines for the last day up until now
#klines = client.get_historical_klines("BNBBTC", Client.KLINE_INTERVAL_1MINUTE, "1 day ago UTC")

## fetch 30 minute klines for the last month of 2017
#klines = client.get_historical_klines("ETHBTC", Client.KLINE_INTERVAL_30MINUTE, "1 Dec, 2017", "1 Jan, 2018")

# gia backtesting perneis ola ta data kai ta epeksergazesai afou ta exeis etoima sto text
# gia live trading eite perneis sunexeia data kai panta zitas ta argotera kalontas auto to programa i mallon
# ftiaxneis ena programma to opoio to bot tha xeirizete live ta data kai tha ta pernei apto binance kai me vasi auta kai tis stratigikes tha leitourgei

klines = client.get_historical_klines("XMRBTC", Client.KLINE_INTERVAL_1MONTH, "1 Nov, 2017")
with open('1M.txt', 'w') as f:
    f.write("Open Time      , Open      , High         , Low        , Close         , Volume      , Close Time   , Quote Asset Volume  , Trades, Taker buy base asset volume, Taker buy quote asset volume\n")
    for line in klines:
        f.write("%s\n" % line)  
## fetch weekly klines since it listed#
klines = client.get_historical_klines("XMRBTC", Client.KLINE_INTERVAL_1WEEK, "1 Nov, 2017")
with open('1W.txt', 'w') as f:
    f.write("Open Time      , Open      , High         , Low        , Close         , Volume      , Close Time   , Quote Asset Volume  , Trades, Taker buy base asset volume, Taker buy quote asset volume\n")
    for line in klines:
        f.write("%s\n" % line)
# etsi den ta grafei sosta sta txt prepei na ta perneis apto klines [0], [1] ktlpa kai na ta metatrepeis ola ta string sto antistoixo double i int