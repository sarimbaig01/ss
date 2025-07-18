#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

sem_t s;  // Unnamed semaphore (no pointer needed)

void *child_thread_function(void *args) {
    printf("This is the child thread running...\n");

    printf("This is the child finished...\n");
    sem_post(&s);  // Post to the semaphore
    return NULL;
}

int main() {
    printf("This is the parent thread running...\n");

    // Initialize unnamed semaphore for use between threads (pshared = 0)
    sem_init(&s, 0, 0);

    pthread_t child_thread;
    pthread_create(&child_thread, NULL, child_thread_function, NULL);

    printf("Parent thread is now waiting for the child to finish...\n");
    sem_wait(&s);  // Wait for child to post
    printf("Parent thread is done waiting...\n");

    // Clean up semaphore
    sem_destroy(&s);
    return 0;
}

/*
//macOS version, using sem_open

sem_t * s;

void *child_thread_function(void *args){

    printf("This is the child thread running...\n");


    printf("This is the child finished...\n");
    sem_post(s);
    return NULL;
}

int main(){

    printf("This is the parent thread running...\n");

   
    //sem_init(&s, 0, 0);
    s =  sem_open("/join_sem", O_CREAT, 0644, 0);

    pthread_t child_thread;
    pthread_create(&child_thread, NULL, child_thread_function, NULL);

    printf("Parent thread is now waiting for the child to finish...\n");
    sem_wait(s);
    printf("Parent thread is done waiting...\n");

    sem_close(s);
    sem_unlink("join_sem");
}*/
