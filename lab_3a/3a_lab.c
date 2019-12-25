#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h> 
#include <sys/types.h>
#include <sys/wait.h>

void end_exit()
{
    printf("end prcess PID -- %d\n", getpid());
}

int main()
{   
    time_t tm;  
    pid_t pid;
    time_t buf;

    int fildes[2];
    ssize_t nbytes;
    int status;
    

    status = pipe(fildes);
    if (status == -1) 
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    switch (pid = fork()) 
    {
        case -1: 
            perror("fork");
            exit(EXIT_FAILURE);
        break;

        case 0:  
            printf("CHILD: PID -- %d, parent's PID -- %d\n", getpid(), getppid());
            close(fildes[1]); 
            sleep(1);                     
            nbytes = read(fildes[0], &buf, sizeof(time_t));  
            close(fildes[0]);
            printf("CHILD: Received string: %s", ctime(&buf)); 
            tm = time(NULL);
            printf("CHILD: Real time: %s", ctime(&tm)); 
            atexit(end_exit); 
            exit(EXIT_SUCCESS);
        break; 

        default: 
            printf("PARENT: PID -- %d, child's PID -- %d\n", getpid(), pid);
            tm = time(NULL);
            close(fildes[0]);                      
            write(fildes[1],&tm, sizeof(time_t));  
            close(fildes[1]); 
            int child_exit_code = 0;                
            waitpid(pid, &child_exit_code, 0);
            atexit(end_exit);
        break;    
    }
    return 0;
}
