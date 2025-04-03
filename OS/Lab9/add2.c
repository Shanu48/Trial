#include <stdio.h>
#include <stdlib.h>

void fifoPageReplacement(int *pages, int numPages, int numFrames) {
    int *frames = (int *)malloc(numFrames * sizeof(int)); // Memory for frames
    int *time = (int *)malloc(numFrames * sizeof(int));   // Track insertion time
    int pageFaults = 0, currentTime = 0;

    // Initialize frames and time arrays
    for (int i = 0; i < numFrames; i++) {
        frames[i] = -1; // -1 indicates empty frame
        time[i] = -1;
    }

    printf("\nPage Reference\tFrames\n");

    for (int i = 0; i < numPages; i++) {
        int page = pages[i];
        int found = 0;

        // Check if the page is already in a frame
        for (int j = 0; j < numFrames; j++) {
            if (frames[j] == page) {
                found = 1;
                break;
            }
        }

        if (!found) { // Page fault occurs
            pageFaults++;

            // Find the oldest frame to replace (FIFO logic)
            int oldestIdx = 0;
            for (int j = 1; j < numFrames; j++) {
                if (time[j] < time[oldestIdx]) {
                    oldestIdx = j;
                }
            }

            frames[oldestIdx] = page;     // Replace with new page
            time[oldestIdx] = currentTime++; // Update insertion time
        }

        // Print current state of frames
        printf("%d\t\t", page);
        for (int j = 0; j < numFrames; j++) {
            if (frames[j] != -1) {
                printf("%d ", frames[j]);
            } else {
                printf("- ");
            }
        }
        printf("\n");
    }

    printf("\nTotal Page Faults: %d\n", pageFaults);

    free(frames); // Free dynamically allocated memory
    free(time);
}

int main() {
    int numPages, numFrames;

    printf("Enter the number of pages: ");
    scanf("%d", &numPages);

    int *pages = (int *)malloc(numPages * sizeof(int)); // Memory for pages array

    printf("Enter the page reference string:\n");
    for (int i = 0; i < numPages; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter the number of frames: ");
    scanf("%d", &numFrames);

    fifoPageReplacement(pages, numPages, numFrames);

    free(pages); // Free dynamically allocated memory

    return 0;
}
