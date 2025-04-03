#include <stdio.h>
#include <stdlib.h>

void worstFit(int *blockSizes, int numBlocks, int *processSizes, int numProcesses) {
    // Array to store the block assigned to each process (-1 means not allocated)
    int *allocation = (int *)malloc(numProcesses * sizeof(int));
    for (int i = 0; i < numProcesses; i++) {
        allocation[i] = -1; // Initialize all processes as not allocated
    }

    // Worst Fit Allocation
    for (int i = 0; i < numProcesses; i++) {
        int worstIdx = -1;
        for (int j = 0; j < numBlocks; j++) {
            if (blockSizes[j] >= processSizes[i]) {
                if (worstIdx == -1 || blockSizes[j] > blockSizes[worstIdx]) {
                    worstIdx = j;
                }
            }
        }

        // If a suitable block is found
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockSizes[worstIdx] -= processSizes[i]; // Reduce the block size
        }
    }

    // Output allocation results
    printf("\nProcess No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("%d\t\t%d\t\t", i + 1, processSizes[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i] + 1);
        } else {
            printf("Not Allocated\n");
        }
    }

    free(allocation); // Free dynamically allocated memory
}

int main() {
    int numBlocks, numProcesses;

    // Input number of blocks and their sizes
    printf("Enter the number of memory blocks: ");
    scanf("%d", &numBlocks);
    int *blockSizes = (int *)malloc(numBlocks * sizeof(int));
    printf("Enter the sizes of the memory blocks:\n");
    for (int i = 0; i < numBlocks; i++) {
        scanf("%d", &blockSizes[i]);
    }

    // Input number of processes and their sizes
    printf("Enter the number of processes: ");
    scanf("%d", &numProcesses);
    int *processSizes = (int *)malloc(numProcesses * sizeof(int));
    printf("Enter the sizes of the processes:\n");
    for (int i = 0; i < numProcesses; i++) {
        scanf("%d", &processSizes[i]);
    }

    // Call Worst Fit function
    worstFit(blockSizes, numBlocks, processSizes, numProcesses);

    // Free dynamically allocated memory
    free(blockSizes);
    free(processSizes);

    return 0;
}
