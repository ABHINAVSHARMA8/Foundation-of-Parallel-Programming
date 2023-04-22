EXE=  mat_mul #iterative_averaging

all:	$(EXE)

iterative_averaging:	iterative_averaging.cpp
	mpicxx -g -o $@ $^ -fopenmp
	mpirun -n 4 ./iterative_averaging 10000000 64 4

mat_mul:	mm_mpi.c
	mpicc -g -o $@ $^ 
	mpirun -n 4 ./mat_mul

clean:
	rm -r $(EXE)