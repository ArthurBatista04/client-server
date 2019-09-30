CXX=gcc


all: main

main: 
	$(CXX) serverCustomer.c socket.c -o serverCustomer.out
	@echo "SUCCESS ON COMPILING CUSTOMER SERVER!"
	$(CXX) serverCustomer.c socket.c -o serverCustomer.out
	@echo "SUCCESS ON COMPILING FOOD SERVER!"