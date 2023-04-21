#include<iostream>
#include<algorithm>
#include<stdio.h>
#include<string.h>
#include<cmath>
#include<sys/time.h>
#include <cassert>
#include<mpi.h>
#include<omp.h>
using namespace std;

/*
 * Ported from HJlib
 *
 * Author: Vivek Kumar
 *
 */

//48 * 256 * 2048
int n,ni,threads,id, np;
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
      MPI_Status stats;
      if(id!=0)
        MPI_Send(&shadow[low],1,MPI_DOUBLE,id-1,1,MPI_COMM_WORLD);
      if(id!=np-1)
        MPI_Send(&shadow[high-1],1,MPI_DOUBLE,id+1,1,MPI_COMM_WORLD);

      int first=0,second=1;
      if(id!=0)
        MPI_Recv(&first,1,MPI_DOUBLE,id-1,1,MPI_COMM_WORLD,&stats);//shadow[low-1]
      if(id!=np-1)
         MPI_Recv(&second,1,MPI_DOUBLE,id+1,1,MPI_COMM_WORLD,&stats);//shadow[high]
      
      a[high-1]=(shadow[high-2]+second)/2.0;
      
      a[low]=(first+shadow[low+1])/2.0;
      #pragma parallel for num_threads(threads) default(none) shared(a,shadow) firsprivate(low,high)
      for(uint64_t j=low+1; j<high-1; j++) {
        a[j] = (shadow[j - 1] + shadow[j + 1]) / 2.0;
      }


      
  }
}

void runParallel(int start,int end) {
  for(int i=0; i<ni; i++) {
    recurse(start,end);
    double* temp = a;
    a = shadow;
    shadow = temp;
  }
}

int main(int argc, char** argv) {
  n=atoi(argv[1]);
  ni=atoi(argv[2]);
  

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  cout<<id<<" "<<np<<endl;
  if(np==2) threads=2;
  else threads=1;

  int start=(n/np)*id,end=start + (n/np);
  if(id==np-1) end=n+1;
  if(id==0) start=1;


  a = new double[(n + 2)];
  shadow = new double[(n + 2)];
  memset(a, 0, sizeof(double) * (n + 2));
  memset(shadow, 0, sizeof(double) * (n + 2));
  shadow[n + 1] = 1.0;
  long s = get_usecs();
  runParallel(start,end);
  long e = get_usecs();
  int sum=0;
  
  for(int i=start;i<end;i++) sum+=a[i];
  MPI_Status stats;
  if(id>0){
    MPI_Send(&sum,1,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
  }

  if(id==0){
    int temp;
    for(int i=1;i<np;i++){
      MPI_Recv(&temp,1,MPI_DOUBLE,i,1,MPI_COMM_WORLD,&stats);
      sum+=temp;
    }
    cout<<"Sum is "<<sum<<endl;
  }
  
  //for(int i=2;i<n+1;i++) assert(a[i]==a[i-1]);
  
  
  double dur = ((double)(e-s))/1000000;
  printf("Time = %.3f\n",dur);
  delete(a);
  delete(shadow);
}
