#include <stdlib.h>

#include "reader.h"

void extract_cpu_data(cpu_data *data)
{

    FILE *stat_file;

    // stat_file = fopen("test_stat", "r");
    stat_file = fopen("/proc/stat", "r");
    if (stat_file == NULL)
    {
        fprintf(stderr, "%s", "Could not open file! \n");
    }

    char *line;
    size_t buffer_size = 100;
    line = (char *)malloc(buffer_size * sizeof(char));

    cpu_data *core, prev_core;

    /*CPU combined (first line of stat)*/
    getline(&line, &buffer_size, stat_file);
    int success = sscanf(line, "%s %u %u %u %u %u %u %u %u %u %u",
                         data->name, &data->user, &data->nice, &data->system, &data->idle,
                         &data->iowait, &data->irq, &data->softirq, &data->steal,
                         &data->guest, &data->guest_nice);

    /*individual cores*/
    core = (cpu_data *)malloc(sizeof(cpu_data));
    data->cpu = core;
    getline(&line, &buffer_size, stat_file);

    while (line[0] == 'c')
    {

        success = sscanf(line, "%s %u %u %u %u %u %u %u %u %u %u",
                         core->name, &core->user, &core->nice, &core->system, &core->idle,
                         &core->iowait, &core->irq, &core->softirq, &core->steal,
                         &core->guest, &core->guest_nice);

        cpu_data *tmp = (cpu_data *)malloc(sizeof(cpu_data));
        core->cpu = tmp;
        core = tmp;
        getline(&line, &buffer_size, stat_file);
    }
    core->cpu = NULL;
    fclose(stat_file);
}

void free_data_memory(cpu_data *data)
{   
    data=data->cpu;
    cpu_data *tmp = data;
    while (tmp->cpu != NULL)
    {

        data = data->cpu;
        free(tmp);
        tmp=data;
        
    }
}

void print_cpu_data_single(cpu_data *data)
{

    printf("Name: %s\n", data->name);
    printf("User: %u\n", data->user);
    printf("%u\n", data->nice);
    printf("%u\n", data->system);
    printf("%u\n", data->idle);
    printf("%u\n", data->iowait);
    printf("%u\n", data->irq);
    printf("%u\n", data->softirq);
    printf("%u\n", data->steal);
    printf("%u\n", data->guest);
    printf("%u\n", data->guest_nice);
}

void print_cpu_data(cpu_data *data)
{

    cpu_data *ptr = data;
    while (ptr->cpu != NULL)
    {
        printf("-----------%s-------------\n", ptr->name);
        printf("User: %u\n", ptr->user);
        printf("%u\n", ptr->nice);
        printf("%u\n", ptr->system);
        printf("%u\n", ptr->idle);
        printf("%u\n", ptr->iowait);
        printf("%u\n", ptr->irq);
        printf("%u\n", ptr->softirq);
        printf("%u\n", ptr->steal);
        printf("%u\n", ptr->guest);
        printf("%u\n", ptr->guest_nice);
        printf("Ptr: 0x%08x\n", (unsigned)ptr->cpu);
        ptr = ptr->cpu;
    }
}