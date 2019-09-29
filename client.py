import socket
import sys
import random
import os
from ctypes import *

class Error(Structure):
    _pack_ = 1
    _fields_ = [("message",c_char_p),("code",c_uint32)]

class Customer(Structure):
    _pack_ = 1
    _fields_ = [("id", c_uint32),
                ("name", c_char_p),
                 ("cpf", c_char_p),
                ("age", c_uint32)
                ]
class Data(Structure):
    _pack_ = 1
    _fields_ =[("method",c_uint32), ("customer",Customer), ("error",Error) ]

def main():
    server_addr = ('localhost', 38107)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        s.connect(server_addr)
        print("Connected to %s" % repr(server_addr))
    except:
        print("ERROR: Connection to %s refused" % repr(server_addr))
        sys.exit(1)

    try:
        while True:
            option = raw_input('Create or read customer?(C/r)')
            if option in ['C']:
                name = raw_input('Name: ')
                cpf = raw_input('Cpf: ')
                age = int(raw_input('Age: '))
                customer_input = Customer(id=int(random.uniform(1,100)),name = name, cpf = str(cpf), age = age) 

                print("Sending id=%d, name=%s, cpf=%s, age=%d"  % (customer_input.id,
                                                customer_input.name,
                                                customer_input.cpf,
                                                customer_input.age
                                                ))
                data = Data(customer = customer_input, method=1, error=Error(message=None, code=0))

                nsent = s.send(data)
                print("Sent %d bytes" % nsent)

                buff = s.recv(sizeof(Data))
                data_output = Data.from_buffer_copy(buff)
                print(data_output.method, data_output.customer.name)   
            else:
                pass
            
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
