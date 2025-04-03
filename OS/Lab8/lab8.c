#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3

// Function to calculate the Need matrix (Maximum - Allocation)
void calculateNeed(int need[MAX_PROCESSES][MAX_RESOURCES], int maximum[MAX_PROCESSES][MAX_RESOURCES], int allocation[MAX_PROCESSES][MAX_RESOURCES]) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

// Safety Algorithm to check if the system is in a safe state
bool safetyAlgorithm(int available[MAX_RESOURCES], int maximum[MAX_PROCESSES][MAX_RESOURCES], 
                     int allocation[MAX_PROCESSES][MAX_RESOURCES], int need[MAX_PROCESSES][MAX_RESOURCES]) {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    int safeSequence[MAX_PROCESSES];
    int count = 0;

    // Initialize work (available resources)
    for (int i = 0; i < MAX_RESOURCES; i++) {
        work[i] = available[i];
    }

    while (count < MAX_PROCESSES) {
        bool progressMade = false;

        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (!finish[i]) {
                bool canFinish = true;

                // Check if the process can finish
                for (int j = 0; j < MAX_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        canFinish = false;
                        break;
                    }
                }

                if (canFinish) {
                    // If the process can finish, simulate finishing it
                    safeSequence[count++] = i;
                    finish[i] = true;

                    // Add the allocated resources back to work
                    for (int j = 0; j < MAX_RESOURCES; j++) {
                        work[j] += allocation[i][j];
                    }

                    progressMade = true;
                    break;
                }
            }
        }

        if (!progressMade) {
            // If no progress is made, the system is in an unsafe state
            return false;
        }
    }

    // Print the safe sequence if the system is in a safe state
    printf("System is in a safe state.\nSafe sequence: ");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        printf("P%d ", safeSequence[i]);
    }
    printf("\n");

    return true;
}

// Resource Request Algorithm to check if the request can be granted
bool resourceRequestAlgorithm(int available[MAX_RESOURCES], int maximum[MAX_PROCESSES][MAX_RESOURCES],
                               int allocation[MAX_PROCESSES][MAX_RESOURCES], int need[MAX_PROCESSES][MAX_RESOURCES], 
                               int processID, int request[MAX_RESOURCES]) {
    // Check if request is less than or equal to need and available resources
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (request[i] > need[processID][i]) {
            printf("Error: Process has requested more than needed.\n");
            return false;
        }
        if (request[i] > available[i]) {
            printf("Error: Not enough resources available.\n");
            return false;
        }
    }

    // Simulate the allocation of requested resources
    for (int i = 0; i < MAX_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[processID][i] += request[i];
        need[processID][i] -= request[i];
    }

    // Check if the system is still in a safe state after the allocation
    return safetyAlgorithm(available, maximum, allocation, need);
}

int main() {
    int available[MAX_RESOURCES];
    int maximum[MAX_PROCESSES][MAX_RESOURCES];
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int request[MAX_RESOURCES];
    int processID;

    // Taking user input for available resources
    printf("Enter the number of available resources (A B C): ");
    for (int i = 0; i < MAX_RESOURCES; i++) {
        scanf("%d", &available[i]);
    }

    // Taking user input for maximum demand matrix
    printf("\nEnter the maximum demand for each process (row by row, each process has 3 values):\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        printf("For Process P%d: ", i);
        for (int j = 0; j < MAX_RESOURCES; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }

    // Taking user input for allocation matrix
    printf("\nEnter the current allocation for each process (row by row, each process has 3 values):\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        printf("For Process P%d: ", i);
        for (int j = 0; j < MAX_RESOURCES; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    // Calculate the need matrix
    calculateNeed(need, maximum, allocation);

    // Check if the system is in a safe state
    if (!safetyAlgorithm(available, maximum, allocation, need)) {
        printf("System is not in a safe state.\n");
    }

    // Request resources from the user
    printf("\nEnter the process ID for resource request (0-4): ");
    scanf("%d", &processID);

    printf("Enter the request for resources (A B C): ");
    for (int i = 0; i < MAX_RESOURCES; i++) {
        scanf("%d", &request[i]);
    }

    // Check if the request can be granted
    if (resourceRequestAlgorithm(available, maximum, allocation, need, processID, request)) {
        printf("Request by Process P%d granted.\n", processID);
    } else {
        printf("Request by Process P%d denied.\n", processID);
    }

    return 0;
}

