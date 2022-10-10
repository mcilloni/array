#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

void printslice(struct slice s) {
    fputs("[ ", stdout);

    const uintptr_t *it = slice_begin(s), *const fin = slice_end(s);
    if (it != fin) {
        printf("%" PRIdPTR, *it++);

        for (; it < fin; ++it) {
            printf(", %" PRIdPTR, *it);
        }
    }

    puts(" ]");
}

int main(void) {
    struct array arr = {0};
    
    if (!array_init(&arr, 3)) {
        fputs("error: failed to allocate memory\n", stdout);

        return EXIT_FAILURE;
    }

    for (uint8_t i = 0; i < 31; ++i) {
        array_append(&arr, i);
    }

    printslice(arr.content);

    struct slice slice = slice_range(arr.content, 3, 14);

    printslice(slice);

    slice = slice_range(arr.content, 12, SLICE_MAX);

    printslice(slice);

    slice = slice_range(arr.content, 12, 100);

    printslice(slice);

    array_deinit(&arr);

    return EXIT_SUCCESS;
}
