#include "array.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void) {

  srand(time(NULL));

  Array *arr = array_new(3);

  const char *lol = "lol", *folz = "folz";

  array_append(arr, lol);
  array_append(arr, folz);

  bool inside = false;
  size_t len = array_len(arr);
  fputs("[ ", stdout);
  for (uint8_t i = 0; i < len; ++i) {
    if (inside) {
      fputs(", ",stdout);
    } else {
      inside = true;
    }

    printf("%s", *array_get(arr, i));
  }
  puts(" ]");

  array_free(arr);
  return EXIT_SUCCESS;
}
