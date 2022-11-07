#include "analyzer.h"

void init(cpu_data *data, cpu_data *previous_data, cpu_usage *current_usage)
{

    cpu_data *ptr = data;
    cpu_data *previous_ptr = previous_data;
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
        previous_data = previous_data->cpu;
        ptr = ptr->cpu;
    }
    previous_data->cpu=NULL;

    previous_data = previous_ptr;
}

void get_current_usage(cpu_data *data, cpu_usage *current_usage, cpu_usage *previous_usage)
{
    static bool first = true;

    first = false;
}

void extract_usage(cpu_data *data, cpu_data *previous_data, cpu_usage *current_usage)
{

    cpu_data *ptr = data;
    cpu_data *prev_ptr = previous_data;

    unsigned count = 0;
    while (data->cpu != NULL)
    {

        unsigned prev_idle = previous_data->idle + previous_data->iowait;
        unsigned current_idle = data->idle + data->iowait;

        unsigned prev_non_idle = previous_data->user + previous_data->nice + previous_data->system +
                                 previous_data->irq + previous_data->softirq + previous_data->steal;

        unsigned current_non_idle = data->user + data->nice + data->system +
                                    data->irq + data->softirq + data->steal;

        unsigned prev_total = prev_idle + prev_non_idle;
        unsigned current_total = current_idle + current_non_idle;

        unsigned d_total = current_total - prev_total;
        unsigned d_idle = current_idle - prev_idle;

        current_usage->usage = (float)(d_total - d_idle) / d_total;
        current_usage = current_usage->next_cpu;
        current_usage->cpu_no = count;
        count++;
        data = data->cpu;
        previous_data = data->cpu;
    }
}
