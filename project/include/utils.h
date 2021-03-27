#ifndef PROJECT_INCLUDE_UTILS_H_
#define PROJECT_INCLUDE_UTILS_H_

#define likely(x)       __builtin_expect((x), 1)
#define unlikely(x)     __builtin_expect((x), 0)

#include <stdlib.h>
#include <stdbool.h>

typedef enum ERROR {
    SUCCESS = 0,
    ERROR_MEM,
    ERROR_OPEN_FILE,
    ERROR_WRITING,
    ERROR_ARG,
    ERROR_INPUT,
    ERROR_OUTPUT,
    ERROR_THREAD,
    ERROR_COUNT
} ERROR;

#endif  // PROJECT_INCLUDE_UTILS_H_
