///The program runs in an infinite loop
///Press Ctrl + C to end the process
///and see output

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <semaphore.h>

#define MAX_SIZE 100

int buff[MAX_SIZE];
int counter = 0;
int next_prod = 0, next_cons = 0;

//semaphores
sem_t sem_lock;
sem_t sem_p;
sem_t sem_c;

void produce(int val) {
    sem_wait(&sem_lock);
    //locked critical section
    assert(counter < MAX_SIZE);
    buff[next_prod] = val;
    next_prod = (next_prod + 1) % MAX_SIZE;
    counter++;
    sem_post(&sem_lock);
}

int consume() {
     
    sem_wait(&sem_lock);
    //locked critical section
    assert(counter > 0);
    int tmp = next_cons;
    next_cons = (next_cons + 1) % MAX_SIZE;
    counter--;
    sem_post(&sem_lock);
    return buff[tmp];
}

void *prod(void *args) {
    while (1) {
        sem_wait(&sem_p);
        int tmp = rand() % 1000;
        produce(tmp);
        printf("Produced: %d\n", tmp);
        sem_post(&sem_c);
    }
    return NULL;
}

void *cons(void *args) {
    while (1) {
        sem_wait(&sem_c);
        int tmp = consume();
        printf("Consumed: %d\n", tmp);
        sem_post(&sem_p);
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread1, cons_thread2;

    //initialize semaphores
    sem_init(&sem_lock, 0, 1);           //binary semaphore (mutex)
    sem_init(&sem_p, 0, MAX_SIZE);       //available buffer slots (producer queue)
    sem_init(&sem_c, 0, 0);              //available items to consume (consumer queue)

    //create producer thread
    if (pthread_create(&prod_thread, NULL, prod, NULL) != 0) {
        perror("Failed to create producer thread");
        exit(1);
    }

    //create consumer threads
    if (pthread_create(&cons_thread1, NULL, cons, NULL) != 0 ||
        pthread_create(&cons_thread2, NULL, cons, NULL) != 0) {
        perror("Failed to create consumer thread");
        exit(1);
    }

    //join threads (won’t terminate normally)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread1, NULL);
    pthread_join(cons_thread2, NULL);

    //cleanup (not reached unless you modify loop or use signal handler)
    sem_destroy(&sem_lock);
    sem_destroy(&sem_p);
    sem_destroy(&sem_c);

    return 0;
}



/*
//macOS version
#define MAX_SIZE 100

int buff [MAX_SIZE];
int counter = 0;
int next_prod = 0, next_cons = 0;

sem_t * sem_lock; 

sem_t *sem_p, *sem_c; 

void produce(int val){
    sem_wait(sem_lock);
    assert(counter<MAX_SIZE);
    buff[next_prod] = val;
    next_prod = (next_prod + 1) % MAX_SIZE;
    counter++;
    sem_post(sem_lock);
}

int consume(){
    sem_wait(sem_lock);
    assert(counter>0);
    int tmp = next_cons;
    next_cons = (next_cons + 1) % MAX_SIZE;
    counter--;
    sem_post(sem_lock);
    return buff[tmp];
}

void *prod(void *args) {
    while(1) {
        sem_wait(sem_p);
        int tmp = rand()%1000;
        produce(tmp);
        printf("Produced: %d\n", tmp);
        sem_post(sem_c);
    }
    return NULL;
}

void *cons(void *args) {
     while(1){
        sem_wait(sem_c);//note: unlike pthread_cond_wait, sem_wait does not release the lock
        int tmp = consume();
        printf("Consumed: %d\n", tmp);
        sem_post(sem_p);
    }
    return NULL;
}


int main() {

    //initialize semaphores

    //initialize lock to 1
    sem_lock = sem_open("/sem_lock1", O_CREAT, 0644, 1);

    //initialize producer semaphore to MAX_SIZE 
    sem_p = sem_open("/sem_p1", O_CREAT, 0644, MAX_SIZE);

    //initialize consumer semaphore to 0 
    sem_c = sem_open("/sem_c1", O_CREAT, 0644, 0);

    //add at the very start of main
    sem_unlink("/sem_lock1");
    sem_unlink("/sem_p1");
    sem_unlink("/sem_c1");


    pthread_t prod_thread, cons_thread1, cons_thread2;

    //create producer thread
    if (pthread_create(&prod_thread, NULL, prod, NULL) != 0) {
        perror("Failed to create producer thread");
        exit(1);
    }

    //create consumer thread
    if (pthread_create(&cons_thread1, NULL, cons, NULL) != 0) {
        perror("Failed to create consumer thread");
        exit(1);
    }

    //create another consumer thread
    if (pthread_create(&cons_thread2, NULL, cons, NULL) != 0) {
        perror("Failed to create consumer thread");
        exit(1);
    }

    //join threads (in practice, the will run indefinitely)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread1, NULL);
    pthread_join(cons_thread2, NULL);

    sem_close(sem_lock);
    sem_close(sem_p);
    sem_close(sem_c);

    return 0;
}
    */
