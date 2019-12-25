#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h> 
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

void end_exit()
{
    printf("end prcess PID -- %d\n", getpid());
}

int main()
{   
    time_t buf;
    time_t tm;  
    pid_t pid;

    ssize_t nbytes;

    int fifo_descriptor;
    char fifoname[256];
    strcpy(fifoname, "/tmp/fifo");

    unlink(fifoname);
    if (mkfifo(fifoname, 0777) == -1) 
    {
        perror("fifo: fail!");
        exit(EXIT_FAILURE);
    }

    switch (pid = fork()) 
    {
        case -1: 
            perror("fork: fail!");
            exit(EXIT_FAILURE);
            break;

        case 0:  
            printf("CHILD: PID -- %d, parent's PID -- %d\n", getpid(), getppid());
            sleep(1);  
            fifo_descriptor = open(fifoname, O_RDONLY); 
            if (fifo_descriptor == -1)
            {
                perror("Can't open fifo!");
                exit(EXIT_FAILURE);
            } 

            nbytes = read(fifo_descriptor, &buf, sizeof(time_t));  
            close(fifo_descriptor);
            printf("CHILD: Received string: %s", ctime(&buf)); 
            tm = time(NULL);
            printf("CHILD: Real time: %s", ctime(&tm)); 
            atexit(end_exit); 
            exit(EXIT_SUCCESS);
            break; 

        default: 
            printf("PARENT: PID -- %d, child's PID -- %d\n", getpid(), pid);
            tm = time(NULL);
            fifo_descriptor = open(fifoname, O_WRONLY);
            if (fifo_descriptor == -1)
            {
                perror("Can't open fifo!");
                exit(EXIT_FAILURE);
            }   
            write(fifo_descriptor,&tm, sizeof(time_t));  
            close(fifo_descriptor); 
            int child_exit_code = 0;                
            waitpid(pid, &child_exit_code, 0);
            atexit(end_exit);
            break;
        
    }
    return 0;
}
