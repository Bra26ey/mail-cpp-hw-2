#ifndef PROJECT_INCLUDE_ARRAY_H_
#define PROJECT_INCLUDE_ARRAY_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct Array {
    int* data;
    size_t size;
} Array;

Array* create_array(size_t size);
void free_array(Array *array);
int read_array(Array *const array, FILE *stream);
int print_array(const Array *const array, FILE *stream);
bool is_equal(const Array *const first, const Array *const second);

#endif  // PROJECT_INCLUDE_ARRAY_H_
