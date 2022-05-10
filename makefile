all: euclid_server euclid_client euclid
euclid_client: polynoms.o formulas.o euclid_client.o parser.o lexer.o tarsky.o
	g++ polynoms.o lexer.o parser.o tarsky.o formulas.o euclid_client.o -lgmp -lgmpxx -o euclid_client
euclid_server: polynoms.o formulas.o parser.o lexer.o tarsky.o euclid_server.o
	g++ polynoms.o lexer.o parser.o tarsky.o formulas.o euclid_server.o -lgmp -lgmpxx -o euclid_server
euclid: polynoms.o formulas.o parser.o lexer.o tarsky.o euclid.o
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
	rm -rf euclid. *.o
