#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

//this is a more modular version of 
//the code I showed in class
//while in class our purpose is to
//keep the syntax simple and focus
//entirely on concurrency concepts
///this code here shows how we can
//avoid global variables and implement
//a more modular, reusable, and scalable
//version of the code
//(Note: the classroom version is
//available below in comment)

#define NUM_READERS 3
#define NUM_WRITERS 2

typedef struct rw_lock{
    sem_t w_lock, lock;
    int num_readers;
} rw_lock_t;

typedef struct rw_arg{
    rw_lock_t * rwl_ptr;
    int id;
} rw_arg_t;

void reader_lock(rw_lock_t* rwl){
    sem_wait(&rwl->lock);
    rwl->num_readers++;
    if(rwl->num_readers==1){
        //first reader acquires
        //the writer lock (as soon as it is available)
        //so that a writer may not enter
        //as long as there is even one reader
        sem_wait(&rwl->w_lock);
    }
    sem_post(&rwl->lock);
}

void reader_unlock(rw_lock_t* rwl){
    sem_wait(&rwl->lock);
    rwl->num_readers--;
    if(rwl->num_readers==0){
        //this is last reader
        //release the w_lock
        //for a potential writer (or another reader)
        sem_post(&rwl->w_lock);
    }
    sem_post(&rwl->lock);
}

void writer_lock(rw_lock_t* rwl){
    sem_wait(&rwl->w_lock);
}

void writer_unlock(rw_lock_t* rwl){
    sem_post(&rwl->w_lock);
}

void rw_lock_init(rw_lock_t *rwl){
     //lock for shared data
    sem_init(&rwl->lock, 0, 1);

    //lock for writer (only acquirable by a writer if 
    //a reader doesn't hold it)
    sem_init(&rwl->w_lock, 0, 1);

    rwl->num_readers = 0;
}

void rw_lock_close(rw_lock_t* rwl){
    sem_destroy(&rwl->lock);
    sem_destroy(&rwl->w_lock);
}

void* reader(void* arg) {
    rw_arg_t * rw_arg = ((rw_arg_t*)arg);
    for (int i = 0; i < 3; i++) {
        reader_lock(rw_arg->rwl_ptr);
        printf("Reader %d is reading...\n", rw_arg->id);
        sleep(1);
        printf("Reader %d is done reading.\n", rw_arg->id);
        reader_unlock(rw_arg->rwl_ptr);
        sleep(1);
    }
    return NULL;
}

void* writer(void* arg) {
    rw_arg_t * rw_arg = ((rw_arg_t*)arg);
    for (int i = 0; i < 2; i++) {
        writer_lock(rw_arg->rwl_ptr);
        printf("Writer %d is writing...\n", rw_arg->id);
        sleep(2);
        printf("Writer %d is done writing.\n", rw_arg->id);
        writer_unlock(rw_arg->rwl_ptr);
        sleep(1);
    }
    return NULL;
}

int main(){
    rw_lock_t rwl;
    rw_lock_init(&rwl);
    
    pthread_t r_threads[NUM_READERS];
    pthread_t w_threads[NUM_WRITERS];

    rw_arg_t r_args[NUM_READERS];
    rw_arg_t w_args[NUM_WRITERS];

    // Start reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        r_args[i].rwl_ptr = &rwl;
        r_args[i].id = i + 1;
        pthread_create(&r_threads[i], NULL, reader, &r_args[i]);
    }

    // Start writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        w_args[i].rwl_ptr = &rwl;
        w_args[i].id = i + 1;
        pthread_create(&w_threads[i], NULL, writer, &w_args[i]);
    }

    // Wait for all threads
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(r_threads[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(w_threads[i], NULL);
    }

    rw_lock_close(&rwl);

    return 0;
}


