#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t mutexes[NUM_PHILOSOPHERS]; // Mutexes for each fork

// Function to simulate philosopher's thinking and eating process
void* philosopher(void* num) {
    int id = *((int*) num);

    while (1) {
        printf("Philosopher %d is thinking.\n", id);
        usleep(rand() % 1000); // Simulate thinking time

        // Pick up the left fork
        pthread_mutex_lock(&mutexes[id]);

        // Pick up the right fork
        pthread_mutex_lock(&mutexes[(id + 1) % NUM_PHILOSOPHERS]);

        // Start eating
        printf("Philosopher %d is eating.\n", id);
        usleep(rand() % 1000); // Simulate eating time

        // Put down the right fork
        pthread_mutex_unlock(&mutexes[(id + 1) % NUM_PHILOSOPHERS]);

        // Put down the left fork
        pthread_mutex_unlock(&mutexes[id]);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize mutexes for each fork
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
    }

    // Create threads for each philosopher
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    // Wait for all philosophers to finish (in this case, they'll run indefinitely)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy mutexes (this will never happen as the philosophers run indefinitely)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }

    return 0;
}
