#include "threads.h"

extern sem_t watchdog_semaphore;
extern pthread_mutex_t wlock;

#define E_TEST 4

extern int thread_id;
void test_thread()
{

    while (1)
    {
        static unsigned count = 0;
        /*lock resources, register running thread's ID*/
        pthread_mutex_lock(&wlock);
        thread_id = E_TEST;
        pthread_mutex_unlock(&wlock);
        sem_post(&watchdog_semaphore);

        printf("Count: %u\n", count);
        count++;
        if (count >= 5)
        {
            printf("Testing watchdog...\n");
            sleep(count);
        }
        sleep(1);
    }
}

int main(void)
{

    pthread_t watchdog, test;

    sem_init(&watchdog_semaphore, 0, 0);
    pthread_create(&watchdog, NULL, (void *)watchdog_thread, NULL);
    pthread_create(&test, NULL, (void *)test_thread, NULL);


    while(1){
        
    }

    return 0;
}