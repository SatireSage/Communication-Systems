#include "bbuff.h"
#include "stats.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

static int factories, kids, seconds = 0;
static bool factory_stop_flag = false;

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

// Extraction function
void extract_arguments(int argc, char *argv[])
{
    // Check if the number of arguments is correct. Only 3 arguments are allowed.
    if (argc != 4)
    {
        printf("\033[1;31mError: Invalid Number of Arguments. Please run script again with only 3 arguments.\n\033[0m");
        exit(1);
    }
    int arguments[3];
    // Convert the arguments to integers and store them in an array.
    for (int i = 1; i < argc; i++)
        arguments[i - 1] = atoi(argv[i]);
    // Check if the arguments are positive non-zero integers.
    if (arguments[0] <= 0 || arguments[1] <= 0 || arguments[2] <= 0)
    {
        printf("\033[1;31mError: Invalid Arguments. Please run script again with only positive non-zero integers.\n\033[0m");
        exit(1);
    }
    else
    {
        // Assign the arguments to the global variables.
        factories = arguments[0];
        kids = arguments[1];
        seconds = arguments[2];
    }
}

// Candy-factory thread
void *candy_factory(void *factory_number)
{
    int factory_num = *((int *)factory_number);
    while (!factory_stop_flag)
    {
        // Pick a number of seconds which it will (later) wait. The number is randomly selected between 0 and 3 (inclusive).
        int factory_delay = rand() % 4;
        // Print a message about the candy production
        printf("Factory %d ships candy and waits %ds\n", factory_num, factory_delay);
        // Create a new candy
        candy_t *candy = malloc(sizeof(candy_t));
        if (candy == NULL)
        {
            printf("\033[1;31mError: Failed to allocate memory for candy object.\n\033[0m");
            exit(1);
        }
        // Set the factory number and creation timestamp
        candy->factory_number = factory_num;
        candy->creation_ts_ms = current_time_in_ms();
        // Insert the candy into the bounded buffer
        bbuff_blocking_insert(candy);
        // Record the time in the stats module
        stats_record_produced(factory_num);
        // Sleep for the number of seconds
        sleep(factory_delay);
    }
    printf("\033[1;34m");
    printf("Candy-factory %d done\n", factory_num);
    printf("\033[0m");
    pthread_exit(NULL);
}

// Kid thread
void *kid(void *kid_id)
{
    while (true)
    {
        // Consume a candy from the bounded buffer
        candy_t *consume_candy = bbuff_blocking_extract();
        // Record the time in the stats module
        double consumption_time = current_time_in_ms() - consume_candy->creation_ts_ms;
        stats_record_consumed(consume_candy->factory_number, consumption_time);
        // Free the memory of the candy
        free(consume_candy);
        // Sleep for the number of seconds
        sleep(rand() % 2);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // 1.  Extract arguments
    extract_arguments(argc, argv);
    // 2.  Initialization of modules
    srand(time(NULL));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    stats_init(factories);
    bbuff_init();
    // 3.  Launch candy-factory threads
    pthread_t factory_threads[factories];
    int factory_id[factories];
    for (int i = 0; i < factories; i++)
    {
        factory_id[i] = i;
        if (pthread_create(&factory_threads[i], &attr, candy_factory, &factory_id[i]) != 0)
        {
            printf("\033[1;31mError: Failed to create candy-factory thread.\n\033[0m");
            exit(1);
        }
    }
    // 4.  Launch kid threads
    pthread_t kid_threads[kids];
    for (int i = 0; i < kids; i++)
    {
        if (pthread_create(&kid_threads[i], &attr, kid, &i) != 0)
        {
            printf("\033[1;31mError: Failed to create kid thread.\n\033[0m");
            exit(1);
        }
    }
    // 5.  Wait for requested time
    for (int i = 0; i < seconds; i++)
    {
        sleep(1);
        printf("\033[1;32m");
        printf("Time %ds\n", i + 1);
        printf("\033[0m");
    }
    // 6.  Stop candy-factory threads
    factory_stop_flag = true;
    for (int i = 0; i < factories; i++)
    {
        if (pthread_join(factory_threads[i], NULL) != 0)
        {
            printf("\033[1;31mError: Failed to join candy-factory thread.\n\033[0m");
            exit(1);
        }
    }
    // 7.  Wait until there is no more candies
    while (!bbuff_is_empty())
    {
        printf("\033[1;34m");
        printf("Waiting for all candies to be consumed");
        printf("\n\033[0m");
        sleep(1);
    }
    // 8.  Stop kid threads
    for (int i = 0; i < kids; i++)
    {
        pthread_cancel(kid_threads[i]);
        if (pthread_join(kid_threads[i], NULL) != 0)
        {
            printf("\033[1;31mError: Failed to join kid thread.\n\033[0m");
            exit(1);
        }
    }
    // 9.  Print statistics
    stats_display();
    // 10. Cleanup any allocated memory
    stats_cleanup();
    bbuff_cleanup();
    return 0;
}