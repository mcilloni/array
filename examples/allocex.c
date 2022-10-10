#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

int main(void) {
    struct array arr = {0};
    
    if (!array_init(&arr, 3)) {
        fputs("error: allocation failed\n", stderr);

        return EXIT_FAILURE;
    }

    for (uint8_t i = 0; i < 31; ++i) {
        array_append(&arr, malloc(sizeof(char)));
    }

    array_deinit_with(&arr, free);

    return EXIT_SUCCESS;
}
