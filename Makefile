CXX=gcc


all: main

main: 
	$(CXX) serverCustomer.c socket.c -o serverCustomer.out
	@echo "SUCCESS ON COMPILING CUSTOMER SERVER!"
	$(CXX) serverFood.c socket.c -o serverFood.out
	@echo "SUCCESS ON COMPILING FOOD SERVER!"
	$(CXX) -pthread serverDrink.c socket.c -o serverDrink.out
	@echo "SUCCESS ON COMPILING DRINK SERVER!"
	$(CXX) proxy.c socket.c -o proxy.out
	@echo "SUCCESS ON COMPILING PROXY!"