/*
//classroom version
#define NUM_READERS 3
#define NUM_WRITERS 2

sem_t w_lock, lock;
int num_readers = 0;

void reader_lock(){
    sem_wait(&lock);
    num_readers++;
    if(num_readers==1){
        //first reader acquires
        //the writer lock (as soon as it is available)
        //so that a writer may not enter
        //as long as there is even one reader
        sem_wait(&w_lock);
    }
    sem_post(&lock);
}

void reader_unlock(){
    sem_wait(&lock);
    num_readers--;
    if(num_readers==0){
        //this is last reader
        //release the w_lock
        //for a potential writer (or another reader)
        sem_post(&w_lock);
    }
    sem_post(&lock);
}

void writer_lock(){
    sem_wait(&w_lock);
}

void writer_unlock(){
    sem_post(&w_lock);
}

void* reader(void* arg) {
    int id = *((int*)arg);
    for (int i = 0; i < 3; i++) {
        reader_lock();
        printf("Reader %d is reading...\n", id);
        sleep(1);
        printf("Reader %d is done reading.\n", id);
        reader_unlock();
        sleep(1);
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *((int*)arg);
    for (int i = 0; i < 2; i++) {
        writer_lock();
        printf("Writer %d is writing...\n", id);
        sleep(2);
        printf("Writer %d is done writing.\n", id);
        writer_unlock();
        sleep(1);
    }
    return NULL;
}

int main(){

    //lock for shared data
    sem_init(&lock, 0, 1);

    //lock for writer (only acquirable by a writer if 
    //a reader doesn't hold it)
    sem_init(&w_lock, 0, 1);

    
    pthread_t r_threads[NUM_READERS];
    pthread_t w_threads[NUM_WRITERS];
    int r_ids[NUM_READERS];
    int w_ids[NUM_WRITERS];

    // Start reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        r_ids[i] = i + 1;
        pthread_create(&r_threads[i], NULL, reader, &r_ids[i]);
    }

    // Start writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        w_ids[i] = i + 1;
        pthread_create(&w_threads[i], NULL, writer, &w_ids[i]);
    }

    // Wait for all threads
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(r_threads[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(w_threads[i], NULL);
    }

    sem_destroy(&lock);
    sem_destroy(&w_lock);

    return 0;
}*/

/*
//macOS version
#define NUM_READERS 3
#define NUM_WRITERS 2

typedef struct rw_lock{
    sem_t *w_lock, *lock;
    int num_readers;
} rw_lock_t;

typedef struct rw_arg{
    rw_lock_t * rwl_ptr;
    int id;
} rw_arg_t;

void reader_lock(rw_lock_t* rwl){
    sem_wait(rwl->lock);
    rwl->num_readers++;
    if(rwl->num_readers==1){
        //first reader acquires
        //the writer lock (as soon as it is available)
        //so that a writer may not enter
        //as long as there is even one reader
        sem_wait(rwl->w_lock);
    }
    sem_post(rwl->lock);
}

void reader_unlock(rw_lock_t* rwl){
    sem_wait(rwl->lock);
    rwl->num_readers--;
    if(rwl->num_readers==0){
        //this is last reader
        //release the w_lock
        //for a potential writer (or another reader)
        sem_post(rwl->w_lock);
    }
    sem_post(rwl->lock);
}

void writer_lock(rw_lock_t* rwl){
    sem_wait(rwl->w_lock);
}

void writer_unlock(rw_lock_t* rwl){
    sem_post(rwl->w_lock);
}

void rw_lock_init(rw_lock_t *rwl){
     //lock for shared data
    rwl->lock = sem_open("/lock", O_CREAT, 0644, 1);

    //lock for writer (only acquirable by a writer if 
    //a reader doesn't hold it)
    rwl->w_lock = sem_open("/w_lock", O_CREAT, 0644, 1);

    rwl->num_readers = 0;
}

void rw_lock_close(rw_lock_t* rwl){
    sem_unlink("/lock");
    sem_unlink("/w_lock");
    sem_close(rwl->lock);
    sem_close(rwl->w_lock);
}

void* reader(void* arg) {
    rw_arg_t * rw_arg = ((rw_arg_t*)arg);
    for (int i = 0; i < 3; i++) {
        reader_lock(rw_arg->rwl_ptr);
        printf("Reader %d is reading...\n", rw_arg->id);
        sleep(1);
        printf("Reader %d is done reading.\n", rw_arg->id);
        reader_unlock(rw_arg->rwl_ptr);
        sleep(1);
    }
    return NULL;
}

void* writer(void* arg) {
    rw_arg_t * rw_arg = ((rw_arg_t*)arg);
    for (int i = 0; i < 2; i++) {
        writer_lock(rw_arg->rwl_ptr);
        printf("Writer %d is writing...\n", rw_arg->id);
        sleep(2);
        printf("Writer %d is done writing.\n", rw_arg->id);
        writer_unlock(rw_arg->rwl_ptr);
        sleep(1);
    }
    return NULL;
}

int main(){
   
    rw_lock_t rwl;
    rw_lock_init(&rwl);
    
    pthread_t r_threads[NUM_READERS];
    pthread_t w_threads[NUM_WRITERS];
  

    //thread argument
    rw_arg_t args;
    args.rwl_ptr = &rwl;
    

    // Start reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        rw_arg_t args;
        args.rwl_ptr = &rwl;
        args.id = i+1;
        pthread_create(&r_threads[i], NULL, reader, &args);
    }

    // Start writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        rw_arg_t args;
        args.rwl_ptr = &rwl;
        args.id = i+1;
        pthread_create(&w_threads[i], NULL, writer, &args);
    }

    // Wait for all threads
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(r_threads[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(w_threads[i], NULL);
    }

    rw_lock_close(&rwl);

    return 0;
}
*/
