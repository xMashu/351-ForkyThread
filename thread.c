#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    int id;
} stuff_t;

pthread_mutex_t lock;

void *sleepRand(void* vptr)
{
    stuff_t *stuff = (stuff_t*)vptr;
    int randTime = rand() % 8 + 1;

    pthread_mutex_lock(&lock); //lock so other threads dont interfere
    FILE *file = fopen("results.txt", "a");
    fprintf(file, "Thread id %p beginning\n", pthread_self()); 
    fflush(file);
    fclose(file);
    pthread_mutex_unlock(&lock); //unlock

    sleep(randTime);

    pthread_mutex_lock(&lock);
    fopen("results.txt", "a");
    fprintf(file, "Thread id %p exiting\n", pthread_self()); 
    fflush(file);
    fclose(file);
    pthread_mutex_unlock(&lock);

    return NULL;    
}

void pattern1(int things)
{
    pthread_t threads[things];
    pthread_mutex_init(&lock, NULL); 

    FILE *file = fopen("results.txt", "a");
    fprintf(file, "Pattern 1:\n");
    fflush(file);
    fclose(file);

    for (int i = 0; i < things; i++)
    {
        pthread_create(&threads[i], NULL, sleepRand, NULL); //create new threads with sleepRand, creates all the threads at once
    }

    for (int i = 0; i < things; i++) {
        pthread_join(threads[i], NULL); //wait for threads to finish
    }

}

void pattern2(int things)
{
    pthread_t threads[things];
    pthread_mutex_init(&lock, NULL);

    FILE *file = fopen("results.txt", "a");
    fprintf(file, "Pattern 2:\n");
    fflush(file);
    fclose(file);

    for (int i = 0; i < things; i++)
    {
        pthread_create(&threads[i], NULL, sleepRand, NULL); //create new threads w/ sleepRand, creates then stops over and over till done
        pthread_join(threads[i], NULL); //wait for threads to finish
    }

}


int main(int argc, char *argv[]) {

   int things = 0;
   int patternNumber = 0;

   sscanf(argv[1], "%d", &things); 
   sscanf(argv[2], "%d", &patternNumber);

    srand(time(NULL));

    if (patternNumber == 1)
    {
        pattern1(things);
    }
    else if(patternNumber == 2)
    {
        pattern2(things);
    }

return 0;
}