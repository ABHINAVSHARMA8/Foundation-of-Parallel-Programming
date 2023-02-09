#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<assert.h>

typedef std::Function<void()> Task;

static int workers=0;
static int deque_size=0;
static deque_t *deques=NULL;
static pthread_t *threads=NULL;
static pthread_key_t key;
volatile int shutdown=0;
volatile int task_counter=0;
static pthread_mutex_t lock;



struct deque_t {
  pthread_mutex_t lock;
  int  head=-1;
  int tail=-1;
  Task queue[deque_size];
  size_t task_count;  // No of tasks produced by this pool
  size_t task_stolen; // No of tasks stolen from this pool
};

void deque_init(deque_t pool);
void *key_init(void *args);
void *worker_routine(void *args);
