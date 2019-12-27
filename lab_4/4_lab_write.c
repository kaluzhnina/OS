#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <string.h>

#define SHMSZ 32
#define FILENAME "fileshm"

int main() 
{ 
    time_t tm; 
    int shmid;
    char* shm, *s;

    key_t key = ftok(FILENAME, 1);
    if (key == -1)
    {
        perror("ftok: no key!");
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
    while(1)
    {
        tm = time(NULL);
        pid_t pid = getpid();
        sprintf(s, "%d", (int)pid);
        sprintf(shm, "%s%s%s", ctime(&tm), "PID -- ", s);
        sleep(1);
    }
    shmdt(shm);
    shmctl(shmid,IPC_RMID,NULL); 
    return 0;
}
  
  