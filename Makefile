

all:
	g++ -std=c++11 -c quill-runtime.cpp
	g++ -std=c++11 -c nqueens.cpp
	g++ -std=c++11 quill-runtime.o nqueens.o -o nqueens -lpthread
	
format:
	clang-format -i *.c
	clang-format -i *.cpp

clean:
	rm nqueens
	rm quill-runtime.o
	rm nqueens.o
	rm test.o
	rm test

