import csv
import math
import timeit

######################################
############ DATA_PARSER #############
######################################
start_parser = timeit.default_timer()
X_Y_TRANSACTIONS = []       	  # all history 
total_exchange_volume_X = 0.0  # total trading volume measured in left pair
total_exchange_volume_Y = 0.0 # total trading volume measured in left pair
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