#include "array.h"

#include <stdio.h>

#include "utils.h"

Array* create_array(size_t size) {
    if (unlikely(size == 0)) {
        return NULL;
    }

    Array *array = (Array*) malloc(sizeof(Array));
    if (unlikely(array == NULL)) {
        fprintf(stderr,"error: memory allocation failed");
        return NULL;
    }

    array->data = (int*) malloc(size * sizeof(int));
    if (array->data == NULL) {
        free(array);
        fprintf(stderr,"error: memory allocation failed");
        return NULL;
    }

    array->size = size;

    return array;
}

void free_array(Array* array) {
    if (unlikely(array == NULL)) {
        return;
    }

    free(array->data);
    free(array);
}

int read_array(Array *const array, FILE *stream) {
    if (unlikely(array == NULL || stream == NULL)) {
        return ERROR_ARG;
    }

    for (size_t i = 0; i < array->size; ++i) {
        int code = fscanf(stream, "%d", array->data + i);
        if (unlikely(code < 0)) {
            fprintf(stderr,"error: incorrect input");
            return ERROR_INPUT;
        }
    }

    return SUCCESS;
}

int print_array(const Array *const array, FILE *stream) {
    if (unlikely(array == NULL)) {
        return ERROR_ARG;
    }

    for (size_t i = 0; i < array->size; ++i) {
        int code = fprintf(stream, "%d ", array->data[i]);
        if (unlikely(code < 0)) {
            fprintf(stderr,"error: output failed");
            return ERROR_OUTPUT;
        }
    }
    fprintf(stdout, "\n");

    return SUCCESS;
}

bool is_equal(const Array *const first, const Array *const second) {
    if (unlikely(first == NULL || second == NULL)) {
        return false;
    }

    if (first->size != second->size) {
        return false;
    }

    for (size_t i = 0; i < first->size; ++i) {
        if (first->data[i] != second->data[i]) {
            return false;
        }
    }

    return true;
}