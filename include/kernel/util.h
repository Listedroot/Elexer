#ifndef KERNEL_UTIL_H
#define KERNEL_UTIL_H

#include <stdint.h>
#include <stddef.h>

// String functions
size_t kstrlen(const char* str);
int kstrcmp(const char* s1, const char* s2);
int kstrncmp(const char* s1, const char* s2, size_t n);
char* kstrcpy(char* dest, const char* src);
char* kstrncpy(char* dest, const char* src, size_t n);
char* kstrcat(char* dest, const char* src);
char* kstrncat(char* dest, const char* src, size_t n);
char* kstrchr(const char* str, int c);
char* kstrrchr(const char* str, int c);
char* kstrstr(const char* haystack, const char* needle);
char* kstrtok(char* str, const char* delimiters);

// Memory functions
void* kmemset(void* ptr, int value, size_t num);
void* kmemcpy(void* dest, const void* src, size_t num);
void* kmemmove(void* dest, const void* src, size_t num);
int kmemcmp(const void* ptr1, const void* ptr2, size_t num);
void* kmemchr(const void* ptr, int value, size_t num);

// Conversion functions
int katoi(const char* str);
long katol(const char* str);
long long katoll(const char* str);
unsigned long katoul(const char* str);
unsigned long long katoull(const char* str);
char* kitoa(int value, char* str, int base);
char* kitoa(int value, char* str, int base);
char* kltoa(long value, char* str, int base);
char* klltoa(long long value, char* str, int base);
char* kultoa(unsigned long value, char* str, int base);
char* kulltoa(unsigned long long value, char* str, int base);

// Character functions
int kisalpha(int c);
int kisdigit(int c);
int kisalnum(int c);
int kiscntrl(int c);
int kisgraph(int c);
int kislower(int c);
int kisupper(int c);
int kisprint(int c);
int kispunct(int c);
int kisspace(int c);
int ktoupper(int c);
int ktolower(int c);

// Bit manipulation
uint8_t kbit_set(uint8_t byte, uint8_t bit);
uint8_t kbit_clear(uint8_t byte, uint8_t bit);
uint8_t kbit_toggle(uint8_t byte, uint8_t bit);
bool kbit_check(uint8_t byte, uint8_t bit);

// Math functions
int kabs(int n);
long int klabs(long int n);
long long int kllabs(long long int n);

// Debugging
void khexdump(const void* data, size_t size);
void kstacktrace(void);

#endif // KERNEL_UTIL_H
