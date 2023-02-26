#include "quill-runtime.h"

namespace quill{

void init_runtime(){
     
    char *streamsStr = getenv("QUILL_WORKERS");
    if (streamsStr == NULL) {
        fprintf(stderr, "QUILL_WORKERS WORKERS NONE\n");
    } else {
        workers= atoi(streamsStr);
    }
    if (workers <= 0)
        workers = 1;//defualt case

    deques=(Deque_t *)(malloc(sizeof(Deque_t)*workers));
    params=(param_t *)(malloc(sizeof(param_t)*(workers-1)));
    threads=(pthread_t *)(malloc(sizeof(pthread_t)*(workers-1)));
    for(int i=0;i<workers;i++) 
        deque_init(i);
    
    for(int i=1;i<workers;i++)
        params[i]={i};
    
    pthread_key_create(&key,NULL);
    pthread_mutex_init(&lock,NULL);

    for(int i=1;i<workers;i++) 
        pthread_create(&threads[i],NULL,worker_routine,(void *)&params[i]);
    
}


void start_finish(){
    task_counter=0;   
}

void async(Task &&lambda){
   //push async task to the worker's deque
    int rank=thread_id(); //per thread id
    pthread_mutex_lock(&lock);
    task_counter+=1;
    pthread_mutex_unlock(&lock);
    push_to_tail(lambda,rank);      
}

void end_finish(){

    int id=thread_id();
     
    while(task_counter!=0){
        Task t=find_and_execute_task(id);//either pop from tail or steal
         
        if(t!=NULL){
            t();//execute the task
            pthread_mutex_lock(&lock);
            task_counter-=1;
            pthread_mutex_unlock(&lock);
        }

    }
}

void finalize_runtime(){

    shutdown=1;
    for(int i=1;i<workers;i++) pthread_join(threads[i],NULL);
    for(int i=0;i<workers;i++) deque_destroy(i);
    
    pthread_mutex_destroy(&lock);
    pthread_key_delete(key);
    free(deques);
    free(threads);
    free(params);  
}

void *worker_routine(void *args){

    pthread_setspecific(key,args);//set thread id,default is 0 for main thread
    int id=((param_t *)args)->id;
    
    while(!shutdown){
        Task t=find_and_execute_task(id);//pop from tail or steal

        if(t!=NULL){
            t();//execute task
            pthread_mutex_lock(&lock);
            task_counter-=1;
            pthread_mutex_unlock(&lock);
        }
    }

    return nullptr;
}

Task find_and_execute_task(int rank){

    Task t=(pop_from_tail(rank));//lockless
   
    if(t!=NULL)
        return t;
        
    else{
        
        int victim=rand()%workers;//select a random victim
        Task t=pop_from_head(victim);//steal-syncchronised
       
        return t;//NULL if victim does not have a task i.e failed steal
    }
}

bool isFull(int rank){
    
    bool ans= ((deques[rank].head == 0 && deques[rank].tail == deque_size - 1) || deques[rank].head == deques[rank].tail + 1);
    return ans;
}

bool isEmpty(int rank){
  
    bool ans= (deques[rank].head==-1);
    return ans;
}

void deque_init(int i){

    pthread_mutex_init(&(deques[i].lock), 0);
    deques[i].head=-1;
    deques[i].tail=0;
    
}

void deque_destroy(int rank){
    pthread_mutex_destroy(&deques[rank].lock);
}
    
    
    

void push_to_tail(Task p,int rank){
    
    try{
        //check if maximum limit of tasks has reached or not
        if(isFull(rank)) {
            throw -1;
        
        }
    }

    catch(..){
        std::cout<<"Deque overflow"<<std::endl;
        exit(0);
    }
    
    //if maximum limit of tasks has not reached
    if(isEmpty(rank)){
        //first task
        deques[rank].head=0;
        deques[rank].tail=0;   
    }

    else if(deques[rank].tail==deque_size-1) deques[rank].tail=0;//circular array

    else
        deques[rank].tail+=1;
    
    deques[rank].queue[deques[rank].tail]=p;//insert task
      
}

Task pop_from_head(int rank){
   
    pthread_mutex_lock(&deques[rank].lock);
    
    if(isEmpty(rank)){
      pthread_mutex_unlock(&deques[rank].lock);
      return NULL;
    }
    
    Task t=deques[rank].queue[deques[rank].head];//get task

    if(deques[rank].head==deques[rank].tail){
        //single task
        deques[rank].head=-1;
        deques[rank].tail=-1;
    }

    else if(deques[rank].head>=deque_size-1)//circular array
        deques[rank].head=0;

    else
        deques[rank].head=deques[rank].head+1;

    pthread_mutex_unlock(&deques[rank].lock);
     
    return t;
   
}

Task pop_from_tail(int rank){
   
    if(isEmpty(rank)) 
      return NULL;
    
  
   if(deques[rank].head==deques[rank].tail){
        //synchronisation for single task
        pthread_mutex_lock(&deques[rank].lock);
        Task t=deques[rank].queue[deques[rank].tail];
        deques[rank].head=-1;
        deques[rank].tail=-1;
        pthread_mutex_unlock(&deques[rank].lock);
        return t;
    }

   
    Task t=deques[rank].queue[deques[rank].tail];//get task
    
    if(deques[rank].tail==0)
        deques[rank].tail=deque_size-1;
    
    else
        deques[rank].tail=deques[rank].tail-1;

    return t;

}

int thread_id(){

    void *p=pthread_getspecific(key);
    if(p==NULL) {return 0;}//defualt case for main thread
    
    return *((int *)p);
}

}





   
   