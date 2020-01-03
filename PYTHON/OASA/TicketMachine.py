from decimal import Decimal, ROUND_UP


class TicketMachine:

    TICKET_PRICE_NORMAL = 1.4
    TICKET_PRICE_REDUCED = 0.6
    DISCOUNT_NORMAL = 0.9
    DISCOUNT_REDUCED = 0.95
    VALID_COINS = ["0.01", "0.02", "0.05", "0.1", "0.2", "0.5", "1", "2"]

    def __init__(self, tickets_ordered=0, user_type="X", tickets_type="Τύπος",
                 total_cost=0.0, change=0.0, amount_inserted=0.0):
        # constructor method
        self.tickets_ordered = tickets_ordered
        self.user_type = user_type
        self.tickets_type = tickets_type
        self.total_cost = total_cost
        self.change = change
        self.amount_inserted = amount_inserted

    def greeting(self):
        # initial greeting method
        print("Καλωσήρθατε καινούριε/α χρήστη!")

    def set_identity(self):
        # this method sets the user identity type until valid (Α, Φ, Ε, Η)
        while self.user_type not in ["Α", "Φ", "Ε", "Η"]:
            print("A = Ανήλικος")
            print("Φ = Φοιτητής")
            print("Ε = Ενήλικας")
            print("Η = Ηλικιωμένος")
            self.user_type = input("Παρακαλώ εισάγετε μία από τις παραπάνω "
                                   "ιδιότητες : ")
        return self.user_type

    def order_tickets(self):
        # this method sets the number of tickets needed until valid (1-4)
        while (self.tickets_ordered < 1) or (self.tickets_ordered > 4):
            reply = input("Παρακαλώ εισάγετε το πλήθος (ελάχιστο 1, μέγιστο 4) "
                          "των εισιτηριών προς αγορά : ")
            if reply.isnumeric():
                self.tickets_ordered = int(reply)
        return self.tickets_ordered

    def set_tickets_type(self):
        # this method sets the type (Ενιαίο, Μειωμένο) of tickets to be bought
        # and checks validity based on user_type
        if self.user_type in ["Α", "Φ", "Η"]:
            while self.tickets_type != "Μειωμένο":
                self.tickets_type = input("Παρακαλώ εισάγετε την κατηγορία "
                                          "(Ενιαίο, Μειωμένο) "
                                          "του εισιτηρίου : ")
        elif self.user_type == "Ε":
            while self.tickets_type != "Ενιαίο":
                self.tickets_type = input("Παρακαλώ εισάγετε την κατηγορία "
                                          "(Ενιαίο, Μειωμένο) "
                                          "του εισιτηρίου : ")
        return self.tickets_type

    def buy_tickets(self):
        # this method calculates the cost of the tickets and waits until
        # the total amount is filled accepting 1 coin each time
        if self.tickets_type == "Ενιαίο":
            if self.tickets_ordered == 4:
                # user is eligible for a discount
                self.total_cost = (self.tickets_ordered *
                                   self.TICKET_PRICE_NORMAL) * 0.9
            else:
                self.total_cost = (self.tickets_ordered *
                                   self.TICKET_PRICE_NORMAL)
        elif self.tickets_type == "Μειωμένο":
            if self.tickets_ordered >= 3:
                # user is eligible for a discount
                self.total_cost = (self.tickets_ordered *
                                   self.TICKET_PRICE_REDUCED) * 0.95
            else:
                self.total_cost = (self.tickets_ordered *
                                   self.TICKET_PRICE_REDUCED)
        # quantize the cost for monetary use (up to 2 decimal digits)
        self.total_cost = Decimal(self.total_cost).quantize(Decimal('.01'))
        print("Το συνολικό κόστος είναι " + str(self.total_cost) + " €")
        return self.total_cost

    def pay_cost(self):
        # this method counts inserted coins and checks for their validity
        print("Παρακαλώ εισάγετε κέρματα των {1(0.01€), 2(0.02€), 5(0.05€),"
              " 10(0.1€), 20(0.2€) ή 50(0.5€)} λεπτών του € και {1 ή 2} €")
        while True:
            current_input = input("Εισαγωγή κέρματος: ")
            if current_input == "ΟΚ":
                break
            elif current_input not in self.VALID_COINS:
                print("Παρακαλώ εισάγετε κέρματα των {1(0.01€), 2(0.02€),"
                      " 5(0.05€), 10(0.1€), 20(0.2€) ή 50(0.5€)} λεπτών του € "
                      "και {1 ή 2} €")
            else:
                self.amount_inserted += float(current_input)
        return self.amount_inserted

    def get_change(self):
        self.change = Decimal(self.amount_inserted) - Decimal(self.total_cost)
        return self.change

    def print_receipt(self, user_type, tickets_ordered, tickets_type,
                      total_cost, amount_inserted, change):
        print("Απόδειξη Συναλλαγής :")
        if user_type == "Φ":
            print("Ιδιότητα Χρήστη : Φοιτητής")
        elif user_type == "Η":
            print("Ιδιότητα Χρήστη : Ηλικιωμένος")
        elif user_type == "Α":
            print("Ιδιότητα Χρήστη : Ανήλικος")
        else:
            print("Ιδιότητα Χρήστη : Ενήλικας")
        print("Πλήθος Εισιτηρίων : " + str(tickets_ordered))
        print("Είδος Εισιτηρίων : " + tickets_type)
        print("Κόστος Εισιτηρίων : " + str(total_cost) + " €")
        print("Πληρωθέν Ποσό : " + str(amount_inserted) + " €")
        print("Ρέστα : " + str(change) + " €")
