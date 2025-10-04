#include <stdio.h>
#include <pthread.h>

#define LOOP_COUNT 5000

int counter = 0;

void *worker(void *arg) {
    for (long i = 0; i < LOOP_COUNT; i++) {
        counter++; 
    }
    return NULL;
}

int main(int argc, char* argv[]) {

    pthread_t p1, p2;

    printf("Initial value of counter: %d\n", counter);

    if (pthread_create(&p1, NULL, worker, NULL) != 0 ||
        pthread_create(&p2, NULL, worker, NULL) != 0) {
        printf("Error in pthread_create");
        return 1;
    }

    // wait for the two threads
    // to finish
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("Final value of counter: %d\n", counter);
    return 0;
}
