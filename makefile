all: euclid_server euclid_client
euclid_client: polynoms.o formulas_client.o euclid_client.o tarsky_client.o
	g++ polynoms.o tarsky_client.o formulas_client.o euclid_client.o -lgmp -lgmpxx -o euclid_client
euclid_server: polynoms.o formulas_server.o parser.o lexer.o tarsky_server.o euclid_server.o
	g++ polynoms.o lexer.o parser.o tarsky_server.o formulas_server.o euclid_server.o -lgmp -lgmpxx -o euclid_server
parser.o: parser.cpp
	g++ -c parser.cpp -o parser.o
lexer.o: lexer.cpp
	g++ -c lexer.cpp -o lexer.o
formulas_server.o: formulas_server.cpp
	g++ -c formulas_server.cpp -o formulas_server.o
formulas_client.o: formulas_client.cpp
	g++ -c formulas_client.cpp -o formulas_client.o
polynoms.o: polynoms.cpp
	g++ -c polynoms.cpp -o polynoms.o
tarsky_server.o: tarsky_server.cpp
	g++ -c tarsky_server.cpp -o tarsky_server.o
tarsky_client.o: tarsky_client.cpp
	g++ -c tarsky_client.cpp -o tarsky_client.o
clean:
	rm -rf serverclient. *.o
