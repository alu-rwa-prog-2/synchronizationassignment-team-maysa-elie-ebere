#include <stdio.h> 
#include <unistd.h>
#include <pthread.h> 
#include <semaphore.h> 

#define P 5 
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 
#define LEFT (philosNum + 4) % P 
#define RIGHT (philosNum + 1) % P 

int state[P]; 
int phil[P] = { 0, 1, 2, 3, 4 }; 

sem_t mutex; 
sem_t S[P]; 

void check(int philosNum) 
{ 
    if (state[philosNum] == HUNGRY 
        && state[LEFT] != EATING 
        && state[RIGHT] != EATING) { 
        
        state[philosNum] = EATING; 

        sleep(2); 

        printf("Philosopher %d takes fork %d and %d\n", 
                    philosNum + 1, LEFT + 1, philosNum + 1); 

        printf("Philosopher %d is Eating\n", philosNum + 1); 

        sem_post(&S[philosNum]); 
    } 
} 

// Fx to take a fork/chopsticks
void liftFork(int philosNum) 
{ 

    sem_wait(&mutex); 

    // Hungry state
    state[philosNum] = HUNGRY; 

    printf("Philosopher %d is Hungry\n", philosNum + 1); 

    // Use a check fx that says if you eat if neighbours are not eating 
    check(philosNum); 

    sem_post(&mutex); 

    sem_wait(&S[philosNum]); 

    sleep(1); 
} 

// // Fx to put down a fork/chopsticks
void layFork(int philosNum) 
{ 

    sem_wait(&mutex); 

    // Thinking state 
    state[philosNum] = THINKING; 

    printf("Philosopher %d putting fork %d and %d down %d\n", philosNum + 1, LEFT + 1, philosNum + 1, RIGHT + 1); 
    printf("Philosopher %d is thinking\n", philosNum + 1); 

    check(LEFT); 
    check(RIGHT); 

    sem_post(&mutex); 
} 

void* philospher(void* num) 
{ 

    while (1) { 

        int* i = num; 

        sleep(1); 

        liftFork(*i); 

        sleep(0); 

        layFork(*i); 
    } 
} 

int main() 
{ 

    int i; 
    pthread_t thread_id[P]; 

    // Use semaphores in synchronization 
    sem_init(&mutex, 0, 1); 

    for (i = 0; i < P; i++) 

        sem_init(&S[i], 0, 0); 

    for (i = 0; i < P; i++) { 

        // Process creation 
        pthread_create(&thread_id[i], NULL, 
                    philospher, &phil[i]); 

        printf("Philosopher %d is thinking\n", i + 1); 
    } 

    for (i = 0; i < P; i++) 

        pthread_join(thread_id[i], NULL); 
} 