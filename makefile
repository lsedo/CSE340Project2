all: test

test: test.o Lexer.o Grammar.o
	g++ -o test test.o Lexer.o Grammar.o

test.o: test.cpp
	g++ -c test.cpp

Lexer.o: Lexer.cpp
	g++ -c Lexer.cpp

Grammar.o: Grammar.cpp
	g++ -c Grammar.cpp

clean:
	rm -f test *.o

run: test
	./test