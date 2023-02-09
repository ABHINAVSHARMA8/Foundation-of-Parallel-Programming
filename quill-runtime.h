#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef std::Function<void()> Task;

static int workers=0;
static int deque_size=0;
static deque_t *deques=NULL;
volatile int shutdown=0;



struct deque_t {
  pthread_mutex_t lock;
  int  head=-1;
  int tail=-1;
  Task queue[deque_size];
  size_t task_count;  // No of tasks produced by this pool
  size_t task_stolen; // No of tasks stolen from this pool
};

void deque_init(deque_t pool);
