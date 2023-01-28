#include "stamp.h"
using namespace std;
static void* et_thread(void* args) {
   //For execute tuple function
    function<void()> f_ptr = ((et *)args)->fptr;
    f_ptr();
    
    return nullptr;
}

static void* pf1_thread(void* args) {
   //For 1d parallel_for function
    function<void(int)> f_ptr = ((parallel_for_1_t *)args)->fptr;
    int  x=((parallel_for_1_t *)args)->start;
    int  y=((parallel_for_1_t *)args)->end;
   // cout<<x<<" "<<y<<" pf1"<<endl;
     
    for(int i=x;i<y;i++){
        
       
        f_ptr(a[i]);
        
    }
    
    
    return nullptr;
}


 

void execute_tuple(std::function<void()> &&lambda1,std::function<void()> &&lambda2){

    
    pthread_t t1,t2;
    et child1={lambda1};
    et child2={lambda2};

    

   pthread_create(&t1,NULL,et_thread,(void*)&child1);
   pthread_create(&t2,NULL,et_thread,(void*)&child2);

   pthread_join(t1,NULL);
   pthread_join(t2,NULL);

   //lambda1();lambda2(); WORKING
}

void parallel_for(int low,int high,int stride,std::function<void(int)> &&lambda,int numThreads){

    int num_i=(high-low)/stride;
    if(numThreads>num_i) numThreads=num_i;
   //std::cout<<"Stride is "<<stride<<std::endl;
   

    int iter=0;
    for(int i=low;i<high;i+=stride) a.push_back(i);

    vector<int> start,end;

    for(int i=0;i<numThreads;i++){

       
            int x=num_i/numThreads;
          
            if(start.size()==0){
                start.push_back(0);
                end.push_back(x);
            }

            else{
                if(i==numThreads-1){

                    start.push_back(end[end.size()-1]);
                    end.push_back(a.size());
                    break;
                }

                
                start.push_back(end[end.size()-1]);
                end.push_back(start[start.size()-1]+x);
            }
        
    }

    



    pthread_t t[numThreads];
    parallel_for_1_t child[numThreads];
     
    
    while(iter<numThreads){
      
         child[iter]={lambda,start[iter],end[iter]};
       
        pthread_create(&t[iter],NULL,pf1_thread,(void*)&child[iter]);
        
    
        iter++;
    }

    for(int x=0;x<numThreads;x++) pthread_join(t[x],NULL);

    
    
    a.clear();
    
}

void parallel_for(int high,std::function<void(int)> &&lambda,int numThreads){}

void parallel_for(int low1,int high1,int stride1,int low2,int high2,int stride2,std::function<void(int,int)> &&lambda,int numThreads){}

void parallel_for(int high1,int high2,std::function<void(int,int)> &&lambda,int numThreads){}