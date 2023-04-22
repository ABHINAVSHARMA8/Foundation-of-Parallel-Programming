/*
 * Ported from HJlib for CSE 502 course at IIITD
 * Author: Vivek Kumar
 */


#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include<mpi.h>

double **a, **b, **c;
int id,np;

/*
 * Timer routine
 */
long get_usecs (void) {
   struct timeval t;
   gettimeofday(&t,NULL);
   return t.tv_sec*1000000+t.tv_usec;
}

/*
 * Intialize input matrices and the output matrix
 */

/*
 * release memory
 */
void freeall(int n) {
  /*
  for(int i=0; i<n; i++) {
    delete(a[i]);
    delete(b[i]);
    delete(c[i]);
  }
  */
  free(a);
  free(b);
  free(c);
}

/*
 * Validate the result of matrix multiplication
 */
int verify(int n) {
  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++) {
      if(c[i][j] != n) {
        printf("result = %.3f\n",c[i][j]);
        return 0;
      }
    }
  }
  return 1;
}

void multiply(int n) {
  int start=(n/np)*id;
  int end=start + (n/np);
  if(id==np-1) end=n;
  for(int i=start; i<end; i++) {
    for(int j=0; j<n; j++) {
      for(int k=0; k<n; k++) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }    
  }
}

int main(int argc, char** argv) {
  int n = argc>1 ? atoi(argv[1]) : 1024;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  // initialize
  //init(n);
  a[n][n];
  b[n][n];
  c[n][n];
  /*
  for(int i=0; i<n; i++) {
    a[i] = new double[n];
    b[i] = new double[n];
    c[i] = new double[n];
  }
  */
  if(id==0){
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            a[i][j] = 1.0;  
            b[i][j] = 1.0;  
            c[i][j] = 0;
        }    
    }

    MPI_Bcast(&a,n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(&b,n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(&c,n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
  }

  else{
    MPI_Bcast(&a,n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(&b,n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(&c,n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);

  }
    
  //start timer
  long start = get_usecs();
  //multiply matrices
  multiply(n);
  //end timer
  long end = get_usecs();
  double dur = ((double)(end-start))/1000000;
  if(id>0){
    
    MPI_Send(&c,n*n,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
  }
  else{
    MPI_Status stats;
    double temp[n][n];
    for(int i=1;i<np;i++){
        MPI_Recv(&temp,n*n,MPI_DOUBLE,i,1,MPI_COMM_WORLD,&stats);
        for(int j=0;j<n;j++){
            for(int k=0;j<n;k++){
                if(temp[j][k]!=0){
                    c[j][k]=temp[j][k];
                }
            }
        }
    }
    

  }
  //validate result
  int result = verify(n);
  
  printf("MatrixMultiplication result = %d, Time = %.3f\n",result, dur);
  //release memory
  freeall(n);
  return 0;
}
