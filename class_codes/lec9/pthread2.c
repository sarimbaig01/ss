#include <pthread.h>
#include <stdio.h>

int g_count = 0;

void* counter(void *arg)
{
    for(int i = 0; i < 10000; i++){
        g_count++;
    }
    
    return NULL;
}

int main(int argc, char* argv[]){

    pthread_t t1, t2;

    pthread_create(&t1, NULL, counter, NULL);
    pthread_create(&t2, NULL, counter, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final value of g_count: %d\n", g_count);

    return 0;
}
