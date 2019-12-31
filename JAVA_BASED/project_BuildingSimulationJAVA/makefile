JFLAGS = -g
JCC = javac

default: SimC1.class Building.class Citizen.class Escort.class Space.class \
		 EntryArea.class Elevator.class Floor.class Office.class

SimC1.class:	SimC1.java
		$(JCC) $(JFLAGS) SimC1.java

Building.class: Building.java
		$(JCC) $(JFLAGS) Building.java

Citizen.class: Citizen.java
		$(JCC) $(JFLAGS) Citizen.java

Escort.class: Escort.java
		$(JCC) $(JFLAGS) Escort.java

Space.class: Space.java
		$(JCC) $(JFLAGS) Space.java

EntryArea.class: EntryArea.java
		$(JCC) $(JFLAGS) EntryArea.java

Elevator.class: Elevator.java
		$(JCC) $(JFLAGS) Elevator.java

Floor.class: Floor.java
		$(JCC) $(JFLAGS) Floor.java

Office.class: Office.java
		$(JCC) $(JFLAGS) Office.java

clean: 
		$(RM) *.class