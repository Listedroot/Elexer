#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

// Copy n bytes from src to dest
void* memcpy(void* dest, const void* src, size_t n);

// Set n bytes of dest to val
void* memset(void* dest, int val, size_t n);

// Get length of string s
size_t strlen(const char* s);

// Compare two strings
int strcmp(const char* s1, const char* s2);

// Copy string src to dest
char* strcpy(char* dest, const char* src);

// Copy at most n characters from src to dest
char* strncpy(char* dest, const char* src, size_t n);

// Compare at most n characters of two strings
int strncmp(const char* s1, const char* s2, size_t n);

// Concatenate strings
char* strcat(char* dest, const char* src);

// Find first occurrence of character c in string s
char* strchr(const char* s, int c);

// Find last occurrence of character c in string s
char* strrchr(const char* s, int c);

// Move memory area (handles overlap)
void* memmove(void* dest, const void* src, size_t n);

// Compare memory areas
int memcmp(const void* s1, const void* s2, size_t n);

// Find character in memory
void* memchr(const void* s, int c, size_t n);

// Find length of initial segment not containing reject
size_t strcspn(const char* s1, const char* s2);

// Find length of initial segment containing accept
size_t strspn(const char* s1, const char* s2);

// Find first occurrence of any character from accept in s
char* strpbrk(const char* s, const char* accept);

// Find first occurrence of substring in string
char* strstr(const char* haystack, const char* needle);

// Split string into tokens
char* strtok(char* str, const char* delim);

// Convert string to lowercase in place
char* strlwr(char* s);

// Convert string to uppercase in place
char* strupr(char* s);

// Reverse a string in place
char* strrev(char* s);

// Duplicate string
char* strdup(const char* s);

// Duplicate string with size limit
char* strndup(const char* s, size_t n);

// Copy string with size limitation (safer version of strncpy)
size_t strlcpy(char* dest, const char* src, size_t size);

// Concatenate strings with size limitation
size_t strlcat(char* dest, const char* src, size_t size);

// Case-insensitive string comparison
int strcasecmp(const char* s1, const char* s2);

// Case-insensitive string comparison with length limit
int strncasecmp(const char* s1, const char* s2, size_t n);

// Get error message string
char* strerror(int errnum);

// Get length of a string with size limit
size_t strnlen(const char* s, size_t maxlen);

#endif // STRING_H
