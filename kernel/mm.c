#include "../include/kernel.h"
#include "../include/kernel/mm.h"
#include "../drivers/vga.h"
#include "../drivers/serial.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// Simple memory block structure
struct block {
    size_t size;
    bool free;
    struct block *next;
};

// Start of the heap
static uint8_t heap[KERNEL_HEAP_SIZE];
static struct block *free_list = (void*)heap;

// Initialize the memory manager
void mm_init(uintptr_t mem_upper) {
    (void)mem_upper; // Currently unused, but kept for future use
    free_list->size = KERNEL_HEAP_SIZE - sizeof(struct block);
    free_list->free = true;
    free_list->next = NULL;
}

// Simple implementation of kmalloc (first-fit algorithm)
void* kmalloc(size_t size) {
    struct block *curr;
    void *result = NULL;
    
    // Align size to 8 bytes
    size = (size + 7) & ~7;
    
    // Find a free block that's large enough
    for (curr = free_list; curr != NULL; curr = curr->next) {
        if (curr->free && curr->size >= size) {
            // If the block is big enough to split
            if ((curr->size - size) > (sizeof(struct block) + 8)) {
                struct block *new_block = (void*)((uint8_t*)curr + sizeof(struct block) + size);
                new_block->size = curr->size - size - sizeof(struct block);
                new_block->free = true;
                new_block->next = curr->next;
                
                curr->size = size;
                curr->next = new_block;
            }
            
            curr->free = false;
            result = (void*)((uint8_t*)curr + sizeof(struct block));
            break;
        }
    }
    
    return result;
}

// Simple implementation of kfree
void kfree(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    
    struct block *curr = (struct block*)((uint8_t*)ptr - sizeof(struct block));
    curr->free = true;
    
    // Merge with next block if it's free
    if (curr->next != NULL && curr->next->free) {
        curr->size += curr->next->size + sizeof(struct block);
        curr->next = curr->next->next;
    }
    
    // Merge with previous block if it's free
    if (curr != free_list) {
        struct block *prev = free_list;
        while (prev->next != curr) {
            prev = prev->next;
        }
        
        if (prev->free) {
            prev->size += curr->size + sizeof(struct block);
            prev->next = curr->next;
        }
    }
}

// Initialize memory manager during kernel startup
void mm_initialize(void) {
    mm_init(0); // We're not using the memory map yet
    vga_puts("Memory manager initialized\n");
}
