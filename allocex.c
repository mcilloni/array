#include "array.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>



int main(void) {

  Array *arr = array_new(3);

  for (uint8_t i = 0; i < 31; ++i) {
    array_append(arr, malloc(sizeof(char)));
  }

  array_freeContents(arr, NULL);

  array_free(arr);
  return EXIT_SUCCESS;
}

