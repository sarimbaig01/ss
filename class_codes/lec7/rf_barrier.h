#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    unsigned count;
    unsigned gen;
    unsigned max_t_count;
} barrier_t;

void barrier_generation(barrier_t* barrier, int *gen) {   
    pthread_mutex_lock(&barrier->mutex);
    *gen = barrier->gen;
    pthread_mutex_unlock(&barrier->mutex);
}

void barrier_init(barrier_t* barrier, int thread_count, int max_gen) {
    barrier->count = 0;
    barrier->max_t_count = thread_count;
    barrier->gen = 0;
    pthread_mutex_init(&barrier->mutex, 0);
    pthread_cond_init(&barrier->cond, 0);
}

void barrier_wait(barrier_t* barrier) {
    pthread_mutex_lock(&barrier->mutex);

    unsigned my_gen = barrier->gen;

    barrier->count++;
    if (barrier->count >= barrier->max_t_count) {
        // Last thread has arrived, reset count and wake up everyone
        barrier->count = 0;
        barrier->gen++;
        pthread_cond_broadcast(&barrier->cond);
    } else {
        // Wait for the generation to advance
        while (my_gen == barrier->gen) {
            pthread_cond_wait(&barrier->cond, &barrier->mutex);
        }
    }

    pthread_mutex_unlock(&barrier->mutex);
}

void barrier_destroy(barrier_t* barrier) {
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
}
