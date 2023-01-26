default:	libstamp.so

libstamp.so:	stamp.cpp
	g++ -std=c++11 -shared -fPIC  -o libstamp.so  stamp.cpp

format:
	clang-format -i *.c
	clang-format -i *.cpp
clean:
	rm libstamp.so
