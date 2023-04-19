EXE=iterative_averaging

all:	$(EXE)

iterative_averaging:	iterative_averaging.cpp
	g++ -std=c++11 -o iterative_averaging iterative_averaging.cpp -fopenmp

clean:
	rm -r $(EXE)