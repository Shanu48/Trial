#include <stdio.h>
#include <stdlib.h>

typedef struct Block {
    size_t size;
    int is_allocated; // 1 if allocated, 0 if free
    struct Block* next;
} Block;

Block* memory_pool = NULL; // Head of the free list

// Function to initialize the memory pool with a given size
void initialize_memory(size_t total_size) {
    memory_pool = (Block*) malloc(sizeof(Block));
    if (!memory_pool) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    memory_pool->size = total_size;
    memory_pool->is_allocated = 0;
    memory_pool->next = NULL;
}

// Function to allocate memory using the First Fit strategy
void* first_fit_allocate(size_t size) {
    Block* current = memory_pool;
    
    while (current != NULL) {
        if (!current->is_allocated && current->size >= size) {
            // Allocate memory by marking the block as allocated
            current->is_allocated = 1;

            // Split the block if there is extra space
            if (current->size > size + sizeof(Block)) {
                Block* new_block = (Block*) ((char*)current + sizeof(Block) + size);
                new_block->size = current->size - size - sizeof(Block);
                new_block->is_allocated = 0;
                new_block->next = current->next;
                current->next = new_block;
                current->size = size;
            }

            return (void*) (current + 1); // Return pointer to the memory area after the Block struct
        }
        current = current->next;
    }

    printf("Memory allocation failed using First Fit!\n");
    return NULL;
}

// Function to allocate memory using the Best Fit strategy
void* best_fit_allocate(size_t size) {
    Block* current = memory_pool;
    Block* best_fit = NULL;

    while (current != NULL) {
        if (!current->is_allocated && current->size >= size) {
            if (best_fit == NULL || current->size < best_fit->size) {
                best_fit = current;
            }
        }
        current = current->next;
    }

    if (best_fit != NULL) {
        // Allocate memory by marking the block as allocated
        best_fit->is_allocated = 1;

        // Split the block if there is extra space
        if (best_fit->size > size + sizeof(Block)) {
            Block* new_block = (Block*) ((char*)best_fit + sizeof(Block) + size);
            new_block->size = best_fit->size - size - sizeof(Block);
            new_block->is_allocated = 0;
            new_block->next = best_fit->next;
            best_fit->next = new_block;
            best_fit->size = size;
        }

        return (void*) (best_fit + 1); // Return pointer to the memory area after the Block struct
    }

    printf("Memory allocation failed using Best Fit!\n");
    return NULL;
}

// Function to free allocated memory
void free_memory(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    Block* block_to_free = (Block*)ptr - 1;
    block_to_free->is_allocated = 0;

    // Try to merge adjacent free blocks (coalescing)
    Block* current = memory_pool;
    while (current != NULL) {
        if (!current->is_allocated && current->next != NULL && !current->next->is_allocated) {
            current->size += current->next->size + sizeof(Block);
            current->next = current->next->next;
        }
        current = current->next;
    }
}

// Function to print memory blocks (for debugging purposes)
void print_memory_blocks() {
    Block* current = memory_pool;
    while (current != NULL) {
        printf("Block at %p - Size: %zu, Allocated: %d\n", current, current->size, current->is_allocated);
        current = current->next;
    }
}

int main() {
    // Initialize memory pool with a total size of 1000 bytes
    initialize_memory(1000);

    // Try allocating memory using First Fit
    printf("Allocating 200 bytes using First Fit...\n");
    void* ptr1 = first_fit_allocate(200);
    print_memory_blocks();

    // Try allocating memory using Best Fit
    printf("Allocating 150 bytes using Best Fit...\n");
    void* ptr2 = best_fit_allocate(150);
    print_memory_blocks();

    // Free the allocated memory
    printf("Freeing the first allocation...\n");
    free_memory(ptr1);
    print_memory_blocks();

    printf("Freeing the second allocation...\n");
    free_memory(ptr2);
    print_memory_blocks();

    return 0;
}

