#ifndef KERNEL_MM_H
#define KERNEL_MM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Kernel heap size (4MB)
#define KERNEL_HEAP_SIZE (4 * 1024 * 1024)

// Memory map entry types
#define MEMORY_FREE 1
#define MEMORY_RESERVED 2
#define MEMORY_ACPI_RECLAIM 3
#define MEMORY_NVS 4
#define MEMORY_BADRAM 5

// Memory map entry structure
typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi_attrs;
} __attribute__((packed)) memory_map_entry_t;

// Memory block header
struct memory_block {
    size_t size;
    bool free;
    struct memory_block *next;
} __attribute__((packed));

typedef struct memory_block memory_block_t;

// Initialize memory manager
void mm_init(uintptr_t mem_upper);

// Memory allocation functions
void* kmalloc(size_t size);
void* kcalloc(size_t num, size_t size);
void* krealloc(void* ptr, size_t size);
void kfree(void* ptr);

// Memory manipulation
void* kmemset(void* ptr, int value, size_t num);
void* kmemcpy(void* dest, const void* src, size_t num);
int kmemcmp(const void* ptr1, const void* ptr2, size_t num);
void* kmemmove(void* dest, const void* src, size_t num);

// Memory map functions
void mm_print_map(void);
void mm_add_region(uintptr_t base, size_t size, uint32_t type);
void mm_remove_region(uintptr_t base, size_t size);

// Memory statistics
typedef struct {
    size_t total_memory;
    size_t used_memory;
    size_t free_memory;
    size_t block_count;
    size_t alloc_count;
    size_t free_count;
} memory_stats_t;

void mm_get_stats(memory_stats_t* stats);

// Page frame allocator
void pfa_init(void);
void* pfa_alloc(void);
void pfa_free(void* page);

// Virtual memory functions
void vmm_init(void);
void* vmm_alloc_page(void);
void vmm_free_page(void* page);
void* vmm_map_page(void* phys, void* virt);
void vmm_unmap_page(void* virt);

// Heap functions
void heap_init(void);
void* heap_alloc(size_t size);
void heap_free(void* ptr);

#endif // KERNEL_MM_H
