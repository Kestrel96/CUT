#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#include "reader.h"
#include "analyzer.h"

volatile sig_atomic_t stop = 0;

cpu_data current_data, previous_data;
cpu_usage usage;

pthread_mutex_t lock;

sem_t read_semaphore, analyze_semaphore, print_semaphore;

typedef struct arguments arguments;
struct arguments
{
    cpu_data *current_data;
    cpu_data *previous_data;
    cpu_usage *usage;
};

typedef struct thread_helper;

bool updated = false;
bool printed = true;

void memory_cleanup()
{

    free_data_memory(&current_data);
    free_data_memory(&previous_data);
    free_usage_memory(&usage);
}

void term(int signum)
{
    stop = 1;
    printf("SIGTERM received!\n");
}

void *reader_thread(arguments *args)
{

    while (1)
    {
        printf("reader\n");
        if (stop == 1)
        {
            sem_post(&analyze_semaphore);
            break;
        }
        sem_wait(&read_semaphore);

        pthread_mutex_lock(&lock);

        extract_cpu_data(args->current_data);
        pthread_mutex_unlock(&lock);
        sem_post(&analyze_semaphore);
    }
}

void *analyzer_thread(arguments *args)
{

    while (1)
    {
        printf("Analyzer \n");
        if (stop == 1)
        {
            sem_post(&print_semaphore);
            break;
        }
        sem_wait(&analyze_semaphore);

        pthread_mutex_lock(&lock);

        get_current_usage(args->current_data, args->previous_data, args->usage);
        pthread_mutex_unlock(&lock);
        sem_post(&print_semaphore);
    }
}

void *printer_thread(arguments *args)
{

    while (1)
    {

        if (stop == 1)
        {
            sem_post(&read_semaphore);
            break;
        }
        sem_wait(&print_semaphore);
        pthread_mutex_lock(&lock);

        print_usage(args->usage);
        printed = true;
        pthread_mutex_unlock(&lock);
        sleep(1);
        sem_post(&read_semaphore);
    }
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

    extract_cpu_data(&current_data);
    init(&current_data, &previous_data, &usage);

    free_usage_memory(&usage);

    pthread_t reader, analyzer, printer;

    sem_init(&read_semaphore, 0, 1);
    sem_init(&analyze_semaphore, 0, 0);
    sem_init(&print_semaphore, 0, 0);

    pthread_create(&analyzer, NULL, analyzer_thread, (void *)&args);
    pthread_create(&printer, NULL, printer_thread, (void *)&args);
    pthread_create(&reader, NULL, reader_thread, (void *)&args);

    while (1)
    {
        sleep(5);
        if (stop == 1)
        {
            printf("Waiting for threads to finish...\n");
            sem_post(&print_semaphore);
            pthread_join(&printer, NULL);
            sem_post(&read_semaphore);
            pthread_join(&reader, NULL);
            sem_post(&analyze_semaphore);
            pthread_join(&analyzer, NULL);
            printf("Threads finished...\n");
            sem_destroy(&read_semaphore);
            sem_destroy(&analyze_semaphore);
            sem_destroy(&print_semaphore);
            printf("Semaphores destroyed...\n");
            memory_cleanup();
            printf("Memory freed...\n");
            exit(0);
        }
    }

    return 0;
}