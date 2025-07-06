#ifndef MEM_H
#define MEM_H

#include <stddef.h>

/* Sets the first num bytes of ptr to the specified value */
void* memset(void* ptr, int value, size_t num);

/* Copies num bytes from src to dest (no overlap handling) */
void* memcpy(void* dest, const void* src, size_t num);

/* Copies num bytes from src to dest, safe for overlapping regions */
void* memmove(void* dest, const void* src, size_t num);

/* Compares the first num bytes of ptr1 and ptr2
   Returns 0 if equal, negative if ptr1 < ptr2, positive if ptr1 > ptr2 */
int memcmp(const void* ptr1, const void* ptr2, size_t num);

/* Scans the first num bytes of ptr for the given value.
   Returns pointer to first occurrence or NULL if not found */
void* memchr(const void* ptr, int value, size_t num);

#endif
// im tired man