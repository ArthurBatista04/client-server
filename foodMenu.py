from ctypes import *


class STATUS(Structure):
    _pack_ = 1
    _fields_ = [("code", c_int), ("message", c_char*100)]


class FOOD(Structure):
    _pack_ = 1
    _fields_ = [("id", c_int),
                ("name", c_char*100),
                ("category", c_char*100),
                ("price", c_float)
                ]


class DATA(Structure):
    _pack_ = 1
    _fields_ = [("method", c_int), ("food", FOOD),  ("status", STATUS)]


def food_menu(option, s):
    if option.lower() in ['a', '']:
        id = raw_input("ID (0-99): ")
        try:
            id = int(id)
        except:
            print("ERROR: id must be a number!")
            return
        if id not in range(0, 100):
            print('ERROR: id must be between 0 through 99!')
            return
        name = raw_input('Name: ')
        category = raw_input("Category: ")
        price = raw_input('Price: ')
        try:
            price = float(price)
        except:
            print("ERROR: price must be a number!")
            return
        FOOD_input = FOOD(id=id, name=name, price=price, category=category)

        print("Sending data to server...")
        data = DATA(food=FOOD_input, method=1,
                    status=STATUS(code=0, message='None'))

    elif option.lower() in ['f']:
        id = raw_input("ID: ")
        try:
            id = int(id)
        except:
            print("ERROR: id must be a number!")
            return
        FOOD_input = FOOD(id=id, name='None', price=-1, category='None')
        data = DATA(food=FOOD_input, method=2,
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
    if data_output.food.id != -1:
        print("ID = %d, Name = %s, price = %f and category = %s" % (data_output.food.id,
                                                                    data_output.food.name,
                                                                    data_output.food.price,
                                                                    data_output.food.category
                                                                    ))
