import csv
import math
import timeit

######################################
############ DATA_PARSER #############
######################################
start_parser = timeit.default_timer()
total_XMR_BTC_DB = []       	  # all history 
total_exchange_volume_left = 0.0  # total trading volume measured in left pair
total_exchange_volume_right = 0.0 # total trading volume measured in left pair
total_transactions = 0      	  # total transactions
with open('RAW_BTC_XMR.csv') as csvfile:
	readCSV = csv.reader(csvfile)
	for row in readCSV:
		if row != []:
			total_XMR_BTC_DB.append(row) # dont want to add empty lines
csvfile.close()
total_transactions = len(total_XMR_BTC_DB)# number of transactions
stop_parser = timeit.default_timer()
print('Parsing completed in :',(stop_parser - start_parser),'seconds!')
print(total_transactions,'total transactions processed successfully!')
######################################
######## CANDLE_MAKER ################
######################################
######################################
#initialise the candlestick lists and variables to be used
start_candlemaker = timeit.default_timer()
upper_lmt = math.inf
tag = 'sentiment'
# 1 minute
M1_Candlesticks = []
T_min1_vol = 0
B_min1_vol = 0
S_min1_vol = 0
min1_open = 0
min1_close = 0
min1_high = -1
min1_low = upper_lmt
# 5 minutes
M5_Candlesticks = []
min5_counter = 0
T_min5_vol = 0
B_min5_vol = 0
S_min5_vol = 0
min5_open = 0
min5_close = 0
min5_high = -1
min5_low = upper_lmt
# 10 minutes
M10_Candlesticks = []
min10_counter = 0
T_min10_vol = 0
B_min10_vol = 0
S_min10_vol = 0
min10_open = 0
min10_close = 0
min10_high = -1
min10_low = upper_lmt
# 15 minutes
M15_Candlesticks = []
min15_counter = 0
T_min15_vol = 0
B_min15_vol = 0
S_min15_vol = 0
min15_open = 0
min15_close = 0
min15_high = -1
min15_low = upper_lmt
# 30 minutes
M30_Candlesticks = []
min30_counter = 0
T_min30_vol = 0
B_min30_vol = 0
S_min30_vol = 0
min30_open = 0
min30_close = 0
min30_high = -1
min30_low = upper_lmt
# 1 hour
H1_Candlesticks = []
T_hour1_vol = 0
B_hour1_vol = 0
S_hour1_vol = 0
hour1_open = 0
hour1_close = 0
hour1_high = -1
hour1_low = upper_lmt
# 4 hours
H4_Candlesticks = []
hour4_counter = 0
T_hour4_vol = 0
B_hour4_vol = 0
S_hour4_vol = 0
hour4_open = 0
hour4_close = 0
hour4_high = -1
hour4_low = upper_lmt
# 1 day
D1_Candlesticks = []
T_day1_vol = 0
B_day1_vol = 0
S_day1_vol = 0
day1_open = 0
day1_close = 0
day1_high = -1
day1_low = upper_lmt
# 4 days
D4_Candlesticks = []
day4_open_day = 0
day4_open_month = 0
day4_counter = 0
T_day4_vol = 0
B_day4_vol = 0
S_day4_vol = 0
day4_open = 0
day4_close = 0
day4_high = -1
day4_low = upper_lmt
# 1 week
W1_Candlesticks = []
week1_counter = 0
week1_open_day = 0
week1_open_month = 0
T_week1_vol = 0
B_week1_vol = 0
S_week1_vol = 0
week1_open = 0
week1_close = 0
week1_high = -1
week1_low = upper_lmt
# 2 weeks
W2_Candlesticks = []
week2_counter = 0
week2_open_day = 0
week2_open_month = 0
T_week2_vol = 0
B_week2_vol = 0
S_week2_vol = 0
week2_open = 0
week2_close = 0
week2_high = -1
week2_low = upper_lmt
# 1 month
MN_Candlesticks = []
T_month_vol = 0
B_month_vol = 0
S_month_vol = 0
month_open = 0
month_close = 0
month_high = -1
month_low = upper_lmt
# non divisable timeframes to be initialised here
get_date = total_XMR_BTC_DB[0][0]
cur_month = int(get_date[5:7])
cur_day = int(get_date[8:10])
day4_open_month = cur_month
day4_open_day = cur_day
week1_open_month = cur_month
week1_open_day = cur_day
week2_open_month = cur_month
week2_open_day = cur_day
prev_min = 60
prev_date = 0
# processing transactions into candles starts here
for i in range(0, total_transactions - 1):
	total_exchange_volume_left += float(total_XMR_BTC_DB[i][3])
	total_exchange_volume_right += (float(total_XMR_BTC_DB[i][3]) * float(total_XMR_BTC_DB[i][2]))
	# process i transaction
	get_date = total_XMR_BTC_DB[i][0]
	cur_min = int(get_date[14:16])
	cur_hour = int(get_date[11:13])
	cur_day = int(get_date[8:10])
	cur_month = int(get_date[5:7])
	cur_year = int(get_date[0:4])
	cur_order = total_XMR_BTC_DB[i][1]
	cur_rate = float(total_XMR_BTC_DB[i][2])
	cur_volume = float(total_XMR_BTC_DB[i][3])
	#process  i + 1 transaction
	get_date = total_XMR_BTC_DB[i + 1][0]
	next_min = int(get_date[14:16])
	next_hour = int(get_date[11:13])
	next_day = int(get_date[8:10])
	next_month = int(get_date[5:7])
	next_year = int(get_date[0:4])
	next_order = total_XMR_BTC_DB[i + 1][1]
	next_rate = float(total_XMR_BTC_DB[i + 1][2])
	next_volume = float(total_XMR_BTC_DB[i + 1][3])
	#case 1 transactions at same month
	if (cur_month == next_month) and (cur_year == next_year):
		if month_open == 0:
			month_open = cur_rate
		if cur_rate > month_high:
			month_high = cur_rate
		if cur_rate < month_low:
			month_low = cur_rate    
		if cur_order == 'buy':
			B_month_vol += cur_volume
		elif cur_order == 'sell':
			S_month_vol += cur_volume
	#case 2 next transaction is at new month		
	else:
		month_close = cur_rate
		#calculate total month volume and sentiment
		if month_close > month_open:
			tag = 'buy'
			T_month_vol = B_month_vol + S_month_vol
		elif month_open > month_close:
			tag = 'sell'
			T_month_vol = S_month_vol + B_month_vol
		T_month_vol += cur_volume	
		#create the month's candlestick and add it
		candle = [cur_year, cur_month, T_month_vol, tag, month_open, month_close, month_low, month_high]
		MN_Candlesticks.append(candle)
		#print(candle)
		#initialise for next month
		month_open = 0
		month_close = 0
		month_high = -1
		month_low = upper_lmt
		T_month_vol = 0
		B_month_vol = 0
		S_month_vol = 0
		#case 1 transactions at same day	
	if (cur_day == next_day) and ((cur_month == next_month) and (cur_year == next_year)):
		if day1_open == 0:
			day1_open = cur_rate
		if cur_rate > day1_high:
			day1_high = cur_rate
		if cur_rate < day1_low:
			day1_low = cur_rate
		if cur_order == 'buy':
			B_day1_vol += cur_volume
		elif cur_order == 'sell':
			S_day1_vol += cur_volume
	#case 2 next transaction is at new day		
	else:
		day1_close = cur_rate
		#calculate total day1 volume and sentiment
		if day1_close > day1_open:
			tag = 'buy'
			T_day1_vol = B_day1_vol + S_day1_vol
		elif day1_open > day1_close:
			tag = 'sell'
			T_day1_vol = S_day1_vol + B_day1_vol
		T_day1_vol += cur_volume	
		#create the 1day's candlestick and add it
		candle = [cur_month, cur_day, T_day1_vol, tag, day1_open, day1_close, day1_low, day1_high]
		D1_Candlesticks.append(candle)
		#print(candle)
		#initialise for next day
		day1_open = 0
		day1_close = 0
		day1_high = -1
		day1_low = upper_lmt
		T_day1_vol = 0
		B_day1_vol = 0
		S_day1_vol = 0
		day4_counter += 1
		week1_counter += 1
		week2_counter += 1
	#case 1 transactions at same hour	
	if (cur_hour == next_hour) and ((cur_month == next_month) and (cur_year == next_year) and (cur_day == next_day)):
		if hour1_open == 0:
			hour1_open = cur_rate	
		if cur_rate > hour1_high:
			hour1_high = cur_rate
		if cur_rate < hour1_low:
			hour1_low = cur_rate
		if hour4_open == 0:
			hour4_open = cur_rate
		if cur_rate > hour4_high:
			hour4_high = cur_rate
		if cur_rate < hour4_low:
			hour4_low = cur_rate	   
		if cur_order == 'buy':
			B_hour1_vol += cur_volume
			B_hour4_vol += cur_volume
		elif cur_order == 'sell':
			S_hour1_vol += cur_volume
			S_hour4_vol += cur_volume
	#case 2 next transaction is at new hour		
	else:
		hour1_close = cur_rate
		#calculate total hour1 volume and sentiment
		if hour1_close > hour1_open:
			tag = 'buy'
			T_hour1_vol = B_hour1_vol + S_hour1_vol
		elif hour1_open > hour1_close:
			tag = 'sell'
			T_hour1_vol = S_hour1_vol + B_hour1_vol
		T_hour1_vol += cur_volume	
		#create the 1hour's candlestick and add it
		candle = [cur_day, cur_hour, T_hour1_vol, tag, hour1_open, hour1_close, hour1_low, hour1_high]
		H1_Candlesticks.append(candle)
		#print(candle)
		#initialise for next hour
		hour1_open = 0
		hour1_close = 0
		hour1_high = -1
		hour1_low = upper_lmt
		T_hour1_vol = 0
		B_hour1_vol = 0
		S_hour1_vol = 0
		hour4_counter += 1
		if hour4_counter == 4:
			hour4_close = cur_rate
			#calculate total hour4 volume and sentiment
			if hour4_close > hour4_open:
				tag = 'buy'
				T_hour4_vol = B_hour4_vol + S_hour4_vol
			elif hour4_open > hour4_close:
				tag = 'sell'
				T_hour4_vol = S_hour4_vol + B_hour4_vol
			T_hour4_vol += cur_volume	
			#create the 4hour's candlestick and add it
			candle = [cur_day, cur_hour - 3, T_hour4_vol, tag, hour4_open, hour4_close, hour4_low, hour4_high]
			H4_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 4 hours
			hour4_open = 0
			hour4_close = 0
			hour4_high = -1
			hour4_low = upper_lmt
			T_hour4_vol = 0
			B_hour4_vol = 0
			S_hour4_vol = 0
			hour4_counter = 0
	#case 1 transactions at same minute	
	if (cur_min == next_min) and ((cur_month == next_month) and (cur_year == next_year) and (cur_day == next_day) and (cur_hour == next_hour)):
		if min1_open == 0:
			min1_open = cur_rate
		if cur_rate > min1_high:
			min1_high = cur_rate
		if cur_rate < min1_low:
			min1_low = cur_rate    
		if cur_order == 'buy':
			B_min1_vol += cur_volume
		elif cur_order == 'sell':
			S_min1_vol += cur_volume
	#case 2 next transaction is at new minute		
	else:
		min1_close = cur_rate
	#one transaction per min bug_fix1
		if min1_open == 0:
			min1_open = cur_rate
			min1_low = cur_rate
			min1_high = cur_rate
	#b1fixed till here		
		#calculate total min1 volume and sentiment
		if min1_close > min1_open:
			tag = 'buy'
			T_min1_vol = B_min1_vol + S_min1_vol
		elif min1_open > min1_close:
			tag = 'sell'
			T_min1_vol = S_min1_vol + B_min1_vol
		T_min1_vol += cur_volume	
		#create the 1minute's candlestick and add it
		candle = [cur_hour, cur_min, T_min1_vol, tag, min1_open, min1_close, min1_low, min1_high]
		M1_Candlesticks.append(candle)
		#print(candle)
		#initialise for next minute
		min1_open = 0
		min1_close = 0
		min1_high = -1
		min1_low = upper_lmt
		T_min1_vol = 0
		B_min1_vol = 0
		S_min1_vol = 0
	if(i > 0):
		prev_date = total_XMR_BTC_DB[i - 1][0]
		prev_min = int(prev_date[14:16])	
	#here we handle fixed intervals in minutes	
	#implemetation for minutes is different for hours as low frequency trades can be problematic here
	if(cur_min < 5):#same bracket
		if min5_open == 0:
			min5_open = cur_rate
		if cur_rate > min5_high:
			min5_high = cur_rate
		if cur_rate < min5_low:
			min5_low = cur_rate    
		if cur_order == 'buy':
			B_min5_vol += cur_volume
		elif cur_order == 'sell':
			S_min5_vol += cur_volume
		if(next_min > 4):#dif bracket
			min5_close = cur_rate
		#one transaction per min bug_fix1
			if min5_open == 0:
				min5_open = cur_rate
				min5_low = cur_rate
				min5_high = cur_rate
		#b1fixed till here		
			#calculate total min5 volume and sentiment
			if min5_close > min5_open:
				tag = 'buy'
				T_min5_vol = B_min5_vol + S_min5_vol
			elif min5_open > min5_close:
				tag = 'sell'
				T_min5_vol = S_min5_vol + B_min5_vol
			#T_min5_vol += cur_volume	
			#create the 5minute's candlestick and add it
			if(T_min5_vol == 0):
				T_min5_vol += cur_volume
				if cur_min == prev_min:
					T_min5_vol -= float(total_XMR_BTC_DB[i - 1][3])
			candle = [cur_hour, 0, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
			M5_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 5minutes
			min5_open = 0
			min5_close = 0
			min5_high = -1
			min5_low = upper_lmt
			T_min5_vol = 0
			B_min5_vol = 0
			S_min5_vol = 0
	if(cur_min >= 5 and cur_min < 10):
		if min5_open == 0:
			min5_open = cur_rate
		if cur_rate > min5_high:
			min5_high = cur_rate
		if cur_rate < min5_low:
			min5_low = cur_rate    
		if cur_order == 'buy':
			B_min5_vol += cur_volume
		elif cur_order == 'sell':
			S_min5_vol += cur_volume
		if(next_min > 9):#dif bracket
			min5_close = cur_rate
		#one transaction per min bug_fix1
			if min5_open == 0:
				min5_open = cur_rate
				min5_low = cur_rate
				min5_high = cur_rate
		#b1fixed till here		
			#calculate total min5 volume and sentiment
			if min5_close > min5_open:
				tag = 'buy'
				T_min5_vol = B_min5_vol + S_min5_vol
			elif min5_open > min5_close:
				tag = 'sell'
				T_min5_vol = S_min5_vol + B_min5_vol
			#T_min5_vol += cur_volume	
			#create the 5minute's candlestick and add it
			if(T_min5_vol == 0):
				T_min5_vol += cur_volume
				if cur_min == prev_min:
					T_min5_vol -= float(total_XMR_BTC_DB[i - 1][3])
			candle = [cur_hour, 5, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
			M5_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 5minutes
			min5_open = 0
			min5_close = 0
			min5_high = -1
			min5_low = upper_lmt
			T_min5_vol = 0
			B_min5_vol = 0
			S_min5_vol = 0
	if(cur_min >= 10 and cur_min < 15):
		if min5_open == 0:
			min5_open = cur_rate
		if cur_rate > min5_high:
			min5_high = cur_rate
		if cur_rate < min5_low:
			min5_low = cur_rate    
		if cur_order == 'buy':
			B_min5_vol += cur_volume
		elif cur_order == 'sell':
			S_min5_vol += cur_volume
		if(next_min > 14):#dif bracket
			min5_close = cur_rate
		#one transaction per min bug_fix1
			if min5_open == 0:
				min5_open = cur_rate
				min5_low = cur_rate
				min5_high = cur_rate
		#b1fixed till here		
			#calculate total min5 volume and sentiment
			if min5_close > min5_open:
				tag = 'buy'
				T_min5_vol = B_min5_vol + S_min5_vol
			elif min5_open > min5_close:
				tag = 'sell'
				T_min5_vol = S_min5_vol + B_min5_vol
			#T_min5_vol += cur_volume	
			#create the 5minute's candlestick and add it
			if(T_min5_vol == 0):
				T_min5_vol += cur_volume
				if cur_min == prev_min:
					T_min5_vol -= float(total_XMR_BTC_DB[i - 1][3])
			candle = [cur_hour, 10, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
			M5_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 5minutes
			min5_open = 0
			min5_close = 0
			min5_high = -1
			min5_low = upper_lmt
			T_min5_vol = 0
			B_min5_vol = 0
			S_min5_vol = 0
	if(cur_min >= 15 and cur_min < 20):
		if min5_open == 0:
			min5_open = cur_rate
		if cur_rate > min5_high:
			min5_high = cur_rate
		if cur_rate < min5_low:
			min5_low = cur_rate    
		if cur_order == 'buy':
			B_min5_vol += cur_volume
		elif cur_order == 'sell':
			S_min5_vol += cur_volume
		if(next_min > 19):#dif bracket
			min5_close = cur_rate
		#one transaction per min bug_fix1
			if min5_open == 0:
				min5_open = cur_rate
				min5_low = cur_rate
				min5_high = cur_rate
		#b1fixed till here		
			#calculate total min5 volume and sentiment
			if min5_close > min5_open:
				tag = 'buy'
				T_min5_vol = B_min5_vol + S_min5_vol
			elif min5_open > min5_close:
				tag = 'sell'
				T_min5_vol = S_min5_vol + B_min5_vol
			#T_min5_vol += cur_volume	
			#create the 5minute's candlestick and add it
			if(T_min5_vol == 0):
				T_min5_vol += cur_volume
				if cur_min == prev_min:
					T_min5_vol -= float(total_XMR_BTC_DB[i - 1][3])
			candle = [cur_hour, 15, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
			M5_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 5minutes
			min5_open = 0
			min5_close = 0
			min5_high = -1
			min5_low = upper_lmt
			T_min5_vol = 0
			B_min5_vol = 0
			S_min5_vol = 0
	if(cur_min >= 20 and cur_min < 25):
		if min5_open == 0:
			min5_open = cur_rate
		if cur_rate > min5_high:
			min5_high = cur_rate
		if cur_rate < min5_low:
			min5_low = cur_rate    
		if cur_order == 'buy':
			B_min5_vol += cur_volume
		elif cur_order == 'sell':
			S_min5_vol += cur_volume
		if(next_min > 24):#dif bracket
			min5_close = cur_rate
		#one transaction per min bug_fix1
			if min5_open == 0:
				min5_open = cur_rate
				min5_low = cur_rate
				min5_high = cur_rate
		#b1fixed till here		
			#calculate total min5 volume and sentiment
			if min5_close > min5_open:
				tag = 'buy'
				T_min5_vol = B_min5_vol + S_min5_vol
			elif min5_open > min5_close:
				tag = 'sell'
				T_min5_vol = S_min5_vol + B_min5_vol
			#T_min5_vol += cur_volume	
			#create the 5minute's candlestick and add it
			if(T_min5_vol == 0):
				T_min5_vol += cur_volume
				if cur_min == prev_min:
					T_min5_vol -= float(total_XMR_BTC_DB[i - 1][3])
			candle = [cur_hour, 20, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
			M5_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 5minutes
			min5_open = 0
			min5_close = 0
			min5_high = -1
			min5_low = upper_lmt
			T_min5_vol = 0
			B_min5_vol = 0
			S_min5_vol = 0
	if(cur_min >= 25 and cur_min < 30):
		if min5_open == 0:
			min5_open = cur_rate
		if cur_rate > min5_high:
			min5_high = cur_rate
		if cur_rate < min5_low:
			min5_low = cur_rate    
		if cur_order == 'buy':
			B_min5_vol += cur_volume
		elif cur_order == 'sell':
			S_min5_vol += cur_volume
		if(next_min > 29):#dif bracket
			min5_close = cur_rate
		#one transaction per min bug_fix1
			if min5_open == 0:
				min5_open = cur_rate
				min5_low = cur_rate
				min5_high = cur_rate
		#b1fixed till here		
			#calculate total min5 volume and sentiment
			if min5_close > min5_open:
				tag = 'buy'
				T_min5_vol = B_min5_vol + S_min5_vol
			elif min5_open > min5_close:
				tag = 'sell'
				T_min5_vol = S_min5_vol + B_min5_vol
			#T_min5_vol += cur_volume	
			#create the 5minute's candlestick and add it
			if(T_min5_vol == 0):
				T_min5_vol += cur_volume
				if cur_min == prev_min:
					T_min5_vol -= float(total_XMR_BTC_DB[i - 1][3])
			candle = [cur_hour, 25, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
			M5_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 5minutes
			min5_open = 0
			min5_close = 0
			min5_high = -1
			min5_low = upper_lmt
			T_min5_vol = 0
			B_min5_vol = 0
			S_min5_vol = 0
	if(cur_min >= 30 and cur_min < 35):
		if min5_open == 0:
			min5_open = cur_rate
		if cur_rate > min5_high:
			min5_high = cur_rate
		if cur_rate < min5_low:
			min5_low = cur_rate    
		if cur_order == 'buy':
			B_min5_vol += cur_volume
		elif cur_order == 'sell':
			S_min5_vol += cur_volume
		if(next_min > 34):#dif bracket
			min5_close = cur_rate
		#one transaction per min bug_fix1
			if min5_open == 0:
				min5_open = cur_rate
				min5_low = cur_rate
				min5_high = cur_rate
		#b1fixed till here		
			#calculate total min5 volume and sentiment
			if min5_close > min5_open:
				tag = 'buy'
				T_min5_vol = B_min5_vol + S_min5_vol
			elif min5_open > min5_close:
				tag = 'sell'
				T_min5_vol = S_min5_vol + B_min5_vol
			#T_min5_vol += cur_volume	
			#create the 5minute's candlestick and add it
			if(T_min5_vol == 0):
				T_min5_vol += cur_volume
				if cur_min == prev_min:
					T_min5_vol -= float(total_XMR_BTC_DB[i - 1][3])
			candle = [cur_hour, 30, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
			M5_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 5minutes
			min5_open = 0
			min5_close = 0
			min5_high = -1
			min5_low = upper_lmt
			T_min5_vol = 0
			B_min5_vol = 0
			S_min5_vol = 0
	if(cur_min >= 35 and cur_min < 40):
		if min5_open == 0:
			min5_open = cur_rate
		if cur_rate > min5_high:
			min5_high = cur_rate
		if cur_rate < min5_low:
			min5_low = cur_rate    
		if cur_order == 'buy':
			B_min5_vol += cur_volume
		elif cur_order == 'sell':
			S_min5_vol += cur_volume
		if(next_min > 39):#dif bracket
			min5_close = cur_rate
		#one transaction per min bug_fix1
			if min5_open == 0:
				min5_open = cur_rate
				min5_low = cur_rate
				min5_high = cur_rate
		#b1fixed till here		
			#calculate total min5 volume and sentiment
			if min5_close > min5_open:
				tag = 'buy'
				T_min5_vol = B_min5_vol + S_min5_vol
			elif min5_open > min5_close:
				tag = 'sell'
				T_min5_vol = S_min5_vol + B_min5_vol
			#T_min5_vol += cur_volume	
			#create the 5minute's candlestick and add it
			if(T_min5_vol == 0):
				T_min5_vol += cur_volume
				if cur_min == prev_min:
					T_min5_vol -= float(total_XMR_BTC_DB[i - 1][3])
			candle = [cur_hour, 35, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
			M5_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 5minutes
			min5_open = 0
			min5_close = 0
			min5_high = -1
			min5_low = upper_lmt
			T_min5_vol = 0
			B_min5_vol = 0
			S_min5_vol = 0
	if(cur_min >= 40 and cur_min < 45):
		if min5_open == 0:
			min5_open = cur_rate
		if cur_rate > min5_high:
			min5_high = cur_rate
		if cur_rate < min5_low:
			min5_low = cur_rate    
		if cur_order == 'buy':
			B_min5_vol += cur_volume
		elif cur_order == 'sell':
			S_min5_vol += cur_volume
		if(next_min > 44):#dif bracket
			min5_close = cur_rate
		#one transaction per min bug_fix1
			if min5_open == 0:
				min5_open = cur_rate
				min5_low = cur_rate
				min5_high = cur_rate
		#b1fixed till here		
			#calculate total min5 volume and sentiment
			if min5_close > min5_open:
				tag = 'buy'
				T_min5_vol = B_min5_vol + S_min5_vol
			elif min5_open > min5_close:
				tag = 'sell'
				T_min5_vol = S_min5_vol + B_min5_vol
			#T_min5_vol += cur_volume	
			#create the 5minute's candlestick and add it
			if(T_min5_vol == 0):
				T_min5_vol += cur_volume
				if cur_min == prev_min:
					T_min5_vol -= float(total_XMR_BTC_DB[i - 1][3])
			candle = [cur_hour, 40, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
			M5_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 5minutes
			min5_open = 0
			min5_close = 0
			min5_high = -1
			min5_low = upper_lmt
			T_min5_vol = 0
			B_min5_vol = 0
			S_min5_vol = 0
	if(cur_min >= 45 and cur_min < 50):
		if min5_open == 0:
			min5_open = cur_rate
		if cur_rate > min5_high:
			min5_high = cur_rate
		if cur_rate < min5_low:
			min5_low = cur_rate    
		if cur_order == 'buy':
			B_min5_vol += cur_volume
		elif cur_order == 'sell':
			S_min5_vol += cur_volume
		if(next_min > 49):#dif bracket
			min5_close = cur_rate
		#one transaction per min bug_fix1
			if min5_open == 0:
				min5_open = cur_rate
				min5_low = cur_rate
				min5_high = cur_rate
		#b1fixed till here		
			#calculate total min5 volume and sentiment
			if min5_close > min5_open:
				tag = 'buy'
				T_min5_vol = B_min5_vol + S_min5_vol
			elif min5_open > min5_close:
				tag = 'sell'
				T_min5_vol = S_min5_vol + B_min5_vol
			#T_min5_vol += cur_volume	
			#create the 5minute's candlestick and add it
			if(T_min5_vol == 0):
				T_min5_vol += cur_volume
				if cur_min == prev_min:
					T_min5_vol -= float(total_XMR_BTC_DB[i - 1][3])
			candle = [cur_hour, 45, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
			M5_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 5minutes
			min5_open = 0
			min5_close = 0
			min5_high = -1
			min5_low = upper_lmt
			T_min5_vol = 0
			B_min5_vol = 0
			S_min5_vol = 0
	if(cur_min >= 50 and cur_min < 55):
		if min5_open == 0:
			min5_open = cur_rate
		if cur_rate > min5_high:
			min5_high = cur_rate
		if cur_rate < min5_low:
			min5_low = cur_rate    
		if cur_order == 'buy':
			B_min5_vol += cur_volume
		elif cur_order == 'sell':
			S_min5_vol += cur_volume
		if(next_min > 54):#dif bracket
			min5_close = cur_rate
		#one transaction per min bug_fix1
			if min5_open == 0:
				min5_open = cur_rate
				min5_low = cur_rate
				min5_high = cur_rate
		#b1fixed till here		
			#calculate total min5 volume and sentiment
			if min5_close > min5_open:
				tag = 'buy'
				T_min5_vol = B_min5_vol + S_min5_vol
			elif min5_open > min5_close:
				tag = 'sell'
				T_min5_vol = S_min5_vol + B_min5_vol
			#T_min5_vol += cur_volume	
			#create the 5minute's candlestick and add it
			if(T_min5_vol == 0):
				T_min5_vol += cur_volume
				if cur_min == prev_min:
					T_min5_vol -= float(total_XMR_BTC_DB[i - 1][3])
			candle = [cur_hour, 50, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
			M5_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 5minutes
			min5_open = 0
			min5_close = 0
			min5_high = -1
			min5_low = upper_lmt
			T_min5_vol = 0
			B_min5_vol = 0
			S_min5_vol = 0
	if(cur_min >= 55 and cur_min < 60):
		if min5_open == 0:
			min5_open = cur_rate
		if cur_rate > min5_high:
			min5_high = cur_rate
		if cur_rate < min5_low:
			min5_low = cur_rate    
		if cur_order == 'buy':
			B_min5_vol += cur_volume
		elif cur_order == 'sell':
			S_min5_vol += cur_volume
		if(next_hour != cur_hour):#dif bracket
			min5_close = cur_rate
		#one transaction per min bug_fix1
			if min5_open == 0:
				min5_open = cur_rate
				min5_low = cur_rate
				min5_high = cur_rate
		#b1fixed till here		
			#calculate total min5 volume and sentiment
			if min5_close > min5_open:
				tag = 'buy'
				T_min5_vol = B_min5_vol + S_min5_vol
			elif min5_open > min5_close:
				tag = 'sell'
				T_min5_vol = S_min5_vol + B_min5_vol
			#T_min5_vol += cur_volume	
			#create the 5minute's candlestick and add it
			if(T_min5_vol == 0):
				T_min5_vol += cur_volume
				if cur_min == prev_min:
					T_min5_vol -= float(total_XMR_BTC_DB[i - 1][3])
			candle = [cur_hour, 55, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
			M5_Candlesticks.append(candle)
			#print(candle)
			#initialise for next 5minutes
			min5_open = 0
			min5_close = 0
			min5_high = -1
			min5_low = upper_lmt
			T_min5_vol = 0
			B_min5_vol = 0
			S_min5_vol = 0				
	#logika me % mporeis na elenxeis tin anisotita alla gamiete ligo me tin sintiki gia to 0, allios thelei allagei o algo me cur,next
	#episis gia pio compact code sto mellon mporeis na mplekseis ta 5 lepta me ta alla me counter, i entheta if , pollla if...

#for non perfectly divided timeframes (4d,1w,2w etc) candles created here
# we are in the same 4day frame
	if day4_counter != 4:
		if day4_open == 0:
			day4_open = cur_rate
		if cur_rate > day4_high:
			day4_high = cur_rate
		if cur_rate < day4_low:
			day4_low = cur_rate
		if cur_order == 'buy':
			B_day4_vol += cur_volume
		elif cur_order == 'sell':
			S_day4_vol += cur_volume	
# else we are in next 4day frame
	else:
		day4_counter = 0
		day4_close = cur_rate
		#calculate total 4 days volume and sentiment
		if day4_close > day4_open:
			tag = 'buy'
			T_day4_vol = B_day4_vol + S_day4_vol
		elif day4_open > day4_close:
			tag = 'sell'
			T_day4_vol = S_day4_vol + B_day4_vol
		T_day4_vol += cur_volume	
		#create the 4 days candlestick and add it
		candle = [day4_open_month, day4_open_day, T_day4_vol, tag, day4_open, day4_close, day4_low, day4_high]
		D4_Candlesticks.append(candle)
		#print(candle)
		#initialise for next 4 days
		day4_open = 0
		day4_close = 0
		day4_high = -1
		day4_low = upper_lmt
		T_day4_vol = 0
		B_day4_vol = 0
		S_day4_vol = 0
		day4_open_month = next_month
		day4_open_day = next_day
# we are in the same 1 week frame
	if week1_counter != 7:
		if week1_open == 0:
			week1_open = cur_rate
		if cur_rate > week1_high:
			week1_high = cur_rate
		if cur_rate < week1_low:
			week1_low = cur_rate
		if cur_order == 'buy':
			B_week1_vol += cur_volume
		elif cur_order == 'sell':
			S_week1_vol += cur_volume	
# else we are in next 1 week frame
	else:
		week1_counter = 0
		week1_close = cur_rate
		#calculate total 1 week's volume and sentiment
		if week1_close > week1_open:
			tag = 'buy'
			T_week1_vol = B_week1_vol + S_week1_vol
		elif week1_open > week1_close:
			tag = 'sell'
			T_week1_vol = S_week1_vol + B_week1_vol
		T_week1_vol += cur_volume	
		#create the 1 week's candlestick and add it
		candle = [week1_open_month, week1_open_day, T_week1_vol, tag, week1_open, week1_close, week1_low, week1_high]
		W1_Candlesticks.append(candle)
		#print(candle)
		#initialise for next 1 week
		week1_open = 0
		week1_close = 0
		week1_high = -1
		week1_low = upper_lmt
		T_week1_vol = 0
		B_week1_vol = 0
		S_week1_vol = 0
		week1_open_month = next_month
		week1_open_day = next_day
	# we are in the same 2 weeks frame
	if week2_counter != 14:
		if week2_open == 0:
			week2_open = cur_rate
		if cur_rate > week2_high:
			week2_high = cur_rate
		if cur_rate < week2_low:
			week2_low = cur_rate
		if cur_order == 'buy':
			B_week2_vol += cur_volume
		elif cur_order == 'sell':
			S_week2_vol += cur_volume	
# else we are in next 2 weeks frame
	else:
		week2_counter = 0
		week2_close = cur_rate
		#calculate total 2 week's volume and sentiment
		if week2_close > week2_open:
			tag = 'buy'
			T_week2_vol = B_week2_vol + S_week2_vol
		elif week2_open > week2_close:
			tag = 'sell'
			T_week2_vol = S_week2_vol + B_week2_vol
		T_week2_vol += cur_volume	
		#create the 2 week's candlestick and add it
		candle = [week2_open_month, week2_open_day, T_week2_vol, tag, week2_open, week2_close, week2_low, week2_high]
		W2_Candlesticks.append(candle)
		#print(candle)
		#initialise for next 2 weeks
		week2_open = 0
		week2_close = 0
		week2_high = -1
		week2_low = upper_lmt
		T_week2_vol = 0
		B_week2_vol = 0
		S_week2_vol = 0
		week2_open_month = next_month
		week2_open_day = next_day	
# we reached last transaction, finish up			
month_close = next_rate
if month_close > month_open:
	tag = 'buy'
	T_month_vol = B_month_vol + S_month_vol
elif month_open > month_close:
	tag = 'sell'
	T_month_vol = S_month_vol + B_month_vol	
T_month_vol += next_volume	
candle = [next_year, next_month, T_month_vol, tag, month_open, month_close, month_low, month_high]
MN_Candlesticks.append(candle)
#print(candle)
week1_close = next_rate
if week1_close > week1_open:
	tag = 'buy'
	T_week1_vol = B_week1_vol + S_week1_vol
elif week1_open > week1_close:
	tag = 'sell'
	T_week1_vol = S_week1_vol + B_week1_vol	
T_week1_vol += next_volume	
candle = [week1_open_month, week1_open_day, T_week1_vol, tag, week1_open, week1_close, week1_low, week1_high]
W1_Candlesticks.append(candle)
#print(candle)
week2_close = next_rate
if week2_close > week2_open:
	tag = 'buy'
	T_week2_vol = B_week2_vol + S_week2_vol
elif week2_open > week2_close:
	tag = 'sell'
	T_week2_vol = S_week2_vol + B_week2_vol	
T_week2_vol += next_volume	
candle = [week2_open_month, week2_open_day, T_week2_vol, tag, week2_open, week2_close, week2_low, week2_high]
W2_Candlesticks.append(candle)
#print(candle)
day1_close = next_rate
if day1_close > day1_open:
	tag = 'buy'
	T_day1_vol = B_day1_vol + S_day1_vol
elif day1_open > day1_close:
	tag = 'sell'
	T_day1_vol = S_day1_vol + B_day1_vol	
T_day1_vol += next_volume	
candle = [next_month, next_day, T_day1_vol, tag, day1_open, day1_close, day1_low, day1_high]
D1_Candlesticks.append(candle)
#print(candle)
day4_close = next_rate
if day4_close > day4_open:
	tag = 'buy'
	T_day4_vol = B_day4_vol + S_day4_vol
elif day4_open > day4_close:
	tag = 'sell'
	T_day4_vol = S_day4_vol + B_day4_vol	
T_day4_vol += next_volume	
candle = [day4_open_month, day4_open_day, T_day4_vol, tag, day4_open, day4_close, day4_low, day4_high]
D4_Candlesticks.append(candle)
#print(candle)
hour1_close = next_rate
if hour1_close > hour1_open:
	tag = 'buy'
	T_hour1_vol = B_hour1_vol + S_hour1_vol
elif hour1_open > hour1_close:
	tag = 'sell'
	T_hour1_vol = S_hour1_vol + B_hour1_vol	
T_hour1_vol += next_volume	
candle = [next_day, next_hour, T_hour1_vol, tag, hour1_open, hour1_close, hour1_low, hour1_high]
H1_Candlesticks.append(candle)
#print(candle)
hour4_close = next_rate
if hour4_close > hour4_open:
	tag = 'buy'
	T_hour4_vol = B_hour4_vol + S_hour4_vol
elif hour4_open > hour4_close:
	tag = 'sell'
	T_hour4_vol = S_hour4_vol + B_hour4_vol	
T_hour4_vol += next_volume	
candle = [next_day, next_hour - 3, T_hour4_vol, tag, hour4_open, hour4_close, hour4_low, hour4_high]
H4_Candlesticks.append(candle)
#print(candle)
if min1_open == 0:
	min1_open = cur_rate
	min1_low = cur_rate
	min1_high = cur_rate
min1_close = next_rate
if min1_close > min1_open:
	tag = 'buy'
	T_min1_vol = B_min1_vol + S_min1_vol
elif min1_open > min1_close:
	tag = 'sell'
	T_min1_vol = S_min1_vol + B_min1_vol	
T_min1_vol += next_volume	
candle = [next_hour, next_min, T_min1_vol, tag, min1_open, min1_close, min1_low, min1_high]
M1_Candlesticks.append(candle)
#print(candle)
min5_close = next_rate
if min5_open == 0:
	min5_open = cur_rate
	min5_low = cur_rate
	min5_high = cur_rate
if min5_close > min5_open:
	tag = 'buy'
	T_min5_vol = B_min5_vol + S_min5_vol
elif min5_open > min5_close:
	tag = 'sell'
	T_min5_vol = S_min5_vol + B_min5_vol	
T_min5_vol += next_volume	
candle = [next_hour, next_min, T_min5_vol, tag, min5_open, min5_close, min5_low, min5_high]
M5_Candlesticks.append(candle)
#print(candle)
total_exchange_volume_left += next_volume
total_exchange_volume_right += (next_volume * next_rate)
with open('results.txt', 'w') as f:
	f.write('1 month candles below\n')
	for mcandle in MN_Candlesticks:
		f.write("%s\n" % mcandle)
	f.write('2 week candles below\n')
	for	w2candle in W2_Candlesticks:
		f.write("%s\n" % w2candle)
	f.write('1 week candles below\n')	
	for w1candle in W1_Candlesticks:
		f.write("%s\n" % w1candle)
	f.write('4 day candles below\n')	
	for d4candle in D4_Candlesticks:
		f.write("%s\n" % d4candle)		
	f.write('1 day candles below\n')	
	for d1candle in D1_Candlesticks:
		f.write("%s\n" % d1candle)
	f.write('4 hour candles below\n')	
	for h4candle in H4_Candlesticks:
		f.write("%s\n" % h4candle)
	f.write('1 hour candles below\n')	
	for h1candle in H1_Candlesticks:
		f.write("%s\n" % h1candle)
	f.write('1 minute candles below\n')		
	for m1candle in M1_Candlesticks:
		f.write("%s\n" % m1candle)
	f.write('5 minute candles below\n')		
	for m5candle in M5_Candlesticks:
		f.write("%s\n" % m5candle)	
f.close()
stop_candlemaker = timeit.default_timer()
print('Candlemaking completed in :',(stop_candlemaker - start_candlemaker),'seconds!')
print(len(MN_Candlesticks),'1-month candles created!')
print(len(W2_Candlesticks),'2-week candles created!')
print(len(W1_Candlesticks),'1-week candles created!')
print(len(D4_Candlesticks),'4-day candles created!')
print(len(D1_Candlesticks),'1-day candles created!')
print(len(H4_Candlesticks),'4-hour candles created!')
print(len(H1_Candlesticks),'1-hour candles created!')
print(len(M1_Candlesticks),'1-minute candles created!')
print(len(M5_Candlesticks),'5-minute candles created!')
print('Total volume in XMR:', total_exchange_volume_left)
print('Total volume in BTC:', total_exchange_volume_right)