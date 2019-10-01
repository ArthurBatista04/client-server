from ctypes import *


class STATUS(Structure):
    _pack_ = 1
    _fields_ = [("code", c_int), ("message", c_char*100)]


class CUSTOMER(Structure):
    _pack_ = 1
    _fields_ = [("id", c_int),
                ("name", c_char*100),
                ("age", c_int)
                ]


class DATA(Structure):
    _pack_ = 1
    _fields_ = [("method", c_int), ("customer", CUSTOMER),  ("status", STATUS)]


def customer_menu(option, s):
    if option.lower() in ['a', '']:
        id = raw_input("ID (0-99):")
        try:
            id = int(id)
        except:
            print("ERROR: id must be a number!")
            return
        if id not in range(0, 100):
            print('ERROR: id must be between 0 through 99!')
            return
        name = raw_input('Name:')
        age = raw_input('Age:')
        try:
            age = int(age)
        except:
            print("ERROR: age must be a number!")
            return
        customer_input = CUSTOMER(id=id, name=name, age=age)

        print("Sending data to server...")
        data = DATA(customer=customer_input, method=1,
                    status=STATUS(code=0, message='None'))

    elif option.lower() in ['f']:
        id = int(raw_input("Input the customer's ID: "))
        customer_input = CUSTOMER(id=id, name='None', age=0)
        data = DATA(customer=customer_input, method=2,
                    status=STATUS(code=0, message='None'))
    else:
        print("ERROR: wrong input. Try again!")
        return

    nsent = s.send(data)
    print("Sent %d bytes" % nsent)
    buff = s.recv(sizeof(DATA))
    data_output = DATA.from_buffer_copy(buff)
    print("\nMessage from server")
    print('Status code: %d ' % data_output.status.code)
    print(data_output.status.message)
    if data_output.customer.id != -1:
        print("ID = %d, Name = %s and Age = %d" % (data_output.customer.id,
                                                   data_output.customer.name,
                                                   data_output.customer.age
                                                   ))
