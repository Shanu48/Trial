#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>  // Added for the sleep function

#define MAX_BUFFER_SIZE 5  // Maximum size of the buffer

// Shared buffer
int buffer[MAX_BUFFER_SIZE];
int in = 0;  // Index for producing
int out = 0; // Index for consuming

// Semaphores
sem_t empty; // Number of empty slots in the buffer
sem_t full;  // Number of full slots in the buffer
sem_t mutex; // Mutex for mutual exclusion

// Producer function
void* producer(void* arg) {
    int item;
    while (1) {
        item = rand() % 100;  // Produce an item (random number)
        
        // Wait for an empty slot
        sem_wait(&empty);
        
        // Enter critical section
        sem_wait(&mutex);
        
        // Produce the item (insert into buffer)
        buffer[in] = item;
        printf("Produced: %d at index %d\n", item, in);
        
        in = (in + 1) % MAX_BUFFER_SIZE;  // Circular increment
        
        // Exit critical section
        sem_post(&mutex);
        
        // Signal that there is a new full slot
        sem_post(&full);
        
        sleep(1);  // Sleep for a while to simulate production time
    }
}

// Consumer function
void* consumer(void* arg) {
    int item;
    while (1) {
        // Wait for a full slot
        sem_wait(&full);
        
        // Enter critical section
        sem_wait(&mutex);
        
        // Consume an item (remove from buffer)
        item = buffer[out];
        printf("Consumed: %d from index %d\n", item, out);
        
        out = (out + 1) % MAX_BUFFER_SIZE;  // Circular increment
        
        // Exit critical section
        sem_post(&mutex);
        
        // Signal that there is a new empty slot
        sem_post(&empty);
        
        sleep(1);  // Sleep for a while to simulate consumption time
    }
}

int main() {
    // Initialize semaphores
    sem_init(&empty, 0, MAX_BUFFER_SIZE); // Initially, all buffer slots are empty
    sem_init(&full, 0, 0);                // Initially, there are no full slots
    sem_init(&mutex, 0, 1);               // Mutex initialized to 1 for mutual exclusion
    
    // Create producer and consumer threads
    pthread_t prod_thread, cons_thread;
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);
    
    // Wait for the threads to finish (they run indefinitely)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);
    
    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    
    return 0;
}

