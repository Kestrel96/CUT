#include <stdio.h>
#include <string.h>


typedef struct s_cpu_data cpu_data;

struct s_cpu_data
{
    char name[6];
    unsigned user;
    unsigned nice;
    unsigned system;
    unsigned idle;
    unsigned iowait;
    unsigned irq;
    unsigned softirq;
    unsigned steal;
    unsigned guest;
    unsigned guest_nice;
    cpu_data* cpu;

};




void extract_cpu_data();

void print_cpu_data(cpu_data* data);