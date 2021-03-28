#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "array.h"
#include "comb_sort.h"
#include "merge_sort.h"

#define NAIVE "-n"
#define PARALLEL "-p"

int main(int argc, const char *argv[]) {
    if (unlikely(argc != 2)) {
        fprintf(stderr,"error: arg != 2\n");
        return ERROR_ARG;
    }

    bool is_parallel = !strcmp(argv[1], PARALLEL);
    bool is_native = !strcmp(argv[1], NAIVE);
    if (!is_native && !is_parallel) {
        fprintf(stderr,"error: unknown arg\n");
        return ERROR_ARG;
    }


    size_t size;
    int code = fscanf(stdin, "%lu", &size);
    if (unlikely(code < 0)) {
        fprintf(stderr,"error: incorrect input\n");
        return ERROR_INPUT;
    }

    Array *array = create_array(size);
    if (array == NULL) {
        return ERROR_MEM;
    }

    code = read_array(array, stdin);
    if (unlikely(code != SUCCESS)) {
        free_array(array);
        return ERROR_INPUT;
    }

    if (is_native) {
        code = comb_sort(array);
        if (unlikely(code != SUCCESS)) {
            free_array(array);
            return code;
        }
    }

    if (is_parallel) {
        code = merge_sort(array);
        if (unlikely(code != SUCCESS)) {
            free_array(array);
            return code;
        }
    }

    code = print_array(array, stdout);
    if (unlikely(code != SUCCESS)) {
        free_array(array);
        return code;
    }

    free_array(array);

    return SUCCESS;
}
