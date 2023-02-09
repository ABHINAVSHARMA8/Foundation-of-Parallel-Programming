SRCCPP=$(wildcard *.cpp)
CPPTARGETS=$(patsubst %.cpp,%,$(SRCCPP))
default:	$(CPPTARGETS)

%: %.cpp
	export DY_LD_LIBRARY=.
	g++ -std=c++11 -lpthread -I.   -o $*.out 

format:
	clang-format -i *.c
	clang-format -i *.cpp

clean:
	rm *.out

