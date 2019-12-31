CC = g++
CFLAGS = -g -Wall

project2: Student_Day.o sim_SD.o
	$(CC) $(CFLAGS) -o project2 Student_Day.o sim_SD.o

Student_Day.o: Student_Day.cpp
	$(CC) $(CFLAGS) -c Student_Day.cpp -o Student_Day.o

sim_SD.o: sim_SD.cpp
	$(CC) $(CFLAGS) -c sim_SD.cpp -o sim_SD.o

.PHONY: clean

clean:
	rm -f project2 project2.o Student_Day.o sim_SD.o