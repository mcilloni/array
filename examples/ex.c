#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

int main(void) {
    struct array arr = {0};
    
    if (!array_init(&arr, 3U)) {
        fputs("error: failed to allocate memory\n", stderr);

        return EXIT_FAILURE;
    }

    array_appendint(&arr, 33);
    printf("arr[%lu] == %" PRIuPTR ", len == %td, cap == %zu\n",
        0LU, *slice_getint(arr.content, 0), slice_len(arr.content), array_cap(&arr));

    array_insertint(&arr, 0, 25);
    printf("arr[%lu] == %" PRIuPTR ", len == %td, cap == %zu\n", 0LU, *slice_getint(arr.content, 0), slice_len(arr.content), array_cap(&arr));

    array_insertint(&arr, 12, 38);
    printf("arr[%lu] == %" PRIuPTR ", len == %td, cap == %zu\n", 12LU, *slice_getint(arr.content, 12), slice_len(arr.content), array_cap(&arr));

    printf("arr[%lu] == %" PRIuPTR ", len == %td, cap == %zu\n", 0LU, *slice_getint(arr.content, 0), slice_len(arr.content), array_cap(&arr));

    array_clear(&arr);

    size_t cap = array_cap(&arr), ncap = 0;

    for (uint8_t i = 0; i < 31; ++i) {
      array_appendint(&arr, i);
      ncap = array_cap(&arr);
      if (cap != ncap) {
        printf("New capacity: %zu\n", ncap);
        cap = ncap;
      }
    }

    bool inside = false;
    fputs("[ ", stdout);
    for (uint8_t i = 0; i < 31; ++i) {
      if (inside) {
        fputs(", ",stdout);
      } else {
        inside = true;
      }

      printf("%" PRIuPTR, *slice_getint(arr.content, i));
    }
    
    puts(" ]");

    array_deinit(&arr);
    return EXIT_SUCCESS;
}
