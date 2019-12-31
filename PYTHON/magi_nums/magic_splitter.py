import math
#declarations
list_num = []
new_list = []
num_str = ""
new_str = ""
suma = 0
length = 0
new_length = 0
i = 0
j = 0
lower =0
upper = 0
sop = 0.0
mop = 0.0
three = 0
six = 0
nine = 0
mutual = 0
#program start
file = open("magic_nums.txt","w+")
print("Please give lower limit!")
lower = int(input())
print("Please give upper limit!")
upper = int(input())
#main loop
for number in range(lower, (upper+1), 1):
	#3 first cases dont need processing
	if number is 3:
		file.write(str(number)+" -- turns into : 3 - strength solo/opposite : infinite, strength mutual/opposite : undefined\n")
	elif number is 6:
		file.write(str(number)+" -- turns into : 6 - strength solo/opposite : infinite, strength mutual/opposite : undefined\n")
	elif number is 9:
		file.write(str(number)+" -- turns into : 9 - strength solo : infinite, strength solo/opposite : undefined\n")
	#general cases below
	elif (number > 9):
		num_str = str(number)
		length = len(num_str)
		#print(num_str[0]+"-"+num_str[1])
		for i in range(0, length, 1):
			list_num.append(int(num_str[i]))
		suma = sum(list_num, 0)
		list_num.clear()
		while suma > 9:
			new_str = str(suma)
			new_length = len(new_str)
			for j in range(0, new_length, 1):
				new_list.append(int(new_str[j]))
			suma = sum(new_list, 0)
			new_list.clear()
		if suma is 3:
			for i in range(0, length, 1):
				if (int(num_str[i]) == 3):
					three += 1
					mutual += 1
				elif (int(num_str[i]) == 6):
					six += 1
					mutual += 1
				elif (int(num_str[i]) == 9):
					nine += 1
			if(nine == 0):
				sop = three
				mop = mutual
			else:		
				sop = three / nine
				mop = mutual / nine
			three = 0
			six = 0
			nine = 0
			mutual = 0			
			file.write(str(number)+" -- turns into : 3 - strength solo/opposite : "+str(sop)+" , strenght mutual/opposite : "+str(mop)+"\n")
		elif suma is 6:
			for i in range(0, length, 1):
				if (int(num_str[i]) == 3):
					three += 1
					mutual += 1
				elif (int(num_str[i]) == 6):
					six += 1
					mutual += 1
				elif (int(num_str[i]) == 9):
					nine += 1
			if(nine == 0):
				sop = six
				mop = mutual
			else:	
				sop = six / nine
				mop = mutual / nine
			three = 0
			six = 0
			nine = 0
			mutual = 0				
			file.write(str(number)+" -- turns into : 6 - strength solo/opposite : "+str(sop)+" , strenght mutual/opposite : "+str(mop)+"\n")
		elif suma is 9:
			for i in range(0, length, 1):
				if (int(num_str[i]) == 3):
					three += 1
					mutual += 1
				elif (int(num_str[i]) == 6):
					six += 1
					mutual += 1
				elif (int(num_str[i]) == 9):
					nine += 1
			sop = nine
			if(mutual == 0):
				mop = nine
			else:
				mop = nine / mutual
			three = 0
			six = 0
			nine = 0
			mutual = 0				
			file.write(str(number)+" -- turns into : 9 - strength solo : "+str(sop)+" , strenght solo/opposite : "+str(mop)+"\n")

#end of program