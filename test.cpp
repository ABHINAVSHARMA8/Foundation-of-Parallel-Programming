#include "quill.h"
#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<assert.h>
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
using namespace std;
struct T{
    
    int x;
};
T a[2];
void f(){
   a[0].x=5;
   a[1].x=8;
    
}
int main(){
    T a1,a2;a1.x=0;a2.x=1;
    a[0]=a1;a[0]=a2;
    f();
    cout<<a[0].x<<" "<<a[1].x<<endl;
    return 0;
}