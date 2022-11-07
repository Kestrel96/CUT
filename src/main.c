#include <stdio.h>

#include "reader.h"

int main()
{

    printf("CUT\n");
    extract_cpu_data();
    // printf("CUT after\n");
    
    // cpu_data cpu;

    // FILE *stat_file;

    // stat_file = fopen("test_stat", "r");
    // if (stat_file == NULL)
    // {
    //     fprintf(stderr, "%s", "Could not open file! \n");
    // }

    // char *line;
    // size_t buffer_size = 100;
    // line = (char *)malloc(buffer_size * sizeof(char));
    // getline(&line, &buffer_size, stat_file);

    // char *buffer = "cpu  1436925 3421 702440 337023946 24231 0 79232 0 0 0";
    // char store_value[10];
    // int total_read;

    // printf("%s\n",line);
    // total_read = sscanf(line, "%s %u", cpu.name,&cpu.user);

    // printf("Value in buffer: %s %u", cpu.name,cpu.user);
    // printf("\nTotal items read: %d", total_read);

    return 0;
}