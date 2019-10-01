# Client/server

This is a simple project on using sockets for interprocess communication. There are three C linux servers available for usage and you can connect to these servers through N clients.

```
Customer server
Food server
Drink server
```

Each server uses a different technique to handle passing information from one process to another and contains a unique database for create and read operations. The customer server uses FIFO, the next one PIPE and lastly shared memory with POSIX semaphore.

## Getting Started

To use these servers follow the instructions below.

### Prerequisites

For the proper functioning of this code it is necessary to use a GNU / Unix system, in addition the following programs must be installed.

```
Python 2.7
GCC
```

### Usage

To run the servers you must first compile all of them using make.

```
make
```

Then, open a different terminal for each command line:

```
./serverCustomer.out
./serverFood.out
./serverDrink.out
```

Finally, for each client open a new terminal and run:

```
python2.7 client.py
```

#### References

- [SOCKET](https://www.geeksforgeeks.org/socket-programming-cc/)
- [FIFO](https://www.geeksforgeeks.org/named-pipe-fifo-example-c-program/)
- [PIPE](https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/)
- [Shared memory](https://www.geeksforgeeks.org/ipc-shared-memory/)
- [Posix semaphore](https://www.geeksforgeeks.org/use-posix-semaphores-c/)
