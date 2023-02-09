#include "quill.h"
#include "quill-runtime.h"

void init_runtime(){

    char *streamsStr = getenv("QUILL_WORKERS"),*sizeStr= getenv("DEQUE_SIZE");

    if (streamsStr == NULL) {
        fprintf(stderr, "QUILL_WORKERS WORKERS NONE\n");
    } else {
        workers= atoi(streamsStr);
    }
    if (workers <= 0)
        workers = 1;

    if (sizeStr == NULL) {
        fprintf(stderr, "DEQUE_SIZE NONE\n");
    } else {
        deque_size= atoi(sizeStr);
    }
    if (deque_size <= 0)
        deque_size = 1;


    deques=(deque_t *)malloc(sizeof(deque_t) * workers);
    for(int i=0;i<workers;i++) deque_init(deques[i]);

    pthread_key_create(&key,NULL);
    pthread_muex_init(&lock,NULL);

    threads=(pthread_t *)malloc(sizeof(pthread_t) * workers);
    //keys=(pthread_key_t *)malloc(sizeof(pthread_key_t) * workers);
    for(int i=0;i<workers;i++) pthread_create(&threads[i],NULL,key_init,(void *)&i);
    for(int i=0;i<workers;i++) pthread_join(threads[i],NULL);

    for(int i=0;i<workers;i++) pthread_create(&threads[i],NULL,worker_routine,NULL);

}

void finalize_runtime(){

    for(int i=0;i<workers;i++) pthread_join(threads[i],NULL);
    for(int i=0;i<workers;i++)
        pthread_mutex_destory(&deques[i]->lock);
    pthread_mutex_destory(&lock);
    pthread_key_delete(key);
    free(deques);
    free(threads);
    
}

void *worker_routine(void *args){

    int id=(int *)pthread_getspecified(&key,NULL);

    while(!shutdown){

        if(size(deques[id])>0){
            T &&lambda=pop_from_tail(deques[id]);
            lambda();
        }
        else{

            int victim=rand()%workers;
            pthread_mutex_lock(&deques[victim]->lock,NULL);
            if(size(deques[victim])>1){
                T &&lambda=pop_from_head(deques[victim]);
            }
            pthread_mutex_unlock(&deques[victim]->lock,NULL);
        }

    }
}

void async(Task &&lambda){

    int rank=rand()%workers; //push to worker number:generated randomly //TODO:check
    pthread_mutex_lock(&lock);
    task_counter+=1;
    pthread_mutex_unlock(&lock);
    push_to_tail(lambda,deques[rank]); 

}

void end_finish(){

    while(task_counter!=0){

        int id=(int *)pthread_getspecified(&key,NULL);

        if(size(deques[id])>0){
            T &&lambda=pop_from_tail(deques[id]);
            lambda();
            pthread_mutex_lock(&lock);
            task_counter+=1;
            pthread_mutex_unlock(&lock);
        }
        else{

            int victim=rand()%workers;
            pthread_mutex_lock(&deques[victim]->lock,NULL);
            if(size(deques[victim])>1){
                T &&lambda=pop_from_head(deques[victim]);
                lambda();
                pthread_mutex_lock(&lock);
                task_counter+=1;
                pthread_mutex_unlock(&lock);
            }
            pthread_mutex_unlock(&deques[victim]->lock,NULL);
        }

        
    

    }

}

void deque_init(deque_t pool){
    //initialise the deque
    pthread_mutex_init(&(pool->lock), 0);
}

void *key_init(void *args){
    //set value i to thread i
    pthread_setspecific(&key,(int *)args);
}

void push_to_tail(Task &&lambda,deque_t deque){

    assert(size(deque)<deque_size);
    deque->tail+=1;
    deque->queue[deque->tail]=lambda;
}

Task pop_from_tail(deque_t deque){

    assert(size(deque)>0);
    Task &&lambda=deque->queue[deque->tail];
     deque->tail-=1;

}

Task pop_from_head(deque_t deque){
    //TODO
    assert(size(deque)>1);
    Task &&lambda=deque->queue[deque->tail];
     deque->tail-=1;

}
