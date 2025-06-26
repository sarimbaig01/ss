#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

int buff;
int full; //0: not full, 1: full

void produce(int val){
    assert(full==0);
    full = 1;
    buff = val;
}

int consume(){
    assert(full==1);
    full = 0;
    return buff;
}

void *prod(void *args) {
    while (1) {
        produce(rand());
    }
}

void *cons(void *args) {
    while (1) {
        int tmp = consume();
        printf("Consumed: %d\n", tmp);
    }
}

int main() {
    pthread_t prod_thread, cons_thread;

    // Create producer thread
    if (pthread_create(&prod_thread, NULL, prod, NULL) != 0) {
        perror("Failed to create producer thread");
        exit(1);
    }

    // Create consumer thread
    if (pthread_create(&cons_thread, NULL, cons, NULL) != 0) {
        perror("Failed to create consumer thread");
        exit(1);
    }

    // Join threads (in practice, these will run indefinitely)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    return 0;
}
