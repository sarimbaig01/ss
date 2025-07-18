// This program shows how we can explicitly
// map an unnamed semaphore to a memory location
// shared between processes.
// This extra work is not needed when using a 
// named semaphore, as shown on the slide
// and in the file named_sem.c

//Note: this code will not work on macOS
//which does not support unnamed semaphores


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

#define SHM_NAME "/shared_sem"
///SHM: stands for shared memory
#define SEM_SIZE sizeof(sem_t)

int main(int argc, char *argv[]) {
    int first_time_creator = 0;

    // Attempt to create shared memory exclusively
    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (fd != -1) {
        first_time_creator = 1;  // This process created it
    } else if (errno == EEXIST) {
        // Already exists; open without O_EXCL
        fd = shm_open(SHM_NAME, O_RDWR, 0666);
        if (fd == -1) {
            perror("shm_open (second attempt)");
            exit(EXIT_FAILURE);
        }
    } else {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Set size if this is the first creator
    if (first_time_creator) {
        if (ftruncate(fd, SEM_SIZE) == -1) {
            perror("ftruncate");
            exit(EXIT_FAILURE);
        }
    }

    // Map the shared memory
    // Due to the shared fd, the process would map to the shared memory location
    // with the other processes using this shm object

    // The fd in the following call links each process
    // to the same shared physical memory
    sem_t *sem = mmap(NULL, SEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sem == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphore only once
    if (first_time_creator) {
        if (sem_init(sem, 1, 1) == -1) {
            perror("sem_init");
            exit(EXIT_FAILURE);
        }
    }

    // Use the semaphore
    sem_wait(sem);
    // critical section here...
    // for example, writing to a shared file...
    sem_post(sem);

    // Cleanup
    munmap(sem, SEM_SIZE);
    close(fd);

    // Final cleanup
    if (argc > 1 && strcmp(argv[1], "cleanup") == 0) {
        sem_destroy(sem);
        shm_unlink(SHM_NAME);
    }

    return 0;
}
