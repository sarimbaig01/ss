///The program runs in an infinite loop
///Press Ctrl + C to end the process
///and see output

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define MAX_SIZE 100

int buff [MAX_SIZE];
int counter = 0;
int next_prod = 0, next_cons = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t  cond_p = PTHREAD_COND_INITIALIZER;
pthread_cond_t  cond_c = PTHREAD_COND_INITIALIZER;

void produce(int val){
    assert(counter<MAX_SIZE);
    buff[next_prod] = val;
    next_prod = (next_prod + 1) % MAX_SIZE;
    counter++;
}

int consume(){
    assert(counter>0);
    int tmp = next_cons;
    next_cons = (next_cons + 1) % MAX_SIZE;
    counter--;
    return buff[tmp];
}

void *prod(void *args) {
    while(1) {
        pthread_mutex_lock(&lock);
        while(counter==MAX_SIZE){
            pthread_cond_wait(&cond_p, &lock);
        }
        int tmp = rand()%1000;
        produce(tmp);
        printf("Produced: %d\n", tmp);
        pthread_cond_signal(&cond_c);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *cons(void *args) {
     while(1){
        pthread_mutex_lock(&lock);
        while(counter==0){
            pthread_cond_wait(&cond_c, &lock);
        }
        int tmp = consume();
        printf("Consumed: %d\n", tmp);
        pthread_cond_signal(&cond_p);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}


int main() {
    pthread_t prod_thread, cons_thread1, cons_thread2;

    // Create producer thread
    if (pthread_create(&prod_thread, NULL, prod, NULL) != 0) {
        perror("Failed to create producer thread");
        exit(1);
    }

    // Create consumer thread
    if (pthread_create(&cons_thread1, NULL, cons, NULL) != 0) {
        perror("Failed to create consumer thread");
        exit(1);
    }

    // Create another consumer thread
    if (pthread_create(&cons_thread2, NULL, cons, NULL) != 0) {
        perror("Failed to create consumer thread");
        exit(1);
    }

    // Join threads (in practice, these will run indefinitely)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread1, NULL);
    pthread_join(cons_thread2, NULL);

    return 0;
}
