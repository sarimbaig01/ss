#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int done_t1 = 0;
pthread_cond_t cond_t1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock_t1 = PTHREAD_MUTEX_INITIALIZER;

void thread_join(){
    pthread_mutex_lock(&lock_t1);
    while(done_t1==0){
        //wait until t1 signals
        pthread_cond_wait(&cond_t1, &lock_t1);
    }
    pthread_mutex_unlock(&lock_t1);
}

void * worker(void * arg){
    printf("<worker>");
    //does work here...
    pthread_mutex_lock(&lock_t1);
    done_t1 = 1;
    printf("</worker>"); 
    //signal to wake up main
    pthread_cond_signal(&cond_t1);
    pthread_mutex_unlock(&lock_t1);
    return NULL;
}

int main(int argc, char ** argv){
    printf("<main>");
    pthread_t worker_tid;
    if(pthread_create(&worker_tid, NULL, worker, NULL)!=0){
        perror("pthread_create failed");
        exit(1);
    }

    thread_join();
   
    printf("</main>");
    return 0;
}
