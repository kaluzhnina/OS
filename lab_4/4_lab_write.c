#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <string.h>
#include <errno.h>

#define SHMSZ 128
#define FILENAME "fileshm"

int main() 
{ 
    time_t tm; 
    int shmid;
    char *shm;
    char *s;

    key_t key = ftok(FILENAME, 2345);
    if (key == -1)
    {
        perror("ftok: no key!");
        exit(EXIT_FAILURE);
    }
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT| IPC_EXCL| 0666)) < 0) 
    {
        if (errno == EEXIST)
        {
            shmid = shmget(key, SHMSZ, 0);
            if(shmid == -1)
            {
                perror("");
                exit(1);
            }
        }else{
            perror("shmget");
            exit(EXIT_FAILURE);
        }
    }

    void* shmaddr;

    if ((char*)(shmaddr = shmat(shmid, NULL, SHM_RND)) == (char*) -1) 
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        tm = time(NULL);
        pid_t pid = getpid();
        sprintf(s, "%d", (int)pid);
        sprintf((char*)shmaddr, "%s%s%s", ctime(&tm), "PID -- ", s);
        sleep(1);
    }
    shmdt(shm);
    shmctl(shmid,IPC_RMID,NULL); 
    return 0;
}
  
  