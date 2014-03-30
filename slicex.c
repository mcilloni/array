#include "array.h"

#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void printarray(Array *arr) {
  bool inside = false;
  fputs("[ ", stdout);
  size_t len = array_len(arr);
  for (uint8_t i = 0; i < len; ++i) {
    if (inside) {
      fputs(", ",stdout);
    } else {
      inside = true;
    }

    printf("%" PRIuPTR,  *array_getint(arr, i));
  }
  puts(" ]");
}

int main(void) {

  srand(time(NULL));

  Array *arr = array_new(3);


  for (uint8_t i = 0; i < 31; ++i) {
    array_appendint(arr, i);
  }

  printarray(arr);

  Array *slice = array_slice(arr, 3, 11);

  printarray(slice);

  array_free(slice);

  slice = array_slice(arr, 12, -1);

  printarray(slice);

  array_free(slice);

  slice = array_slice(arr, 12, 100);

  printarray(slice);

  array_free(slice);

  array_free(arr);
  return EXIT_SUCCESS;
}

