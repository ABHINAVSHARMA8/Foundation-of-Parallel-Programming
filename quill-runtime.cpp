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
}

void deque_init(deque_t pool){

    pthread_mutex_init(&(pool->lock), 0);
}