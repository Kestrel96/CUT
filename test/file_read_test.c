#include <assert.h>

#include "reader.h"


int main(void)
{

    printf("Check if stat file is read correctly.");
    cpu_data data;
    extract_cpu_data(&data);

    cpu_data *ptr = &data;
    unsigned int counter = 0;
    while (ptr->cpu != NULL)
    {

        assert(ptr->user == counter);
        assert(ptr->nice == counter);
        assert(ptr->system == counter);
        assert(ptr->idle == counter);
        assert(ptr->iowait == counter);
        assert(ptr->irq == counter);
        assert(ptr->softirq == counter);
        assert(ptr->steal == counter);
        assert(ptr->guest == counter);
        assert(ptr->guest_nice == counter);

        ptr = ptr->cpu;
        counter++;
    }

    return 0;
}