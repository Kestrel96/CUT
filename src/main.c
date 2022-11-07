#include <stdio.h>
#include <unistd.h>

#include "reader.h"
#include "analyzer.h"

cpu_data current_data, previous_data;
cpu_usage usage;

int main()
{

    printf("CUT\n");
    extract_cpu_data(&current_data);


    for (int i = 0; i < 500; i++)
    {
        get_current_usage(&current_data, &previous_data, &usage);
        extract_cpu_data(&current_data);
        print_usage(&usage);   
        sleep(1);
        
    }

    return 0;
}