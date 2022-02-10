all: euclid
euclid: euclid.o lexer.o
	g++ lexer.o euclid.o -o euclid
tests.o: euclid.cpp
	g++ -c euclid.cpp -o euclid.o
stack.o: stack.cpp
	g++ -c lexer.cpp -o lexer.o
clean:
	rm -rf euclid. *.o

