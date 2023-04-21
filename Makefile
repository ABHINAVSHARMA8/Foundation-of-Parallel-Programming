EXE=iterative_averaging mat_mul

all:	$(EXE)

iterative_averaging:	iterative_averaging.cpp
	mpicxx -g -o $@ $^ -fopenmp
	mpirun -np 4 iterative_averaging

mat_mul:	mat_mul.c
	mpicc -g -o $@ $^ 
	mpirun -np 4 mat_mul

clean:
	rm -r $(EXE)