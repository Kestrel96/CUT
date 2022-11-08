#include "threads.h"

#define E_READER 0
#define E_ANALYZER 1
#define E_PRINTER 2

#define PRINT_INTERVAL 1
#define WATCHDOG_TIMEOUT 2
volatile sig_atomic_t stop = 0;

bool updated = false;
bool printed = true;


pthread_mutex_t lock, wlock;

sem_t read_semaphore, analyze_semaphore, print_semaphore, watchdog_semaphore;
int thread_id = -1;

void watchdog_thread()
{
    while (1)
    {
        if (stop == 1)
        {
            break;
        }
        struct timespec timeout;
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += WATCHDOG_TIMEOUT;
        if (sem_timedwait(&watchdog_semaphore, &timeout) != 0)
        {
            if (pthread_mutex_lock(&wlock) == 0)
            {
                printf("Watchdog interrupt, thread %d stuck!\n", thread_id);
            }
            else
            {
                printf("Thread stuck!\n");
            }
            exit(1);
        }
    }
}

void reader_thread(arguments *args)
{

    while (1)
    {

        if (stop == 1)
        {
            sem_post(&analyze_semaphore);
            break;
        }
        sem_wait(&read_semaphore);
        pthread_mutex_lock(&wlock);
        thread_id = E_READER;
        pthread_mutex_unlock(&wlock);
        sem_post(&watchdog_semaphore);

        pthread_mutex_lock(&lock);

        extract_cpu_data(args->current_data);
        pthread_mutex_unlock(&lock);
        sem_post(&analyze_semaphore);
    }
}

void analyzer_thread(arguments *args)
{

    while (1)
    {

        if (stop == 1)
        {
            sem_post(&print_semaphore);
            break;
        }
        sem_wait(&analyze_semaphore);
        pthread_mutex_lock(&wlock);
        thread_id = E_ANALYZER;
        pthread_mutex_unlock(&wlock);
        sem_post(&watchdog_semaphore);

        pthread_mutex_lock(&lock);

        get_current_usage(args->current_data, args->previous_data, args->usage);
        pthread_mutex_unlock(&lock);
        sem_post(&print_semaphore);
    }
}

void printer_thread(arguments *args)
{

    while (1)
    {

        if (stop == 1)
        {
            sem_post(&read_semaphore);
            break;
        }
        sem_wait(&print_semaphore);
        pthread_mutex_lock(&wlock);
        thread_id = E_PRINTER;
        pthread_mutex_unlock(&wlock);
        sem_post(&watchdog_semaphore);
        pthread_mutex_lock(&lock);

        print_usage(args->usage);
        printed = true;
        pthread_mutex_unlock(&lock);
        sleep(PRINT_INTERVAL);
        sem_post(&read_semaphore);
    }
}
