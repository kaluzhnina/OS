#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <string.h>

#define SHMSZ 128
#define FILENAME "fileshm"

  
int main() 
{ 
    time_t real_time;
    time_t tm; 
    int shmid;

    char *shm;
    sleep(1);
    key_t key = ftok(FILENAME, 2345);
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
        real_time = time(NULL);
        sleep(1);
        printf("PID -- %d;\nTime now: %s\n", getpid(), ctime(&real_time));
    
        printf("Data read from memory: %s\n",shm); 
    }

   shmdt(shm);
     
    return 0; 
} 
