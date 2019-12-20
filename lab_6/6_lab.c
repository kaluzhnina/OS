#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#define NUM_READER_THREADS      10
#define ARRAYSIZE   20 //при больших размерах будет тормозить вывод. 

char a[ARRAYSIZE];
int current_index = 0;
pthread_mutex_t array_mutex;

void *writer(void *tid)
{
    int i = 0;
    while (i++ < ARRAYSIZE - 1)
    {
        pthread_mutex_lock(&array_mutex);
        a[current_index++] = 'A' + (random() % 26);
        pthread_mutex_unlock(&array_mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *reader(void *tid)
{
    int *myid;
    myid = (int *)tid;
    while(1)
    { 
        if (current_index == ARRAYSIZE - 1)
        {
            printf("%d: NOTHING TO DO EXIT\n", *myid);
            pthread_exit(NULL);
        }
        else
        {
            pthread_mutex_lock(&array_mutex);
            printf("%d: ", *myid);
            for (int i = 0; i <= current_index; i++)
            {
                printf("%c", a[i]);
            }
            printf("\n");
            pthread_mutex_unlock(&array_mutex);
            sleep(*myid + 1);
        }
        
    }
}

int main()
{
    pthread_t writer_thread;
    pthread_t reader_threads[NUM_READER_THREADS];
    pthread_attr_t attr;
    int reader_tids[NUM_READER_THREADS];
    int writer_tid = INT_MAX;

    pthread_mutex_init(&array_mutex, NULL);
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&writer_thread, &attr, writer, (void *) &writer_tid);

    for (int i = 0; i < NUM_READER_THREADS; i++) {
        reader_tids[i] = i;
        pthread_create(&reader_threads[i], &attr, reader, (void *) &reader_tids[i]);
    }

    pthread_join(writer_thread, NULL);
    for (int i = 0; i< NUM_READER_THREADS; i++) {
        pthread_join(reader_threads[i], NULL);
    }
    printf("### THE END ###\n");

    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&array_mutex);
    pthread_exit(NULL);
    return 0;
}
