all: euclid
euclid: euclid.o parser.o lexer.o token.o
	g++ lexer.o parser.o euclid.o token.o -o euclid
euclid.o: euclid.cpp
	g++ -c euclid.cpp -o euclid.o
parser.o: parser.cpp
	g++ -c parser.cpp -o parser.o
lexer.o: lexer.cpp
	g++ -c lexer.cpp -o lexer.o
token.o: token.cpp
	g++ -c token.cpp -o token.o
clean:
	rm -rf euclid. *.o

