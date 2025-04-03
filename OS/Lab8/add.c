#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int allocation[MAX_PROCESSES][MAX_RESOURCES];
int request[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];
bool finish[MAX_PROCESSES]; // Tracks processes that can complete

int num_processes, num_resources;

void initialize() {
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    printf("Enter the number of resources: ");
    scanf("%d", &num_resources);

    printf("Enter the allocation matrix:\n");
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    printf("Enter the request matrix:\n");
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &request[i][j]);
        }
    }

    printf("Enter the available resources vector:\n");
    for (int i = 0; i < num_resources; i++) {
        scanf("%d", &available[i]);
    }

    // Initialize finish array
    for (int i = 0; i < num_processes; i++) {
        finish[i] = false;
    }
}

bool is_deadlock() {
    int work[MAX_RESOURCES];
    for (int i = 0; i < num_resources; i++) {
        work[i] = available[i];
    }

    bool progress;
    do {
        progress = false;
        for (int i = 0; i < num_processes; i++) {
            if (!finish[i]) {
                bool can_allocate = true;
                for (int j = 0; j < num_resources; j++) {
                    if (request[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                if (can_allocate) {
                    // Mark process as finished and update work vector
                    finish[i] = true;
                    progress = true;
                    for (int j = 0; j < num_resources; j++) {
                        work[j] += allocation[i][j];
                    }
                }
            }
        }
    } while (progress);

    // Check if any process is still unfinished
    for (int i = 0; i < num_processes; i++) {
        if (!finish[i]) {
            return true; // Deadlock detected
        }
    }

    return false; // No deadlock
}

int main() {
    initialize();

    if (is_deadlock()) {
        printf("Deadlock detected!\n");
    } else {
        printf("No deadlock detected.\n");
    }

    return 0;
}
