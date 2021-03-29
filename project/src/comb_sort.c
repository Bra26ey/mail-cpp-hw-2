#include "comb_sort.h"
#include "utils.h"

#define GAP_COEF 1.25F

static size_t get_gap(size_t gap);
static void swap(int *a, int* b);

int sort(Array *array) {
    if (unlikely(array == NULL)) {
        return ERROR_ARG;
    }

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

    return SUCCESS;
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