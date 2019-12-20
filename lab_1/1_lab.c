#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void end_exit_child()
{
    printf("atexit():end of process child... PID : %d\n", getpid());
}

void end_exit_parent()
{
    printf("atexit(): end of process parent...PID : %d\n", getpid());
}



int main()
{
    pid_t pid;
    
    switch(pid = fork())
    {
        case -1: 
            perror("fork");
            exit(1);
        case 0:
            printf("CHILD:This child process!\n");
            atexit(end_exit_child);
            printf("CHILD:My PID -- %d\n", getpid());
            printf("CHILD:My parent's PID -- %d\n", getppid());
            printf("CHILD:exit!\n");
            int a = 2;
            int exit_code = 10/a;
            exit(exit_code);
        default:
            printf("PARENT:This parent process!\n");
            printf("PARENT:My PID -- %d\n", getpid());
            printf("PARENT:My child's PID -- %d\n", pid);
            printf("PARENT:Waiting for the completion of the child  process...\n");
            int child_exit_code = 0;
            waitpid(pid, &child_exit_code, 0);
            printf("PARENT:child exit code %d\n", WEXITSTATUS(child_exit_code));
            atexit(end_exit_parent);
    }
    return 0;
}