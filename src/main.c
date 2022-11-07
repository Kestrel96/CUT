#include <stdio.h>

#include "reader.h"
#include "analyzer.h"




cpu_data current_data, previous_data;
cpu_usage usage;


int main()
{

    printf("CUT\n");
    print_cpu_data(&current_data);
    extract_cpu_data(&current_data);
    print_cpu_data(&current_data);

    init(&current_data,&previous_data,&usage);
    print_cpu_data(&previous_data);



    return 0;
}