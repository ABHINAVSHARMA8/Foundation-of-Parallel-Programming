#include<pthread.h>
#include <assert.h>
#include <stdio.h>
#include<functional>
#include<algorithm>
#include<iostream>
#include<vector>
#include<utility>
#include<time.h>

static std::vector<int> a;
static std::vector<std::pair<int,int>> b;

typedef struct{
    std::function<void()> fptr;
}et;

typedef struct{
    std::function<void(int)> fptr;
    int start;
    int end;
    
}parallel_for_1_t;

typedef struct{
    std::function<void(int,int)> fptr;
    int count;
    int ptr;
}parallel_for_2_t;

namespace stamp{
    void execute_tuple(std::function<void()> &&lambda1,std::function<void()> &&lambda2);

    void parallel_for(int low,int high,int stride,std::function<void(int)> &&lambda,int numThreads);

    void parallel_for(int high,std::function<void(int)> &&lambda,int numThreads);

    void parallel_for(int low1,int high1,int stride1,int low2,int high2,int stride2,std::function<void(int,int)> &&lambda,int numThreads);

    void parallel_for(int high1,int high2,std::function<void(int,int)> &&lambda,int numThreads);
}

