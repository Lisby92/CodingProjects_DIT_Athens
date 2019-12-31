import pandas as pd
data_frame = pd.read_csv('salam2.csv', names=['Date_Time', 'Tag', 'Ask', 'Bid'],index_col=1, parse_dates=True)
data_frame.head()
data_ask =  data_frame['Ask'].resample('15Min').ohlc()

data_bid =  data_frame['Bid'].resample('15Min').ohlc()

data_ask.head()

data_bid.head()

data_ask_bid=pd.concat([data_ask, data_bid], axis=1, keys=['Ask', 'Bid'])