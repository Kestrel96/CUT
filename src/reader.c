#include <stdlib.h>

#include "reader.h"

#ifndef STAT_PATH
#define STAT_PATH "/proc/stat"
#endif

void extract_cpu_data(cpu_data *data)
{

    FILE *stat_file;
    stat_file = fopen(STAT_PATH, "r");
    if (stat_file == NULL)
    {
        fprintf(stderr, "%s", "Could not open stat file! \n");
        exit(1);
    }

    char *line;
    size_t buffer_size = 100;
    line = (char *)malloc(buffer_size * sizeof(char));

    cpu_data *core;

    /*CPU combined (first line of stat)*/
    getline(&line, &buffer_size, stat_file);
    int success = sscanf(line, "%s %u %u %u %u %u %u %u %u %u %u",
                         data->name, &data->user, &data->nice, &data->system, &data->idle,
                         &data->iowait, &data->irq, &data->softirq, &data->steal,
                         &data->guest, &data->guest_nice);
    if (success != 11)
    {
        printf("Error while parsing stat file!\n");
        exit(2);
    }

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
    data = data->cpu;
    cpu_data *tmp = data;
    while (tmp->cpu != NULL)
    {

        data = data->cpu;
        free(tmp);
        tmp = data;
    }
}

void print_cpu_data_single(cpu_data *data,int cpu_no)
{

    cpu_data* ptr=data;
    cpu_no=cpu_no-1;

    for(int i=0;i<cpu_no;i++){
        if(ptr->cpu==NULL){
            break;
        }
        ptr=ptr->cpu;
    }
    printf("%s ", ptr->name);
    printf("%u ", ptr->user);
    printf("%u ", ptr->nice);
    printf("%u ", ptr->system);
    printf("%u ", ptr->idle);
    printf("%u ", ptr->iowait);
    printf("%u ", ptr->irq);
    printf("%u ", ptr->softirq);
    printf("%u ", ptr->steal);
    printf("%u ", ptr->guest);
    printf("%u \n", ptr->guest_nice);
}

void print_cpu_data(cpu_data *data)
{

    cpu_data *ptr = data;
    while (ptr->cpu != NULL)
    {
        printf("%s ", ptr->name);
        printf("%u ", ptr->user);
        printf("%u ", ptr->nice);
        printf("%u ", ptr->system);
        printf("%u ", ptr->idle);
        printf("%u ", ptr->iowait);
        printf("%u ", ptr->irq);
        printf("%u ", ptr->softirq);
        printf("%u ", ptr->steal);
        printf("%u ", ptr->guest);
        printf("%u \n", ptr->guest_nice);
        //printf("Ptr: 0x%08x\n", (unsigned)ptr->cpu);
        ptr = ptr->cpu;
    }
}