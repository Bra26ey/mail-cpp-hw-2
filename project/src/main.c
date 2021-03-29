#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "comb_sort.h"
#include "merge_sort.h"

#define NAIVE "-n"
#define PARALLEL "-p"

int main(int argc, const char *argv[]) {
    if (unlikely(argc != 2)) {
        fprintf(stderr,"error: arg != 2\n");
        return ERROR_ARG;
    }

    FILE *file = fopen(argv[1], "r");
    if (unlikely(file == NULL)) {
        return ERROR_ARG;
    }

    size_t size;
    int code = fscanf(file, "%lu", &size);
    if (unlikely(code < 0)) {
        fprintf(stderr,"error: incorrect input\n");
        return ERROR_INPUT;
    }

    Array *array = create_array(size);
    if (array == NULL) {
        return ERROR_MEM;
    }

    code = read_array(array, file);
    if (unlikely(code != SUCCESS)) {
        free_array(array);
        return ERROR_INPUT;
    }

    code = sort(array);
    if (unlikely(code != SUCCESS)) {
        free_array(array);
        return code;
    }


    code = print_array(array, stdout);
    if (unlikely(code != SUCCESS)) {
        free_array(array);
        return code;
    }

    free_array(array);

    return SUCCESS;
}
