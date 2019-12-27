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
    time_t tm; 
    int shmid;
    int semid;
    char* shm, *s;
    struct sembuf sem_lock = { 0, -1, 0};
    struct sembuf sem_unlock = { 0, 1, 0};

    key_t key = ftok(FILENAME, 1);
     if (key == -1)
    {
        perror("ftok: no key!");
        exit(EXIT_FAILURE);
    }
    if((semid = semget(key, 1, IPC_CREAT | 0666)) < 0)
    {
        perror("semid:can not get semaphore");
        exit(EXIT_FAILURE);
    }
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) 
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) 
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    semop(semid, &sem_unlock, 1);
    while(1)
    {
        semop(semid, &sem_lock, 1);
        tm = time(NULL);
        pid_t pid = getpid();
        sprintf(s, "%d", (int)pid);
        sprintf(shm, "%s%s%s", ctime(&tm), "PID -- ", s);
        sleep(1);
        semop(semid, &sem_unlock, 1);
    }
    shmdt(shm);
    shmctl(shmid,IPC_RMID,NULL); 
    return 0;
}
  
  