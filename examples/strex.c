#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "array.h"

void printsslice(const struct slice s) {
    fputs("[ ", stdout);

    const uintptr_t *it = slice_begin(s), *const fin = slice_end(s);
    if (it != fin) {
        printf("%s", (const char*) *it++);

        for (; it < fin; ++it) {
            printf(", %s", (const char*) *it);
        }
    }

    puts(" ]");
}

int main(void) {
    struct array arr = {0};
    
    if (!array_init(&arr, 3)) {
        fputs("error: alloc failed\n", stderr);
    }

    const char *const lol = "lol", *const folz = "folz";

    array_append(&arr, (void*) lol);
    array_append(&arr, (void*) folz);

    printsslice(arr.content);

    array_deinit(&arr);

    return EXIT_SUCCESS;
}
