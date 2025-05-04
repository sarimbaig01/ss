#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE 10000

//structure to hold arguments
typedef struct args_type{
    int * arr;
    int l, r;
    int sum; //used for output
} args_type_t;

void *sum_arr(void* args){
    args_type_t * a = (args_type_t*)args;
    a->sum=0;
    for(int i = a->l; i<=a->r; i+=2){
        a->sum+=a->arr[i];
    }
    return NULL;
}

void init_i(int v[]){
    for (int i = 0; i < ARR_SIZE; i++) {
        v[i] = i;
    }
}

int main() {

    int v[ARR_SIZE];
    init_i(v);

    //set up parameters
    args_type_t args1, args2;
    args1.arr = args2.arr = v;
    args1.l = 0, args1.r = ARR_SIZE-1;
    args2.l = 1, args2.r = ARR_SIZE-1;

    pthread_t t1, t2;
    
    //thread to sum even indices 
    pthread_create(&t1, NULL, sum_arr, (void*)&args1);

    //thread to sum odd indices 
    pthread_create(&t2, NULL, sum_arr, (void*)&args2);

    //wait for both threads to finish
    pthread_join(t1, NULL);     
    pthread_join(t2, NULL);            

    //print the total sum of even and odd indices
    printf("v.v = %d\n", args1.sum+args2.sum);

    return 0;
}
