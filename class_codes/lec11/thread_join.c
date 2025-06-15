#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int done_t1 = 0;
pthread_cond_t cond_t1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock_t1 = PTHREAD_MUTEX_INITIALIZER;

void * t1(void * arg){
    printf("<t1>");
    //does work here...
    pthread_mutex_lock(&lock_t1);
    done_t1 = 1;
    printf("</t1>"); 
    //signal to wake up t0
    pthread_cond_signal(&cond_t1);
    pthread_mutex_unlock(&lock_t1);
    return NULL;
}

void * t0(void * arg){
    printf("<t0>");
    pthread_t tid;
    if(pthread_create(&tid, NULL, t1, NULL)!=0){
        perror("pthread_create failed");
        exit(1);
    }
    pthread_mutex_lock(&lock_t1);
    while(done_t1==0){
        //wait until t1 signals
        pthread_cond_wait(&cond_t1, &lock_t1);
    }
    pthread_mutex_unlock(&lock_t1);
    printf("</t0>");
    return NULL;
}

int main(int argc, char ** argv){
    t0(NULL);
    return 0;
}
