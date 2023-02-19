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
        printf("\033[1;31mError: No arguments found. Please run script with arguments.\n\033[0m");
        exit(1);
    }

    // 1.  Extract arguments
    // 2.  Initialization of modules
    // 3.  Launch candy-factory threads
    // 4.  Launch kid threads
    // 5.  Wait for requested time
    // 6.  Stop candy-factory threads
    // 7.  Wait until there is no more candies
    // 8.  Stop kid threads
    // 9.  Print statistics
    // 10. Cleanup any allocated memory

    return 0;
}