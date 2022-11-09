#include <stdlib.h>

#include "reader.h"

#ifndef STAT_PATH
#define STAT_PATH "/proc/stat"
#endif


/**
 * @brief Extracts cpu data from /proc/stat
 * 
 * @param data Structure holding a single core (designated CPU) data. This is the head of a list.
 */
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
    int success = sscanf(line, "%s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
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

        success = sscanf(line, "%s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
                         core->name, &core->user, &core->nice, &core->system, &core->idle,
                         &core->iowait, &core->irq, &core->softirq, &core->steal,
                         &core->guest, &core->guest_nice);

        if (success != 11)
        {
            printf("Error while parsing stat file!\n");
            exit(2);
        }

        cpu_data *tmp = (cpu_data *)malloc(sizeof(cpu_data));
        core->cpu = tmp;
        core = tmp;
        getline(&line, &buffer_size, stat_file);
    }
    core->cpu = NULL;
    fclose(stat_file);
}

/**
 * @brief Freeing the allocated memory of cpu_data
 * 
 * @param data Head of list that is to be freed.
 * @return * void 
 */
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

/**
 * @brief Prints usage info of single core. Debug function
 * 
 * @param data cpu_data struct
 * @param cpu_no Specific core number (0 for avg. CPU usage)
 */
void print_cpu_data_single(cpu_data *data, int cpu_no)
{

    cpu_data *ptr = data;
    cpu_no = cpu_no - 1;

    for (int i = 0; i < cpu_no; i++)
    {
        if (ptr->cpu == NULL)
        {
            break;
        }
        ptr = ptr->cpu;
    }
    printf("%s ", ptr->name);
    printf("%lu ", ptr->user);
    printf("%lu ", ptr->nice);
    printf("%lu ", ptr->system);
    printf("%lu ", ptr->idle);
    printf("%lu ", ptr->iowait);
    printf("%lu ", ptr->irq);
    printf("%lu ", ptr->softirq);
    printf("%lu ", ptr->steal);
    printf("%lu ", ptr->guest);
    printf("%lu \n", ptr->guest_nice);
}

/**
 * @brief Prints data for all cores.
 * 
 * @param data Head of cpu data list.
 */
void print_cpu_data(cpu_data *data)
{

    cpu_data *ptr = data;
    while (ptr->cpu != NULL)
    {
        printf("%s ", ptr->name);
        printf("%lu ", ptr->user);
        printf("%lu ", ptr->nice);
        printf("%lu ", ptr->system);
        printf("%lu ", ptr->idle);
        printf("%lu ", ptr->iowait);
        printf("%lu ", ptr->irq);
        printf("%lu ", ptr->softirq);
        printf("%lu ", ptr->steal);
        printf("%lu ", ptr->guest);
        printf("%lu \n", ptr->guest_nice);
        // printf("Ptr: 0x%08x\n", (unsigned)ptr->cpu);
        ptr = ptr->cpu;
    }
}