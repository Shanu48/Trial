#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Function to simulate FIFO page replacement
void fifo(int* pages, int num_pages, int num_frames) {
    int* frames = (int*)malloc(num_frames * sizeof(int)); // Frames to store pages
    int page_faults = 0, index = 0;

    // Initialize frames with -1 to indicate empty
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
    }

    printf("FIFO Page Replacement Algorithm:\n");

    for (int i = 0; i < num_pages; i++) {
        int page = pages[i];
        int page_found = 0;

        // Check if page is already in frames
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                page_found = 1;
                break;
            }
        }

        // If page is not found, it's a page fault
        if (!page_found) {
            // Replace the oldest page (FIFO)
            frames[index] = page;
            index = (index + 1) % num_frames; // Circular increment
            page_faults++;
            printf("Page Fault: %d\n", page);
        }

        // Print the current frames
        printf("Current Frames: ");
        for (int j = 0; j < num_frames; j++) {
            printf("%d ", frames[j]);
        }
        printf("\n");
    }

    printf("Total Page Faults: %d\n", page_faults);

    // Free allocated memory
    free(frames);
}

// Function to simulate Optimal page replacement
void optimal(int* pages, int num_pages, int num_frames) {
    int* frames = (int*)malloc(num_frames * sizeof(int)); // Frames to store pages
    int page_faults = 0;

    // Initialize frames with -1 to indicate empty
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
    }

    printf("\nOptimal Page Replacement Algorithm:\n");

    for (int i = 0; i < num_pages; i++) {
        int page = pages[i];
        int page_found = 0;

        // Check if page is already in frames
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                page_found = 1;
                break;
            }
        }

        // If page is not found, it's a page fault
        if (!page_found) {
            // Find the page that will not be used for the longest period of time
            int farthest = -1;
            int replace_index = -1;
            for (int j = 0; j < num_frames; j++) {
                int next_use = -1;

                // Check for future use of this page
                for (int k = i + 1; k < num_pages; k++) {
                    if (pages[k] == frames[j]) {
                        next_use = k;
                        break;
                    }
                }

                // If the page is not used anymore, replace it
                if (next_use == -1) {
                    replace_index = j;
                    break;
                }

                // Find the page that is used farthest in the future
                if (next_use > farthest) {
                    farthest = next_use;
                    replace_index = j;
                }
            }

            // Replace the selected page
            frames[replace_index] = page;
            page_faults++;
            printf("Page Fault: %d\n", page);
        }

        // Print the current frames
        printf("Current Frames: ");
        for (int j = 0; j < num_frames; j++) {
            printf("%d ", frames[j]);
        }
        printf("\n");
    }

    printf("Total Page Faults: %d\n", page_faults);

    // Free allocated memory
    free(frames);
}

int main() {
    int num_frames, num_pages;

    // Get the number of frames and the page reference string from the user
    printf("Enter the number of frames: ");
    scanf("%d", &num_frames);

    printf("Enter the number of pages: ");
    scanf("%d", &num_pages);

    int* pages = (int*)malloc(num_pages * sizeof(int));

    printf("Enter the page reference string: ");
    for (int i = 0; i < num_pages; i++) {
        scanf("%d", &pages[i]);
    }

    // Call FIFO algorithm
    fifo(pages, num_pages, num_frames);

    // Call Optimal algorithm
    optimal(pages, num_pages, num_frames);

    // Free the allocated memory for pages
    free(pages);

    return 0;
}
