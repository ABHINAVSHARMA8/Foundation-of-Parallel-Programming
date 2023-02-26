GCC=g++
CFLAGS=-std=c++11
LFLAGS=-lpthread

all:	quill-runtime.o nqueens.o
	$(GCC) $(CFLAGS) quill-runtime.o nqueens.o -o nqueens $(LFLAGS)

quill-runtime.o:
	$(GCC) $(CFLAGS)  -c quill-runtime.cpp

nqueens.o:
	$(GCC) $(CFLAGS) -c nqueens.cpp

format:
	clang-format -i *.c
	clang-format -i *.cpp

clean:
	rm nqueens
	rm *.o
	
	

