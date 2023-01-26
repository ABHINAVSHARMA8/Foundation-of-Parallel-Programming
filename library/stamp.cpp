#include "stamp.h"
using namespace std;

void execute_tuple(function<void()> &&lambda1,function<void()> &&lambda2){

    pthread_t t1;
    //cout<<lambda1<<endl;
   //int status=pthread_create(&t1,NULL,lambda1,this);

   //cout<<status<<endl;
   cout<<"stamp.cpp"<<endl;
}

void parallel_for(int low,int high,int stride,std::function<void(int)> &&lambda,int numThreads){}

void parallel_for(int high,std::function<void(int)> &&lambda,int numThreads){}

void parallel_for(int low1,int high1,int stride1,int low2,int high2,int stride2,std::function<void(int,int)> &&lambda,int numThreads){}

void parallel_for(int high1,int high2,std::function<void(int,int)> &&lambda,int numThreads){}