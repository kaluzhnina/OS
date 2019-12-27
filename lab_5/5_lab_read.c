#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <string.h>
#include <sys/sem.h>

#define SHMSZ 32
#define FILENAME "fileshm"

  
int main() 
{ 
    time_t real_time;
    time_t tm; 
    int shmid;
    int semid;
    struct sembuf sem_lock = { 0, -1, 0};
    struct sembuf sem_unlock = { 0, 1, 0};


    char *shm;
    key_t key = ftok(FILENAME, 1);
     if (key == -1)
    {
        perror("ftok: no key!");
        exit(EXIT_FAILURE);
    }
    if((semid = semget(key, 1, 0666)) < 0)
    {
        perror("semid:can not get semaphore");
        exit(EXIT_FAILURE);
    }
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) 
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) 
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        semop(semid, &sem_lock, 1);
        real_time = time(NULL);
        sleep(1);
        printf("PID -- %d;\nTime now: %s\n", getpid(), ctime(&real_time));
    
        printf("Data read from memory: %s\n",shm); 
        semop(semid, &sem_unlock, 1);
    }

   shmdt(shm);
     
    return 0; 
} 
