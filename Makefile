CXX=gcc


all: C

C: main
	@rm -f *.out
	@rm -f */*.out

main: 
	$(CXX) serverCustomer.c socket.c -o serverCustomer.out
	@echo "Sucesso na compilação!"
