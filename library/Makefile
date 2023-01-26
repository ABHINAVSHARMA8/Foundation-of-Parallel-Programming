CFLAGS= -std=c++11 -lpthread

default:	libstamp.so

libstamp.so:	stamp.cpp
	g++ $(CFLAGS) -shared -fPIC  -o libstamp.so  stamp.cpp

format:
	clang-format -i *.c
	clang-format -i *.cpp
clean:
	rm libstamp.so
