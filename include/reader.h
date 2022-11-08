#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef struct s_cpu_data cpu_data;

struct s_cpu_data
{
    char name[6];
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
    unsigned long iowait;
    unsigned long irq;
    unsigned long softirq;
    unsigned long steal;
    unsigned long guest;
    unsigned long guest_nice;
    cpu_data *cpu;
};

void extract_cpu_data(cpu_data *data);
void print_cpu_data(cpu_data *data);
void print_cpu_data_single(cpu_data *data,int cpu_no);
void free_data_memory(cpu_data* data);
#endif /*READER_H*/