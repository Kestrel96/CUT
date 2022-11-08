#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "reader.h"
#include "analyzer.h"







typedef struct arguments arguments;
struct arguments
{
    cpu_data *current_data;
    cpu_data *previous_data;
    cpu_usage *usage;
};


void watchdog_thread();
void reader_thread(arguments *args);
void analyzer_thread(arguments *args);
void printer_thread(arguments *args);