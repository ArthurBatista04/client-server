CXX=gcc


all: C

C: main
	@rm -f *.out
	@rm -f */*.out

main: 
	$(CXX) serverCustomer.c socket.c -o serverCustomer.out
	@echo "SUCCESS ON COMPILING CUSTOMER SERVER!"
