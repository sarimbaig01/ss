#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

#define SEM_NAME "lock_sem"

int main() {
    //Using sem_open to create a 'named semaphore'
    //i.e., a semaphore with a name associated with
    //it in the file system.

    //The first process to execute the following line
    //will create the semaphore in kernel-managed memory,
    //while the subsequent processes will use the same object
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    //now use the semaphore as a lock,
    //for example to open a shared file

    sem_wait(sem);  // Lock

    FILE *fp = fopen("shared.txt", "a");
    if (fp) {
        fprintf(fp, "Process %d wrote this line.\n", getpid());
        fclose(fp);
    }

    sem_post(sem);  // Unlock
    sem_close(sem);
    // sem_unlink(SEM_NAME); // Optional: remove semaphore when no longer needed

    return 0;
}
