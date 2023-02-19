#include "stats.h"

typedef struct
{
    int candies_produced; // Number of candies produced
    int candies_consumed; // Number of candies consumed
    // Store total delay in position 0, average in position 1, min in position 2, max in position 3.
    double delay_values[4]; // Array of delay values (in ms).
} factory_stats_t;

static factory_stats_t *factories;                        // Array of factories
static int num_factories;                                 // Number of factories
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex

void stats_init(int num_producers)
{
    pthread_mutex_init(&mutex, NULL);                            // Initialize mutex
    num_factories = num_producers;                               // Set number of factories
    factories = malloc(sizeof(factory_stats_t) * num_factories); // Allocate memory for factories
    // Initialize values for each factory
    for (int i = 0; i < num_factories; i++)
    {
        factories[i].candies_produced = 0; // Initialize candies produced to 0
        factories[i].candies_consumed = 0; // Initialize candies consumed to 0
        // Initialize delay values to 0
        for (int j = 0; j < 4; j++)
            factories[i].delay_values[j] = 0;
    }
}

void stats_cleanup(void)
{
    // free memory and prevent dangling pointers
    free(factories);
    factories = NULL;
    // Destroy mutex
    pthread_mutex_destroy(&mutex);
}

void stats_record_produced(int factory_number)
{
    // Lock mutex
    pthread_mutex_lock(&mutex);
    // Increment candies produced
    factories[factory_number].candies_produced++;
    // Unlock mutex
    pthread_mutex_unlock(&mutex);
}

void stats_record_consumed(int factory_number, double delay_in_ms)
{
    // Lock mutex
    pthread_mutex_lock(&mutex);
    // Increment candies consumed
    factories[factory_number].candies_consumed++;
    // Update delay values (total, average, min, max).
    // Update total delay
    factories[factory_number].delay_values[0] += delay_in_ms;
    // Update average delay
    factories[factory_number].delay_values[1] = factories[factory_number].delay_values[0] / factories[factory_number].candies_consumed;
    // Update min delay
    if (factories[factory_number].delay_values[2] == 0 || factories[factory_number].delay_values[2] > delay_in_ms)
        factories[factory_number].delay_values[2] = delay_in_ms;
    // Update max delay
    if (factories[factory_number].delay_values[3] < delay_in_ms)
        factories[factory_number].delay_values[3] = delay_in_ms;
    // Unlock mutex
    pthread_mutex_unlock(&mutex);
}

void stats_display(void)
{
    // Print statistics (yellow heading)
    printf("\033[1;33m\nStatistics: \n");
    // Print header
    printf("%8s%10s%10s%20s%20s%20s\n", "Factory#", "#Made", "#Eaten", "Min Delay[ms]", "Avg Delay[ms]", "Max Delay[ms]");
    printf("\033[0m");
    // Print data for each factory
    for (int i = 0; i < num_factories; i++)
    {
        // Print statistics for each factory. Prints error if number of candies produced does not match number of candies consumed.
        printf("%8d%10d%10d%20.5f%20.5f%20.5f\n", i, factories[i].candies_produced, factories[i].candies_consumed, factories[i].delay_values[2], factories[i].delay_values[1], factories[i].delay_values[3]);
        if (factories[i].candies_produced != factories[i].candies_consumed)
        {
            printf("\033[1;31m");
            printf("ERROR: Mismatch between number made and eaten.\n");
            printf("\033[0m");
        }
    }
}