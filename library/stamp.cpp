#include "stamp.h"

static void* thread_start(void* args) {
    // Get the actual function
    std::function<void()>* f_ptr = static_cast<std::function<void()>*>(args);
    // Run the actual function
    (*f_ptr)();
    // Delete the function from the heap
    delete f_ptr;
    // Return nullptr, since we're not dinking around with futures
    return nullptr;
}



 

void execute_tuple(std::function<void()> &&lambda1,std::function<void()> &&lambda2){

    
   pthread_t t1,t2;
    std::function<void()>* f_no_args = new std::function<void()>;
    
    *f_no_args = std::bind(lambda1);

    std::function<void()>* f2 = new std::function<void()>;
    
    *f2 = std::bind(lambda2);
   pthread_create(&t1,NULL,thread_start,f_no_args);
   pthread_create(&t2,NULL,thread_start,f2);

   pthread_join(t1,NULL);
   pthread_join(t2,NULL);

   //lambda1();lambda2(); WORKING
}

void parallel_for(int low,int high,int stride,std::function<void(int)> &&lambda,int numThreads){

   /* int num_i=(high-low)/stride;
    numThreads=min(numThreads,num_i);
    int sequential=num_i-numThreads;

    pthread_t t[numThreads];

    int i=low;
    while(numThreads>0){

        
        pthread_create(&t[numThreads],NULL,lambda inside for loop)
        numThreads--;
    }
    */
}

void parallel_for(int high,std::function<void(int)> &&lambda,int numThreads){}

void parallel_for(int low1,int high1,int stride1,int low2,int high2,int stride2,std::function<void(int,int)> &&lambda,int numThreads){}

void parallel_for(int high1,int high2,std::function<void(int,int)> &&lambda,int numThreads){}