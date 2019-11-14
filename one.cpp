#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h> 

// Fx to start a thread
int count = 0;

// Creating a thread od supplying pizzas
pthread_mutex_t mutex;
void *threadS(void *args)
{
    pthread_mutex_lock(&mutex);
    if(count<=10)
    {
            count= count+1;
            printf("%d pizza left after supplying\n",count);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

// Creating a thread od consuming pizzas
   void *threadC(void *args)
{
    pthread_mutex_lock(&mutex);
    if(count>0)
    {
            count= count - 1;
            printf("%d pizza left after eating\n",count);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    pthread_t thread_Sid,  thread_Cid;
    while(1)
    {
    while(count<10)
    {
        pthread_create(&thread_Sid, NULL, threadS,NULL);
    }
    while(count>1)
    {
        pthread_create(&thread_Cid,NULL,threadC,NULL);
    }
    }
    pthread_join(thread_Sid, NULL);
    pthread_join(thread_Cid, NULL);
    exit(0);
}  