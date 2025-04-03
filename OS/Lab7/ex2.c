#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

// Shared resource
int shared_resource = 0;  // The shared data that readers and writers access

// Synchronization mechanisms
sem_t mutex;       // To control access to the number of readers
sem_t write_lock;  // To control exclusive access for writers
int read_count = 0; // Number of readers currently accessing the resource

// Reader thread function
void* reader(void* arg) {
    int id = *(int*)arg;

    while (1) {
        // Start reading
        sem_wait(&mutex);  // Enter critical section to update read_count
        read_count++;
        if (read_count == 1) {
            sem_wait(&write_lock); // First reader blocks the writer
        }
        sem_post(&mutex);  // Exit critical section

        // Reading the shared resource
        printf("Reader %d is reading shared resource: %d\n", id, shared_resource);

        sem_wait(&mutex);  // Enter critical section to update read_count
        read_count--;
        if (read_count == 0) {
            sem_post(&write_lock); // Last reader releases the writer lock
        }
        sem_post(&mutex);  // Exit critical section

        sleep(1);  // Simulate time taken for reading
    }

    return NULL;
}

// Writer thread function
void* writer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        // Start writing
        sem_wait(&write_lock);  // Ensure exclusive access to the resource

        // Writing to the shared resource
        shared_resource++;
        printf("Writer %d is writing to shared resource: %d\n", id, shared_resource);

        sem_post(&write_lock);  // Release the write lock

        sleep(2);  // Simulate time taken for writing
    }

    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];

    // Initialize semaphores
    sem_init(&mutex, 0, 1);       // Mutex for protecting read_count
    sem_init(&write_lock, 0, 1);  // Semaphore for controlling writers

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Wait for threads to finish (they run indefinitely in this case)
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroy semaphores (never reached in this example since threads run indefinitely)
    sem_destroy(&mutex);
    sem_destroy(&write_lock);

    return 0;
}

