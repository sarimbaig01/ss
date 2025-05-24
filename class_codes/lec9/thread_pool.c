#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_WORKERS 3
#define MAX_JOBS    15

int job_queue[MAX_JOBS];
int job_count = 0;
int shutdown = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void busy_wait(int cycles) {
    volatile int dummy = 0;
    for (int i = 0; i < cycles; i++) {
        dummy += i;
    }
}

void process_job(int job, int worker_id) {
    printf("Worker %d: processing job %d\n\n", worker_id, job);
}

void* worker_thread(void* arg) {
    int id = *(int*)arg;
    int keep_running = 1;

    while (keep_running) {
        pthread_mutex_lock(&lock);

        while (job_count == 0 && !shutdown)
            pthread_cond_wait(&cond, &lock);

        if (shutdown && job_count == 0) {
            keep_running = 0;  // end loop after unlock
        } else {
            int job = job_queue[--job_count];
            pthread_mutex_unlock(&lock);

            process_job(job, id);
            int loops = 50000000 + rand() % 50000000;
            busy_wait(loops);
        }

        if (keep_running == 0) {
            pthread_mutex_unlock(&lock);
        }
    }

    printf("Worker %d: shutting down\n\n", id);
    return NULL;
}


int main() {
    pthread_t workers[NUM_WORKERS];
    int ids[NUM_WORKERS];

    srand(time(NULL));  // Seed the random number generator

    // Start worker threads
    for (int i = 0; i < NUM_WORKERS; i++) {
        ids[i] = i + 1;
        pthread_create(&workers[i], NULL, worker_thread, &ids[i]);
    }

    // Add jobs to the queue
    for (int j = 1; j <= MAX_JOBS; j++) {
        sleep(rand() % 2);  // Simulate random delay between jobs

        pthread_mutex_lock(&lock);
        job_queue[job_count++] = j;
        printf("Main: added job %d to the queue\n\n", j);
        pthread_cond_signal(&cond);  // Wake up one worker
        pthread_mutex_unlock(&lock);
    }

    // Signal shutdown and wake all workers
    pthread_mutex_lock(&lock);
    shutdown = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);

    // Join all worker threads
    for (int i = 0; i < NUM_WORKERS; i++) {
        pthread_join(workers[i], NULL);
    }

    printf("Main: all workers have finished. Exiting.\n\n");
    return 0;
}
