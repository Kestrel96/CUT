#include <stdlib.h>
#include <stdio.h>

#include "analyzer.h"

/**
 * @brief Initializes cpu_usage and previous_data structure based on data from parsed current data.
 * 
 * @param data Data read for the first time
 * @param previous_data Structure to be initalized
 * @param usage Usage structure to be initialized
 */
void init(cpu_data *data, cpu_data *previous_data, cpu_usage *usage)
{

    cpu_data *ptr = data;
    cpu_usage *u_ptr = usage;
    cpu_data *previous_ptr = previous_data;
    unsigned count = 0;
    while (ptr->cpu != NULL)
    {
        strcpy(previous_data->name, ptr->name);
        previous_data->user = 0;
        previous_data->nice = 0;
        previous_data->system = 0;
        previous_data->idle = 0;
        previous_data->iowait = 0;
        previous_data->irq = 0;
        previous_data->softirq = 0;
        previous_data->steal = 0;
        previous_data->guest = 0;
        previous_data->guest_nice = 0;
        previous_data->cpu = (cpu_data *)malloc(sizeof(cpu_data));

        usage->cpu_no = count;
        usage->usage = 0;
        usage->next_cpu = (cpu_usage *)malloc(sizeof(cpu_usage));
        count++;

        previous_data = previous_data->cpu;
        usage = usage->next_cpu;
        ptr = ptr->cpu;
    }
    previous_data->cpu = NULL;
    usage->next_cpu = NULL;

    previous_data = previous_ptr;
    usage = u_ptr;
}

/**
 * @brief Set current data to previous data (for calculating delta)
 * 
 * @param current Current data from /proc/stat
 * @param previous Structure for previous data
 */
void set_previous(cpu_data *current, cpu_data *previous)
{

    cpu_data *ptr = current;
    cpu_data *prev_ptr = previous;
    while (ptr->cpu != NULL)
    {

        memcpy(previous->name, ptr->name, 6);
        previous->user = ptr->user;
        previous->nice = ptr->nice;
        previous->system = ptr->system;
        previous->idle = ptr->idle;
        previous->iowait = ptr->iowait;
        previous->irq = ptr->irq;
        previous->softirq = ptr->softirq;
        previous->steal = ptr->steal;
        previous->guest = ptr->guest;
        previous->guest_nice = ptr->guest_nice;

        ptr = ptr->cpu;
        previous = previous->cpu;
    }
    previous = prev_ptr;
}

/**
 * @brief Calculates usage based on previous and current data from /proc/stat
 * 
 * @param data Current data
 * @param previous_data Previous proc data
 * @param current_usage Usage struct
 */
void extract_usage(cpu_data *data, cpu_data *previous_data, cpu_usage *current_usage)
{

    cpu_data *ptr = data;
    cpu_data *prev_ptr = previous_data;

    unsigned count = 0;
    while (data->cpu != NULL)
    {
        /*taken from propsed answer: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux*/
        int prev_idle = previous_data->idle + previous_data->iowait;
        int current_idle = data->idle + data->iowait;

        int prev_non_idle = previous_data->user + previous_data->nice + previous_data->system +
                            previous_data->irq + previous_data->softirq + previous_data->steal;

        int current_non_idle = data->user + data->nice + data->system +
                               data->irq + data->softirq + data->steal;

        int prev_total = prev_idle + prev_non_idle;
        int current_total = current_idle + current_non_idle;

        int d_total = current_total - prev_total;
        int d_idle = current_idle - prev_idle;

        if (d_total != 0)
        {
            current_usage->usage = (float)(d_total - d_idle) / d_total * 100;
        }
        else
        {
            current_usage->usage = 0;
        }

        current_usage->cpu_no = count;
        count++;

        data = data->cpu;
        previous_data = previous_data->cpu;
        current_usage = current_usage->next_cpu;
    }
    data = ptr;
    previous_data = prev_ptr;
}

/**
 * @brief Get current usage, set current data to previous. If necessary function initializes previous_data and current_usage lists.
 * 
 * @param data Current data
 * @param previous_data Previous data
 * @param current_usage Current data regarding CPU utilization
 */
void get_current_usage(cpu_data *data, cpu_data *previous_data, cpu_usage *current_usage)
{
    static bool first = true;

    if (first == true)
    {

        init(data, previous_data, current_usage);

        first = false;
        extract_usage(data, previous_data, current_usage);
        set_previous(data, previous_data);

        return;
    }

    extract_usage(data, previous_data, current_usage);
    set_previous(data, previous_data);
}

/**
 * @brief Prints foramtted CPU usage.
 * 
 * @param usage Usage struct
 */
void print_usage(cpu_usage *usage)
{
    cpu_usage *ptr = usage;
    printf("CPU  ");
    printf("Load: %0.2f%%\n", ptr->usage);
    ptr = ptr->next_cpu;

    while (ptr->next_cpu != NULL)
    {
        printf("CPU%u ", ptr->cpu_no);
        printf("Load: %0.2f%%\n", ptr->usage);
        ptr = ptr->next_cpu;
    }
    printf("\n");
}

/**
 * @brief Releases allocated memory
 * 
 * @param usage Usage struct (head of list)
 */
void free_usage_memory(cpu_usage *usage)
{

    usage = usage->next_cpu;
    cpu_usage *tmp = usage;
    while (tmp->next_cpu != NULL)
    {
        usage = usage->next_cpu;
        free(tmp);
        tmp = usage;
    }
}