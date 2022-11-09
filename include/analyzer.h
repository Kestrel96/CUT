#ifndef ANALYZER_H
#define ANALYZER_H
#include "reader.h"
#include "stdbool.h"

typedef struct s_cpu_usage cpu_usage;

/**
 * @brief CPU usage struct. Contains core number, usage and pointer to next core.
 * 
 */
struct s_cpu_usage
{

    unsigned cpu_no;
    float usage;
    cpu_usage *next_cpu;
};

void get_usage(cpu_data *data, cpu_usage *usage, cpu_usage *previous_usage);
void init(cpu_data *data, cpu_data *previous_data, cpu_usage *usage);
void get_current_usage(cpu_data *data, cpu_data *previous_data, cpu_usage *current_usage);
void print_usage(cpu_usage *usage);
void free_usage_memory(cpu_usage *usage);

#endif /*ANALYZER_H*/