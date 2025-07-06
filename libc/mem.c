#include "mem.h"

/* Sets the first number bytes of ptr to value */
void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = (unsigned char*)ptr;
    for (size_t i = 0; i < num; i++) {
        p[i] = (unsigned char)value;
    }
    return ptr;
}

/* Copies num bytes from src to dest; no overlap safety */
void* memcpy(void* dest, const void* src, size_t num) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    for (size_t i = 0; i < num; i++) {
        d[i] = s[i];
    }
    return dest;
}

/* Copies num bytes from src to dest safely even if regions overlap */
void* memmove(void* dest, const void* src, size_t num) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    if (d == s) {
        return dest;
    }

    if (d < s) {
        // Safe to copy forward ....(vsauce music)
        for (size_t i = 0; i < num; i++) {
            d[i] = s[i];
        }
    } else {
        // Copy backwards to avoid overwrite
        for (size_t i = num; i > 0; i--) {
            d[i - 1] = s[i - 1];
        }
    }
    return dest;
}

/* Compares num bytes of ptr1 and ptr2 */
int memcmp(const void* ptr1, const void* ptr2, size_t num) {
    const unsigned char* p1 = (const unsigned char*)ptr1;
    const unsigned char* p2 = (const unsigned char*)ptr2;
    for (size_t i = 0; i < num; i++) {
        if (p1[i] != p2[i]) {
            return (int)p1[i] - (int)p2[i];
        }
    }
    return 0;
}

/* Searches num bytes of ptr for value */
void* memchr(const void* ptr, int value, size_t num) {
    const unsigned char* p = (const unsigned char*)ptr;
    unsigned char val = (unsigned char)value;
    for (size_t i = 0; i < num; i++) {
        if (p[i] == val) {
            return (void*)(p + i);
        }
    }
    return NULL;
}
