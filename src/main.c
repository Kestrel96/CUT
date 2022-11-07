#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "reader.h"
#include "analyzer.h"

#define UPDATE_INTERVAL_US 500000

cpu_data current_data, previous_data;
cpu_usage usage;
pthread_mutex_t lock;

sem_t read_semaphore, analyze_semaphore, print_semaphore,wait_semaphore;
pthread_cond_t update_condition, print_condition, update_condition;

typedef struct arguments arguments;
struct arguments
{
    cpu_data *current_data;
    cpu_data *previous_data;
    cpu_usage *usage;
};

bool updated = false;
bool printed = true;

void *reader_wrapper(arguments *args)
{

    while (1)
    {
        sem_wait(&read_semaphore);
        pthread_mutex_lock(&lock);
        printf("reader\n");



        extract_cpu_data(args->current_data);
        pthread_mutex_unlock(&lock);
        sem_post(&analyze_semaphore);
    }
}

void *analyzer_wrapper(arguments *args)
{

    while (1)
    {
        sem_wait(&analyze_semaphore);
        pthread_mutex_lock(&lock);
        printf("Analyzer \n");

        get_current_usage(args->current_data, args->previous_data, args->usage);
        pthread_mutex_unlock(&lock);
        sem_post(&print_semaphore);
    }
}

void *printer_thread(arguments *args)
{

    while (1)
    {
        sem_wait(&print_semaphore);
        pthread_mutex_lock(&lock);
        printf("printer\n------\n");
        while (updated == true)
        {
            printf("printer loop\n");
            pthread_cond_wait(&print_condition, &lock);
            break;
        }
        print_usage(args->usage);
        printed = true;
        pthread_cond_signal(&update_condition);
        pthread_mutex_unlock(&lock);

        sleep(1);
        sem_post(&read_semaphore);
    }
}

int main()
{

    arguments args;

    args.current_data = &current_data;
    args.previous_data = &previous_data;
    args.usage = &usage;
    updated = false;

    pthread_t reader, analyzer, printer;

    sem_init(&read_semaphore, 0, 1);
    sem_init(&analyze_semaphore, 0, 0);
    sem_init(&print_semaphore, 0, 0);
    sem_init(&wait_semaphore, 0, 0);

    pthread_create(&analyzer, NULL, analyzer_wrapper, (void *)&args);
    pthread_create(&printer, NULL, printer_thread, (void *)&args);
    pthread_create(&reader, NULL, reader_wrapper, (void *)&args);

    while (1)
    {
        sem_wait(&wait_semaphore);
    }

    return 0;
}