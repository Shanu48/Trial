#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int page;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int size;
} LRUCache;

// Function to create an empty LRU cache
LRUCache* createLRUCache() {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->head = cache->tail = NULL;
    cache->size = 0;
    return cache;
}

// Function to add a page to the front of the cache
void addPageToFront(LRUCache* cache, int page) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->page = page;
    new_node->prev = NULL;
    new_node->next = cache->head;

    if (cache->head != NULL) {
        cache->head->prev = new_node;
    }
    cache->head = new_node;

    if (cache->tail == NULL) {
        cache->tail = new_node;
    }

    cache->size++;
}

// Function to remove a page from the cache (least recently used)
void removeLRU(LRUCache* cache) {
    if (cache->size == 0) return;
    Node* temp = cache->tail;

    if (cache->tail->prev != NULL) {
        cache->tail->prev->next = NULL;
    }
    cache->tail = cache->tail->prev;

    free(temp);
    cache->size--;
}

// Function to move a page to the front (recently used)
void moveToFront(LRUCache* cache, int page) {
    Node* current = cache->head;
    while (current != NULL && current->page != page) {
        current = current->next;
    }

    if (current == NULL) {
        return; // Page not found
    }

    // If the page is already at the front, no need to move
    if (current == cache->head) {
        return;
    }

    // Remove the node from its current position
    if (current->prev != NULL) {
        current->prev->next = current->next;
    }
    if (current->next != NULL) {
        current->next->prev = current->prev;
    }

    // Move it to the front
    current->prev = NULL;
    current->next = cache->head;
    if (cache->head != NULL) {
        cache->head->prev = current;
    }
    cache->head = current;

    if (cache->tail == current) {
        cache->tail = current->prev;
    }
}

// Function to check if a page is in the cache
int isPageInCache(LRUCache* cache, int page) {
    Node* current = cache->head;
    while (current != NULL) {
        if (current->page == page) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// Function to simulate LRU page replacement
void LRUPageReplacement(int* pages, int num_pages, int num_frames) {
    LRUCache* cache = createLRUCache();
    int page_faults = 0;

    printf("LRU Page Replacement Algorithm:\n");

    for (int i = 0; i < num_pages; i++) {
        int page = pages[i];

        if (!isPageInCache(cache, page)) {
            // Page fault: the page is not in the cache
            if (cache->size == num_frames) {
                // Cache is full, remove the least recently used page
                removeLRU(cache);
            }
            // Add the new page to the cache
            addPageToFront(cache, page);
            page_faults++;
        } else {
            // Page hit: move the page to the front
            moveToFront(cache, page);
        }

        // Print the current cache state
        Node* current = cache->head;
        while (current != NULL) {
            printf("%d ", current->page);
            current = current->next;
        }
        printf("\n");
    }

    printf("Total Page Faults: %d\n", page_faults);

    // Free the allocated memory for the cache
    Node* current = cache->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(cache);
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

    // Call LRU page replacement algorithm
    LRUPageReplacement(pages, num_pages, num_frames);

    // Free the allocated memory for pages
    free(pages);

    return 0;
}

