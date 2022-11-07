#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef struct s_cpu_data cpu_data;

struct s_cpu_data
{
    char name[6];
    int user;
    int nice;
    int system;
    int idle;
    int iowait;
    int irq;
    int softirq;
    int steal;
    int guest;
    int guest_nice;
    cpu_data *cpu;
};

void extract_cpu_data(cpu_data *data);
void print_cpu_data(cpu_data *data);
void print_cpu_data_single(cpu_data *data);
void free_data_memory(cpu_data* data);
#endif /*READER_H*/