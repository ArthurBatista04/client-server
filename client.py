import socket
import sys
import random
import os
from customerMenu import customer_menu
from foodMenu import food_menu


def main():
    server_addr = ('localhost', 38110)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        s.connect(server_addr)
        print("Connected to %s" % repr(server_addr))
    except:
        print("ERROR: Connection to %s refused" % repr(server_addr))
        sys.exit(1)

    try:
        while True:
            print("Please select a DATABASE.")
            option = raw_input(
                "1 - Customer | 2 - Food | 3 - Drink: ")
            if option is "1":
                option = raw_input('Add or fetch customer?(A/f)')
                customer_menu(option, s)
            elif option is "2":
                option = raw_input('Add food or fetch menu?(A/f)')
                food_menu(option, s)
            option = raw_input('Do you wish to continue?(y/N)')
            if option not in ['y']:
                break
            else:
                os.system('clear')

    finally:
        print("Closing socket")
        s.close()


if __name__ == "__main__":
    main()
