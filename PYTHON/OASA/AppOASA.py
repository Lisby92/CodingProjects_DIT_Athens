from decimal import Decimal

import TicketMachine

# create the instance of TicketMachine
ticket_machine = TicketMachine.TicketMachine()
# greet the user
ticket_machine.greeting()
# set the user identity and store it
user_type = ticket_machine.set_identity()
# set the number of tickets the user want and store it
user_tickets = ticket_machine.order_tickets()
# set the type of ticket the user wants and store it
tickets_type = ticket_machine.set_tickets_type()
# set the ticket buy order and store the total cost in €
total_cost = ticket_machine.buy_tickets()
# prompt user to insert coins until "OK" is inserted and check for validity
# when the user inserts the whole amount , the loop exits
user_amount_inserted = Decimal(0.0)
while True:
    user_amount_inserted = Decimal(ticket_machine.pay_cost())
    amount_remaining = Decimal(total_cost - user_amount_inserted) \
        .quantize(Decimal('.01'))
    if amount_remaining > 0.0:
        # prompt the user to insert the remaining coins
        print("Το υπολειπόμενο ποσό είναι : " + str(amount_remaining) + " €")
    else:
        # user has inserted at least the required amount
        user_change = Decimal(ticket_machine.get_change())\
            .quantize(Decimal('.01'))
        print("Το ποσό πληρώθηκε επιτυχώς!")
        print("Τα ρέστα σας είναι : " + str(user_change) + " €")
        break
user_amount_inserted = Decimal(user_amount_inserted).quantize(Decimal('.01'))
ticket_machine.print_receipt(user_type, user_tickets, tickets_type, total_cost,
                             user_amount_inserted, user_change)
