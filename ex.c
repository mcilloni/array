#include "array.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void) {

  srand(time(NULL));

  Array *arr = array_new(3);

  array_appendint(arr, 33);
  printf("arr[%lu] == %lu, len == %zu, cap == %zu\n", 0LU, *array_getint(arr, 0), array_len(arr), array_cap(arr));


  array_putint(arr, 0, 25);
  printf("arr[%lu] == %lu, len == %zu, cap == %zu\n", 0LU, *array_getint(arr, 0), array_len(arr), array_cap(arr));

  array_putint(arr, 12, 38);
  printf("arr[%lu] == %lu, len == %zu, cap == %zu\n", 12LU, *array_getint(arr, 12), array_len(arr), array_cap(arr));

  printf("arr[%lu] == %lu, len == %zu, cap == %zu\n", 0LU, *array_getint(arr, 0), array_len(arr), array_cap(arr));

  array_prune(arr);

  size_t cap = array_cap(arr), ncap;

  for (uint8_t i = 0; i < 31; ++i) {
    array_appendint(arr, i);
    ncap = array_cap(arr);
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

    printf("%lu", *array_getint(arr, i));
  }
  puts(" ]");

  array_free(arr);
  return EXIT_SUCCESS;
}

