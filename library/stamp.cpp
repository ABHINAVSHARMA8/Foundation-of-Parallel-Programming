#include "stamp.h"
using namespace std;
static void* et_thread(void* args) {
   
    function<void()> f_ptr = ((et *)args)->fptr;
    f_ptr();
    
    return nullptr;
}

static void* pf1_thread(void* args) {
   //For 1d parallel_for function
    function<void(int)> f_ptr = ((parallel_for_1_t *)args)->fptr;
    int  x=((parallel_for_1_t *)args)->start;
    int  y=((parallel_for_1_t *)args)->end;
   
    for(int i=x;i<y;i++)
        f_ptr(a[i]);
        
    return nullptr;
}

static void* pf2_thread(void* args) {
   //For 1d parallel_for function
    function<void(int,int)> f_ptr = ((parallel_for_2_t *)args)->fptr;
    int  x=((parallel_for_2_t *)args)->count;
    int start=((parallel_for_2_t *)args)->ptr;
    
   
    

    while(x-->0){
        f_ptr(b[start].first,b[start].second);
        start++;
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

   
}

void parallel_for(int low,int high,int stride,std::function<void(int)> &&lambda,int numThreads){

    int num_i=(high-low)/stride;
    if(numThreads>num_i) numThreads=num_i;
   
    
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
    int iter=0;
     
    while(iter<numThreads){
      
        child[iter]={lambda,start[iter],end[iter]};
       
        pthread_create(&t[iter],NULL,pf1_thread,(void*)&child[iter]);
        
        iter++;
    }

    for(int x=0;x<numThreads;x++) pthread_join(t[x],NULL);

    
    
    a.clear();
    
}

void parallel_for(int high,std::function<void(int)> &&lambda,int numThreads){

    parallel_for(0,high,1,[&](int i){lambda(i);},numThreads);
}

void parallel_for(int low1,int high1,int stride1,int low2,int high2,int stride2,std::function<void(int,int)> &&lambda,int numThreads){

    int num_i=(high1-low1)/stride1;
    int num_j=(high2-low2)/stride2;

    if(numThreads>num_i*num_j) numThreads=num_i*num_j;

    for(int i=low1;i<high1;i+=stride1) {
        for(int j=low2;j<high2;j+=stride2)
            b.push_back({i,j});
    }

    pthread_t t[numThreads];
    parallel_for_2_t child[numThreads];
    int ptr=0;
        

    for(int i=0;i<numThreads;i++){

        if(i==numThreads-1){

            int x=(b.size()-(b.size()/numThreads)*(numThreads-1));
            child[i]={lambda,x,ptr};
            pthread_create(&t[i],NULL,pf2_thread,(void*)&child[i]);

        }

        else{

            int x=b.size()/numThreads;
            child[i]={lambda,x,ptr};
            pthread_create(&t[i],NULL,pf2_thread,(void*)&child[i]);
            ptr+=x;
        }
    }

    for(int x=0;x<numThreads;x++) pthread_join(t[x],NULL);

    
    
    b.clear();


}

void parallel_for(int high1,int high2,std::function<void(int,int)> &&lambda,int numThreads){

    parallel_for(0,high1,1,0,high2,1,[&](int i,int j){lambda(i,j);},numThreads);
}