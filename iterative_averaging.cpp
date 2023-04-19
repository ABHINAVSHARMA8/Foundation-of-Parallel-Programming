#include<iostream>
#include<algorithm>
#include<stdio.h>
#include<string.h>
#include<cmath>
#include<sys/time.h>
#include <cassert>
#include<mpi.h>
using namespace std;

/*
 * Ported from HJlib
 *
 * Author: Vivek Kumar
 *
 */

//48 * 256 * 2048
int n,ni,p,threads;
double *a, *shadow;


long get_usecs () {
  struct timeval t;
  gettimeofday(&t,NULL);
  return t.tv_sec*1000000+t.tv_usec;
}
 
int ceilDiv(int d) {
  int m = n / d;
  if (m * d == n) 
    return m;
   else 
    return (m + 1);
  
}


void recurse(uint64_t low, uint64_t high) {
  if((high - low) > 512) {
    //THRESHOLD
    uint64_t mid = (high+low)/2;
    /* An async task */ recurse(low, mid);  
    recurse(mid, high);
  } else {
    for(uint64_t j=low; j<high; j++) {
      a[j] = (shadow[j - 1] + shadow[j + 1]) / 2.0;
    }
  }
}

void runParallel(int x) {
  for(int i=0; i<ni; i++) {
    recurse(1, x+1);
    double* temp = a;
    a = shadow;
    shadow = temp;
  }
}

int main(int argc, char** argv) {
  n=atoi(argv[1]);
  ni=atoi(argv[2]);
  p=atoi(argv[3]);
  if(p==2) threads=2;
  else threads=1;

  int rank, np;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &np);

  int start=(n/np)*rank,end=start + (n/np) -1;
  if(rank==np-1) end=n-1;


  a = new double[(n + 2)];
  shadow = new double[(n + 2)];
  memset(a, 0, sizeof(double) * (n + 2));
  memset(shadow, 0, sizeof(double) * (n + 2));
  shadow[n + 1] = 1.0;
  long start = get_usecs();
  runParallel();
  long end = get_usecs();
  
  for(int i=2;i<n+1;i++) assert(a[i]==a[i-1]);
  
  
  double dur = ((double)(end-start))/1000000;
  printf("Time = %.3f\n",dur);
  delete(a);
  delete(shadow);
}
