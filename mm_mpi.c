#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include<mpi.h>
#include <assert.h>
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

int malloc2d(double ***array, int n) {

    /* allocate the n*m contiguous items */
    double *p = (double *)malloc(n*n*sizeof(double));
    if (!p) return -1;
    /* allocate the row pointers into the memory */
    (*array) = (double **)malloc(n*sizeof(double*));
    if (!(*array)) {
       free(p);
       return -1;
    }
    /* set up the pointers into the contiguous memory */
    for (int i=0; i<n; i++) 
       (*array)[i] = &(p[i*n]);
    return 0;
}

int free2d(double ***array) {
    /* free the memory - the first element of the array is at the start */
    free(&((*array)[0][0]));
    /* free the pointers into the memory */
    free(*array);
    return 0;
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
  //perform chunking
  int start=(n/np)*id;
  int end=start + (n/np);
  if(id==np-1) end=n;
  for(int i=start; i<end; i++) { //parallel w.r.t rows in the outer loop
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
  
  
  
  //all processes declare matrices
  malloc2d(&a,n);
  malloc2d(&b,n);
  malloc2d(&c,n);
  
  if(id==0){
    //master process initialises matrices
    for(int i=0; i<n; i++) {
      for(int j=0; j<n; j++) {
        a[i][j] = 1.0;  
        b[i][j] = 1.0;  
        c[i][j] = 0;
      }    
    }
  }

  
  //send matrices to all non root processes
  MPI_Bcast(&a[0][0],n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Bcast(&b[0][0],n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Bcast(&c[0][0],n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
  
  //start timer
  long start = get_usecs();
  //multiply matrices
  multiply(n);
  //end timer
  long end = get_usecs();
  double dur = ((double)(end-start))/1000000;
  if(id>0){
    //send local computation to root process
    MPI_Send(&c[0][0],n*n,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
  }
  else{
    MPI_Status stats;
    double **temp;
    malloc2d(&temp,n);
    
    for(int i=1;i<np;i++){
      MPI_Recv(&temp[0][0],n*n,MPI_DOUBLE,i,1,MPI_COMM_WORLD,&stats);
      //take local computation from non-root processes
      for(int j=0;j<n;j++){
          for(int k=0;k<n;k++){
              if(temp[j][k]!=0){
              
                  c[j][k]=temp[j][k];
              }
          }
      }
    }
    //validate result
    int result = verify(n);
    printf("MatrixMultiplication result = %d, Time = %.3f\n",result, dur);
  }
  //release memory
  free2d(&a);
  free2d(&b);
  free2d(&c);
  
  MPI_Finalize();
  return 0;
}
