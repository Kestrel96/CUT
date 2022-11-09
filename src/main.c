#include <stdio.h>
#include <signal.h>

#include "threads.h"
#include "reader.h"
#include "analyzer.h"

extern volatile sig_atomic_t stop;
extern bool updated;
extern sem_t read_semaphore, analyze_semaphore, print_semaphore, watchdog_semaphore;
extern pthread_mutex_t lock, wlock;

cpu_data current_data, previous_data;
cpu_usage usage;

void memory_cleanup()
{

    free_data_memory(&current_data);
    free_data_memory(&previous_data);
    free_usage_memory(&usage);
}

void term()
{
    stop = 1;
}

int main()
{

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    arguments args;
    args.current_data = &current_data;
    args.previous_data = &previous_data;
    args.usage = &usage;
    updated = false;

    pthread_t reader, analyzer, printer, watchdog;

    sem_init(&read_semaphore, 0, 1);
    sem_init(&analyze_semaphore, 0, 0);
    sem_init(&print_semaphore, 0, 0);
    sem_init(&watchdog_semaphore, 0, 0);

    pthread_create(&analyzer, NULL, (void *)analyzer_thread, (void *)&args);
    pthread_create(&printer, NULL, (void *)printer_thread, (void *)&args);
    pthread_create(&reader, NULL, (void *)reader_thread, (void *)&args);
    pthread_create(&watchdog, NULL, (void *)watchdog_thread, NULL);

    while (1)
    {
        sleep(5);
        if (stop == 1)
        {
            printf("SIGTERM received!\n");
            printf("Waiting for threads to finish...\n");
            sem_post(&print_semaphore);
            pthread_join(printer, NULL);
            sem_post(&read_semaphore);
            pthread_join(reader, NULL);
            sem_post(&analyze_semaphore);
            pthread_join(analyzer, NULL);
            printf("Threads finished...\n");
            sem_destroy(&read_semaphore);
            sem_destroy(&analyze_semaphore);
            sem_destroy(&print_semaphore);
            pthread_mutex_destroy(&lock);
            pthread_mutex_destroy(&wlock);
            printf("Semaphores and mutexes destroyed...\n");
            memory_cleanup();
            printf("Memory freed...\n");
            exit(0);
        }
    }

    return 0;
}