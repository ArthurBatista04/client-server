CXX=gcc


all: main

main: 
	$(CXX) serverCustomer.c socket.c -o serverCustomer.out
	@echo "SUCCESS ON COMPILING CUSTOMER SERVER!"
	$(CXX) serverCustomer.c socket.c -o serverCustomer.out
	@echo "SUCCESS ON COMPILING FOOD SERVER!"
	$(CXX) -pthread serverDrink.c socket.c 
	@echo "SUCCESS ON COMPILING DRINK SERVER!"