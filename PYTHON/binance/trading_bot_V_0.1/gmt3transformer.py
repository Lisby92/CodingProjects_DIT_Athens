def GMT3_transformer(year, month, day, hour, minute):
	
	hour += 3
	if hour >= 24:
		hour -= 24
		if month % 2 == 1:
			if month != 1:
				if day == 31:
					day = 1
					month += 1	
				else:
					day += 1
		elif month % 2 == 0:
			if month == 12:
				if day == 30:
					day = 1
					month = 1
					year += 1
				else:
					day += 1	
			elif month != 2:
				if day == 30:
					day = 1			
					month += 1
				else:
					day += 1
			else:
				flag = 0
				if year % 4 == 0:
					if year % 100 != 0:
						flag = 1
				if flag == 0:
					if year % 400 == 0:
						flag = 1
				if flag == 0:
					if day == 28:
						day = 1
						month += 1
					else:
						day += 1
				else:
					if day == 29:
						day = 1
						month += 1
					else:
						day += 1
						
	date = [year, month, day, hour, minute]
	return date