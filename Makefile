SRCCPP=$(wildcard *.cpp)
CPPTARGETS=$(patsubst %.cpp,%,$(SRCCPP))
default:	$(CPPTARGETS)

%: %.cpp
	g++ -std=c++11 -pthread -I.   -o $*.out 

format:
	clang-format -i *.c
	clang-format -i *.cpp

clean:
	rm *.out

