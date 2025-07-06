#ifndef _STDINT_H
#define _STDINT_H

/* Exact-width integer types */
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

/* Minimum values of exact-width signed integer types */
#define INT8_MIN   (-128)
#define INT16_MIN  (-32768)
#define INT32_MIN  (-2147483647 - 1)
#define INT64_MIN  (-9223372036854775807LL - 1)

/* Maximum values of exact-width signed integer types */
#define INT8_MAX   127
#define INT16_MAX  32767
#define INT32_MAX  2147483647
#define INT64_MAX  9223372036854775807LL

/* Maximum values of exact-width unsigned integer types */
#define UINT8_MAX  255
#define UINT16_MAX 65535
#define UINT32_MAX 4294967295U
#define UINT64_MAX 18446744073709551615ULL

/* Minimum values of minimum-width integer types */
#define INT_LEAST8_MIN   INT8_MIN
#define INT_LEAST16_MIN  INT16_MIN
#define INT_LEAST32_MIN  INT32_MIN
#define INT_LEAST64_MIN  INT64_MIN

/* Maximum values of minimum-width integer types */
#define INT_LEAST8_MAX   INT8_MAX
#define INT_LEAST16_MAX  INT16_MAX
#define INT_LEAST32_MAX  INT32_MAX
#define INT_LEAST64_MAX  INT64_MAX

/* Maximum values of minimum-width unsigned integer types */
#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

/* Minimum values of fastest minimum-width integer types */
#define INT_FAST8_MIN    INT8_MIN
#define INT_FAST16_MIN   INT32_MIN
#define INT_FAST32_MIN   INT32_MIN
#define INT_FAST64_MIN   INT64_MIN

/* Maximum values of fastest minimum-width integer types */
#define INT_FAST8_MAX    INT8_MAX
#define INT_FAST16_MAX   INT32_MAX
#define INT_FAST32_MAX   INT32_MAX
#define INT_FAST64_MAX   INT64_MAX

/* Maximum values of fastest minimum-width unsigned integer types */
#define UINT_FAST8_MAX   UINT8_MAX
#define UINT_FAST16_MAX  UINT32_MAX
#define UINT_FAST32_MAX  UINT32_MAX
#define UINT_FAST64_MAX  UINT64_MAX

/* Integer types capable of holding object pointers */
typedef int32_t  intptr_t;
typedef uint32_t uintptr_t;

/* Greatest-width integer types */
typedef int64_t  intmax_t;
typedef uint64_t uintmax_t;

/* Limits of integer types capable of holding object pointers */
#define INTPTR_MIN  INT32_MIN
#define INTPTR_MAX  INT32_MAX
#define UINTPTR_MAX UINT32_MAX

/* Limits of greatest-width integer types */
#define INTMAX_MIN  INT64_MIN
#define INTMAX_MAX  INT64_MAX
#define UINTMAX_MAX UINT64_MAX

/* Limits of other integer types */
#define PTRDIFF_MIN INT32_MIN
#define PTRDIFF_MAX INT32_MAX
#define SIG_ATOMIC_MIN INT32_MIN
#define SIG_ATOMIC_MAX INT32_MAX
#define SIZE_MAX    UINT32_MAX

/* Integer constant macros */
#define INT8_C(x)   x
#define INT16_C(x)  x
#define INT32_C(x)  x
#define INT64_C(x)  x ## LL

#define UINT8_C(x)  x ## U
#define UINT16_C(x) x ## U
#define UINT32_C(x) x ## U
#define UINT64_C(x) x ## ULL

#define INTMAX_C(x)  x ## LL
#define UINTMAX_C(x) x ## ULL

#endif /* _STDINT_H */
