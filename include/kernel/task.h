#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <stdint.h>
#include <stdbool.h>

// Process states
typedef enum {
    TASK_RUNNING,     // Currently executing
    TASK_READY,       // Ready to run
    TASK_BLOCKED,     // Waiting for an event
    TASK_SLEEPING,    // Sleeping for a specified time
    TASK_ZOMBIE,      // Terminated but not yet reaped
    TASK_DEAD         // Terminated and reaped
} task_state_t;

// Process priority levels
typedef enum {
    PRIORITY_IDLE = 0,
    PRIORITY_LOW = 1,
    PRIORITY_NORMAL = 2,
    PRIORITY_HIGH = 3,
    PRIORITY_REALTIME = 4
} task_priority_t;

// CPU context structure
typef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    uint32_t eflags;
    uint32_t cs;
    uint32_t ds;
    uint32_t es;
    uint32_t fs;
    uint32_t gs;
    uint32_t ss;
    uint32_t cr3;
} cpu_context_t;

// Process control block (PCB)
typedef struct task_control_block {
    uint32_t pid;                   // Process ID
    uint32_t ppid;                  // Parent process ID
    task_state_t state;             // Current state
    task_priority_t priority;       // Priority level
    cpu_context_t context;          // CPU context
    uint32_t kernel_stack;          // Kernel stack pointer
    uint32_t user_stack;            // User stack pointer
    uint32_t page_directory;        // Page directory physical address
    uint32_t time_slice;            // Remaining time slice
    uint32_t wakeup_time;           // Time to wake up (for sleep)
    struct task_control_block* next; // Next task in the list
    struct task_control_block* prev; // Previous task in the list
    char name[32];                  // Process name
} task_t;

// Initialize the tasking system
void tasking_init(void);

// Create a new task
int task_create(void (*entry)(void), const char* name, task_priority_t priority);

// Get the current task
task_t* task_current(void);

// Yield the CPU to the next task
void task_yield(void);

// Exit the current task
void task_exit(int status) __attribute__((noreturn));

// Wait for a child process to exit
int task_wait(int* status);

// Sleep for the specified number of milliseconds
void task_sleep(uint32_t milliseconds);

// Wake up a sleeping task
void task_wakeup(task_t* task);

// Change the priority of a task
void task_set_priority(task_t* task, task_priority_t priority);

// Get the number of running tasks
uint32_t task_count(void);

// Print information about all tasks
void task_print_list(void);

// Context switching
void switch_task(cpu_context_t* old_context, cpu_context_t* new_context);

// Initialize the scheduler
void scheduler_init(void);

// Schedule the next task to run
void schedule(void);

// Add a task to the scheduler
void scheduler_add(task_t* task);

// Remove a task from the scheduler
void scheduler_remove(task_t* task);

// Get the next task to run
task_t* scheduler_next(void);

// Switch to the next task
void scheduler_switch(void);

#endif // KERNEL_TASK_H
