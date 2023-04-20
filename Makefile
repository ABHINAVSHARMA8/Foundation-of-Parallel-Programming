EXE=iterative_averaging

all:	$(EXE)

iterative_averaging:	iterative_averaging.cpp
	mpicxx -g -o $@ $^ -fopenmp
	mpirun -np 4 iterative_averaging

clean:
	rm -r $(EXE)