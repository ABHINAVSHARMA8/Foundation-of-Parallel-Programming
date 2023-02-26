#ifndef QUILL_RUNTIME_H
#define QUILL_RUNTIME_H

#include "quill.h"
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
#include<functional>


namespace quill{

#define deque_size 10000
typedef std::function<void()>  Task;

typedef struct{
  int id;
} param_t;

struct Deque_t{
    //public:
    int id;
    pthread_mutex_t lock;
    int  head;
    int tail;
    Task queue[deque_size];
  
};




static int workers=0;

/*std::vector<Deque_t> deques;
std::vector<param_t> params;
std::vector<pthread_t> threads;
*/
static Deque_t *deques=NULL;
static param_t *params=NULL;
static pthread_t *threads=NULL;

static pthread_key_t key;
volatile int shutdown=0;
volatile int task_counter=0;

static pthread_mutex_t lock;





//void param_init(param_t*,int );
void deque_init(int );
void deque_destroy(int );
void *worker_routine(void*);
Task find_and_execute_task( int);
bool isFull(int );
bool isEmpty( int);
void push_to_tail(Task ,int );
Task pop_from_tail(int );
Task pop_from_head(int );
int thread_id();

}

#endif