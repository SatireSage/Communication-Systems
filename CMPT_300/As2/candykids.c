// print hello world
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    int factory_number;
    double creation_ts_ms;
} candy_t;

double current_time_in_ms(void)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Error: No arguments found. Please run script with arguments.\n");
        exit(1);
    }
    return 0;
}