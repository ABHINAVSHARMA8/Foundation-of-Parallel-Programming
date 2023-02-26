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

namespace quill{

#define deque_size 10000//size of each deque:compile time constant
typedef std::function<void()>  Task;

typedef struct{
  // to set thread id
  int id;
} param_t;

struct Deque_t{ 
  //Deque data structure
  pthread_mutex_t lock; //lock for head
  int head; //head pointer
  int tail; //tail pointer
  Task queue[deque_size]; //array to store tasks
};

static int workers=0; // number of workers
static Deque_t *deques=NULL; //to store deques
static param_t *params=NULL; //to store para,s
static pthread_t *threads=NULL; //to store pthread_t
static pthread_key_t key; 
volatile int shutdown=0; //shutdown variabble
volatile int task_counter=0;//count async tasks
static pthread_mutex_t lock;//lock for task_counter

void deque_init(int);//initialise deques
void deque_destroy(int);//destroy deques
void *worker_routine(void*);//executed by pthread_create()
Task find_and_execute_task(int);//pop from tail or steal
bool isFull(int);//check if deque is full
bool isEmpty(int);//check of deque is emppty
void push_to_tail(Task,int);//lockles
Task pop_from_tail(int);//lockless
Task pop_from_head(int);//synchronised
int thread_id();// get per thread id

}

#endif