SRCCPP=$(wildcard *.cpp)
CPPTARGETS=$(patsubst %.cpp,%,$(SRCCPP))
default:	$(CPPTARGETS)

%: %.cpp
	g++ -std=c++11 -I${STAMP_SHARED}/library -L${STAMP_SHARED}/library  -o $*.out $< -lstamp

format:
	clang-format -i *.c
	clang-format -i *.cpp

clean:
	rm *.out

