import socket
import sys
import random
from ctypes import *


class Payload(Structure):
    _fields_ = [("id", c_uint32),
                ("name", c_char_p)]


def main():
    server_addr = ('localhost', 38106)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        s.connect(server_addr)
        print("Connected to %s" % repr(server_addr))
    except:
        print("ERROR: Connection to %s refused" % repr(server_addr))
        sys.exit(1)

    try:
        while True:
            payload_out = Payload(int(random.uniform(1, 10)),
                                  'hello'.encode('utf-8'))
            print("Sending id=%d, name=%s" % (payload_out.id,
                                              payload_out.name
                                              ))
            nsent = s.send(payload_out)
            # Alternative: s.sendall(...): coontinues to send data until either
            # all data has been sent or an error occurs. No return value.
            print("Sent %d bytes" % nsent)

            buff = s.recv(sizeof(Payload))
            payload_in = Payload.from_buffer_copy(buff)
            print("Received id=%d, name=%s" % (payload_in.id,
                                               payload_in.name))
            option = input('Do you wish to continue?')
            if option not in ['1']:
                break
    finally:
        print("Closing socket")
        s.close()


if __name__ == "__main__":
    main()
