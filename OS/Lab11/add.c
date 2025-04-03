#include <stdio.h>
#include <stdlib.h>

void fcfs(int *requests, int numRequests, int head) {
    int totalSeekTime = 0;
    printf("\nFCFS Disk Scheduling:\n");
    printf("Order of servicing requests: %d", head);

    for (int i = 0; i < numRequests; i++) {
        totalSeekTime += abs(requests[i] - head);
        head = requests[i];
        printf(" -> %d", requests[i]);
    }

    printf("\nTotal Seek Time: %d\n", totalSeekTime);
}

void look(int *requests, int numRequests, int head) {
    int totalSeekTime = 0;
    int i, direction;

    // Sort the requests
    for (i = 0; i < numRequests - 1; i++) {
        for (int j = 0; j < numRequests - i - 1; j++) {
            if (requests[j] > requests[j + 1]) {
                int temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
        }
    }

    printf("\nEnter the direction of movement (1 for high, -1 for low): ");
    scanf("%d", &direction);

    printf("\nLOOK Disk Scheduling:\n");
    printf("Order of servicing requests: %d", head);

    if (direction == 1) { // Moving towards higher tracks
        for (i = 0; i < numRequests && requests[i] < head; i++);
        for (; i < numRequests; i++) {
            totalSeekTime += abs(requests[i] - head);
            head = requests[i];
            printf(" -> %d", head);
        }
        for (i--; i >= 0; i--) {
            totalSeekTime += abs(requests[i] - head);
            head = requests[i];
            printf(" -> %d", head);
        }
    } else { // Moving towards lower tracks
        for (i = numRequests - 1; i >= 0 && requests[i] > head; i--);
        for (; i >= 0; i--) {
            totalSeekTime += abs(requests[i] - head);
            head = requests[i];
            printf(" -> %d", head);
        }
        for (i++; i < numRequests; i++) {
            totalSeekTime += abs(requests[i] - head);
            head = requests[i];
            printf(" -> %d", head);
        }
    }

    printf("\nTotal Seek Time: %d\n", totalSeekTime);
}

int main() {
    int choice, numRequests, *requests, initialHead;

    printf("Enter the number of disk access requests: ");
    scanf("%d", &numRequests);

    requests = (int *)malloc(numRequests * sizeof(int));
    
    printf("Enter the request queue:\n");
    for (int i = 0; i < numRequests; i++) {
        scanf("%d", &requests[i]);
    }

    printf("Enter the initial position of the disk head: ");
    scanf("%d", &initialHead);

    do {
        printf("\nDisk Scheduling Algorithms Menu:\n");
        printf("1. FCFS\n");
        printf("2. LOOK\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fcfs(requests, numRequests, initialHead);
                break;
            case 2:
                look(requests, numRequests, initialHead);
                break;
            case 3:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
                break;
        }
    } while (choice != 3);

    free(requests); // Free dynamically allocated memory
    return 0;
}
