#include "merge_sort.h"

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

#define GAP_COEF 1.25F

typedef struct Arguments {
    Array **array;     // Массивы для сортировки в каждом из потоков
    size_t *bound;     // Номера минимальных элементов массивов, доступных для слияния
    size_t size;       // Размерность = количество потоков
} Arguments;

static Arguments* create_arg(size_t nom_threads, Array *array);
static void free_arg(Arguments *arguments);
static void* thread_comb_sort(void *arg);
static size_t get_gap(size_t gap);
static void swap(int *a, int* b);
static void merge(Arguments *arguments, Array *array);
static size_t get_min_index(Arguments *arguments);

int merge_sort(Array *array) {
    if (unlikely(array == NULL)) {
        return ERROR_ARG;
    }

    if (unlikely(array->size < 2)) {
        return SUCCESS;
    }

    long int NOM_THREADS = sysconf(_SC_NPROCESSORS_ONLN);

    if (unlikely((long) array->size < NOM_THREADS)) {
        NOM_THREADS = 1;
    }

    pthread_t *threads = (pthread_t*) malloc(NOM_THREADS * sizeof(pthread_t));
    if (unlikely(threads == NULL)) {
        fprintf(stderr, "%s", "error: memory allocation failed 1\n");
        return ERROR_MEM;
    }

    Arguments *arguments = create_arg(NOM_THREADS, array);
    if (unlikely(arguments == NULL)) {
        free(threads);
        return ERROR_MEM;
    }

    for (size_t i = 0; (long)i < NOM_THREADS; ++i) {
        int code = pthread_create(threads + i, NULL, thread_comb_sort, (void*)(arguments->array[i]));
        if (unlikely(code != 0)) {
            for (size_t j = 0; j < i; ++j) {
                pthread_cancel(threads[j]);
            }
            free_arg(arguments);
            free(threads);
            fprintf(stderr, "%s", "error: thread creation failed\n");
            return ERROR_THREAD;
        }
    }

    for (size_t i = 0; i < (size_t)NOM_THREADS; ++i) {
        int code = pthread_join(threads[i], NULL);
        if (unlikely(code != 0)) {
            for (size_t j = 0; j < (size_t)NOM_THREADS; ++j) {
                pthread_cancel(threads[j]);
            }
            free_arg(arguments);
            free(threads);
            fprintf(stderr, "%s", "error: thread join failed\n");
            return ERROR_THREAD;
        }
    }

    merge(arguments, array);

    free(threads);
    free_arg(arguments);

    return SUCCESS;
}

Arguments* create_arg(size_t nom_threads, Array *array) {
    Arguments *arguments = (Arguments*)malloc(sizeof(Arguments));
    if (unlikely(arguments == NULL)) {
        fprintf(stderr, "%s\n", "error: memory allocation failed");
        return NULL;
    }

    arguments->size = nom_threads;

    arguments->array = (Array**) malloc(nom_threads * sizeof(Array*));
    if (unlikely(arguments->array == NULL)) {
        fprintf(stderr, "%s\n", "error: memory allocation failed");
        free(arguments);
        return NULL;
    }

    size_t data_size = array->size / nom_threads;
    size_t nom_big_threads = array->size % nom_threads;

    size_t copyed_size = 0;

    for (size_t i = 0; i < nom_big_threads; ++i) {
        arguments->array[i] = create_array(data_size + 1);
        if (unlikely(arguments->array[i] == NULL)) {
            arguments->size = i;  // Для корректной работы free_arg
            free_arg(arguments);
            fprintf(stderr, "%s\n", "error: memory allocation failed");
            return NULL;
        }

        for (size_t j = 0; j < arguments->array[i]->size; ++j) {
            arguments->array[i]->data[j] = array->data[copyed_size + j];
        }
        copyed_size += data_size + 1;
    }

    for (size_t i = nom_big_threads; i < nom_threads; ++i) {
        arguments->array[i] = create_array(data_size);
        if (unlikely(arguments->array[i] == NULL)) {
            free_arg(arguments);
            fprintf(stderr, "%s\n", "error: memory allocation failed");
            return NULL;
        }

        for (size_t j = 0; j < arguments->array[i]->size; ++j) {
            arguments->array[i]->data[j] = array->data[copyed_size + j];
        }
        copyed_size += data_size;
    }

    arguments->bound = (size_t*) malloc(nom_threads * sizeof(size_t));
    if (unlikely(arguments->bound == NULL)) {
        free_arg(arguments);
        fprintf(stderr, "%s\n", "error: memory allocation failed");
        return NULL;
    }
    for (size_t i = 0; i < nom_threads; ++i) {
        arguments->bound[i] = 0;
    }

    return arguments;
}

void free_arg(Arguments* arguments) {
    if (unlikely(arguments == NULL)) {
        return;
    }

    for (size_t i = 0; i < arguments->size; ++i) {
        free_array(arguments->array[i]);
    }
    free(arguments->array);
    free(arguments->bound);
    free(arguments);
}

void* thread_comb_sort(void *arg) {
    Array *array = (Array*) arg;
    size_t gap = array->size;
    bool swaps = true;
    while (gap > 1 || swaps) {
        gap = get_gap(gap);
        swaps = false;
        for (size_t i = 0; i < array->size - gap; ++i) {
            size_t j = i + gap;
            if (array->data[i] > array->data[j]) {
                swap(array->data + i, array->data + j);
                swaps = true;
            }
        }
    }

    pthread_exit(SUCCESS);
}

size_t get_gap(size_t gap) {
    size_t result = gap / GAP_COEF;
    return (result > 1) ? result : 1;
}

void swap(int *a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void merge(Arguments *arguments, Array *array) {
    for (size_t i = 0; i < array->size; ++i) {
        size_t min_index = get_min_index(arguments);
        array->data[i] = arguments->array[min_index]->data[arguments->bound[min_index]++];
    }
}

size_t get_min_index(Arguments *arguments) {
    int min = __INT_MAX__;
    size_t nom_min;
    for (size_t i = 0; i < arguments->size; ++i) {
        if (unlikely(arguments->bound[i] >= arguments->array[i]->size)) {
            continue;
        }

        int current = arguments->array[i]->data[arguments->bound[i]];
        if (min > current) {
            min = current;
            nom_min = i;
        }
    }

    return nom_min;
}