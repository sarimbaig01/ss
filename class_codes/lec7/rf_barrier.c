#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "rf_barrier.h"

//data
#define MAX_GEN 50
#define MAX_THR 3
double R[MAX_GEN+1], F[MAX_GEN+1];
const double a = 0.01, r = 0.1, b= 0.01, m = 0.1;

//r = prey reproduction rate
//a = predation rate
//m = predator mortality rate
//b = predator reproduction coefficient
//R[n+1] = R[n] + rR[n] + aR[n]F[n]
//F[n+1] = F[n] - mF[n] + bR[n]F[n]  

barrier_t barrier;

void* update_rabbits(void* args) {

    int t;
    
    //safely read the generation number
    barrier_generation(&barrier,&t);

    while (t<MAX_GEN) { 
        R[t+1] = R[t] + r * R[t] - a * R[t] * F[t];
        barrier_wait(&barrier);

        //safely read the generation number
        barrier_generation(&barrier,&t);
    }
    return NULL;
}

void* update_foxes(void* args) {

    int t;
    //safely read the generation number
    barrier_generation(&barrier,&t);
    
    while (t<MAX_GEN) {
        F[t+1] = F[t] - m * F[t] + b * R[t] * F[t];
        barrier_wait(&barrier);

         //safely read the generation number
        barrier_generation(&barrier,&t);
    }
    return NULL;
}


int main(){

    //Initial populations
    R[0] = 40;
    F[0] = 9;

    //Print Initial populations
    printf("Generation %d: Rabbits = %f, Foxes = %f\n", 0, R[0], F[0]);

    //Initialize the barrier
    barrier_init(&barrier, MAX_THR, MAX_GEN);

    //Create the thread pool
    pthread_t threads[2];
    pthread_create(&threads[0], NULL, update_rabbits, NULL);
    pthread_create(&threads[1], NULL, update_foxes, NULL);

    int t = 1;
    while (t<MAX_GEN) {
        // Wait until both workers have finished computation
        barrier_wait(&barrier);

        // Print populations
        barrier_generation(&barrier,&t);
        
        printf("Generation %d: Rabbits = %f, Foxes = %f\n", t, R[t], F[t]);
    }

    // Join threads
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    barrier_destroy(&barrier);

    return 0;
}
