#include <stdio.h>
#include <stdlib.h>

// Comparator function for sorting
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void sstf(int arr[], int n, int head) {
    int seek_count = 0, distance, cur_track;
    int completed = 0;
    int visited[n];
    for (int i = 0; i < n; i++) visited[i] = 0;

    printf("\nSSTF Order of Servicing Requests: %d", head);

    while (completed < n) {
        int min_dist = 10000, min_index = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                distance = abs(arr[i] - head);
                if (distance < min_dist) {
                    min_dist = distance;
                    min_index = i;
                }
            }
        }
        visited[min_index] = 1;
        seek_count += min_dist;
        head = arr[min_index];
        completed++;
        printf(" -> %d", head);
    }
    printf("\nTotal Seek Count: %d\n", seek_count);
}

void scan(int arr[], int n, int head, int total_tracks) {
    int seek_count = 0;
    int left[n], right[n];
    int left_count = 0, right_count = 0;

    // Separate requests into left and right of the head position
    for (int i = 0; i < n; i++) {
        if (arr[i] < head) left[left_count++] = arr[i];
        else right[right_count++] = arr[i];
    }

    // Sort the left and right arrays
    qsort(left, left_count, sizeof(int), compare);
    qsort(right, right_count, sizeof(int), compare);

    printf("\nSCAN Order of Servicing Requests: %d", head);

    // Move towards leftmost track first
    for (int i = left_count - 1; i >= 0; i--) {
        seek_count += abs(head - left[i]);
        head = left[i];
        printf(" -> %d", head);
    }

    // Then move towards rightmost track
    for (int i = 0; i < right_count; i++) {
        seek_count += abs(head - right[i]);
        head = right[i];
        printf(" -> %d", head);
    }

    printf("\nTotal Seek Count: %d\n", seek_count);
}

void cscan(int arr[], int n, int head, int total_tracks) {
    int seek_count = 0;
    int left[n], right[n];
    int left_count = 0, right_count = 0;

    // Separate requests into left and right of the head position
    for (int i = 0; i < n; i++) {
        if (arr[i] < head) left[left_count++] = arr[i];
        else right[right_count++] = arr[i];
    }

    // Sort the left and right arrays
    qsort(left, left_count, sizeof(int), compare);
    qsort(right, right_count, sizeof(int), compare);

    printf("\nC-SCAN Order of Servicing Requests: %d", head);

    // Move towards rightmost track first
    for (int i = 0; i < right_count; i++) {
        seek_count += abs(head - right[i]);
        head = right[i];
        printf(" -> %d", head);
    }

    // Jump to start of disk
    seek_count += abs(total_tracks - head);
    head = 0;

    // Process remaining requests on the left side
    for (int i = 0; i < left_count; i++) {
        seek_count += abs(head - left[i]);
        head = left[i];
        printf(" -> %d", head);
    }

    printf("\nTotal Seek Count: %d\n", seek_count);
}

void clook(int arr[], int n, int head) {
    int seek_count = 0;
    int left[n], right[n];
    int left_count = 0, right_count = 0;

    // Separate requests into left and right of the head position
    for (int i = 0; i < n; i++) {
        if (arr[i] < head) left[left_count++] = arr[i];
        else right[right_count++] = arr[i];
    }

    // Sort the left and right arrays
    qsort(left, left_count, sizeof(int), compare);
    qsort(right, right_count, sizeof(int), compare);

    printf("\nC-LOOK Order of Servicing Requests: %d", head);

    // Move towards rightmost track first
    for (int i = 0; i < right_count; i++) {
        seek_count += abs(head - right[i]);
        head = right[i];
        printf(" -> %d", head);
    }

    // Jump to start of remaining requests on the left side
    seek_count += abs(head - left[0]);
    head = left[0];

    // Process remaining requests on the left side
    for (int i = 1; i < left_count; i++) {
        seek_count += abs(head - left[i]);
        head = left[i];
        printf(" -> %d", head);
    }

    printf("\nTotal Seek Count: %d\n", seek_count);
}

int main() {
    int arr[] = {98, 183, 41, 122, 14, 124, 65, 67};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    int head;

    printf("Enter the initial head position: ");
    scanf("%d", &head);

   printf("\nDisk Scheduling Algorithms Menu:\n");
   printf("1. Shortest Seek Time First (SSTF)\n");
   printf("2. SCAN\n");
   printf("3. C-SCAN\n");
   printf("4. C-LOOK\n");
   printf("5. Exit\n");

   int choice;
   while (1) {
       printf("\nEnter your choice: ");
       scanf("%d", &choice);

       switch (choice) {
           case 1:
               sstf(arr, n, head);
               break;
           case 2:
               scan(arr, n, head, 200); // Assuming total tracks = 200
               break;
           case 3:
               cscan(arr, n, head, 200); // Assuming total tracks = 200
               break;
           case 4:
               clook(arr, n, head);
               break;
           case 5:
               printf("Exiting the program.\n");
               return 0;
           default:
               printf("Invalid choice. Please try again.\n");
       }
   }

   return 0;
}
