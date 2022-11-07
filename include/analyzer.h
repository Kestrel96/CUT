#ifndef ANALYZER_H
#define ANALYZER_H
#include "reader.h"
#include "stdbool.h"

typedef struct s_cpu_usage cpu_usage;

struct s_cpu_usage
{

    unsigned cpu_no;
    float usage;
    cpu_usage *next_cpu;
};

void get_usage(cpu_data *data, cpu_usage *usage, cpu_usage *previous_usage);
void init(cpu_data *data, cpu_data *previous_data, cpu_usage *current_usage);
#endif /*ANALYZER_H*/