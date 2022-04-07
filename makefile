all: euclid
euclid: polynoms.o formulas.o euclid.o parser.o lexer.o tarsky.o
	g++ polynoms.o lexer.o parser.o tarsky.o formulas.o euclid.o -lgmp -lgmpxx -o euclid
parser.o: parser.cpp
	g++ -c parser.cpp -o parser.o
lexer.o: lexer.cpp
	g++ -c lexer.cpp -o lexer.o
formulas.o: formulas.cpp
	g++ -c formulas.cpp -o formulas.o
polynoms.o: polynoms.cpp
	g++ -c polynoms.cpp -o polynoms.o
tarsky.o: tarsky.cpp
	g++ -c tarsky.cpp -o tarsky.o
clean:
	rm -rf euclid1. *.o
