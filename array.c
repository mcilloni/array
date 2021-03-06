#include "array.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct array_struct {
  uintptr_t *arr;
  size_t cap,len;
  bool isSlice;
};

Array* array_new(size_t cap) {
  Array *arr = calloc(1, sizeof(Array));
  if (cap) {
    arr->arr = calloc(cap, sizeof(uintptr_t));
  }
  arr->cap = cap;
  return arr;
}


void array_newcap(Array *array, size_t newcap) {

  size_t lowercap = (array->cap > newcap) ? newcap : array->cap;

  uintptr_t *newarr = calloc(newcap, sizeof(uintptr_t));

  memmove(newarr, array->arr, lowercap * sizeof(uintptr_t));

  free(array->arr);

  array->arr = newarr;
  array->len = (array->len > newcap) ? newcap : array->len;
  array->cap = newcap;
  array->isSlice = false;

}

uintptr_t* array_getint(Array *array, size_t index) {
  if (array->len <= index) {
    return NULL;
  }

  return array->arr + index;
}

size_t array_cap(Array *array) {
  return array->cap;
}

size_t array_len(Array *array) {
  return array->len;
}

const static uint8_t CAP_INC = 10;

bool array_putint(Array *array, size_t index, uintptr_t value) {
  bool newalloc = false;
  if (array->cap <= index) {
    newalloc = true;
  /* Why this? Because I want cap to be rounded to a multiple of CAP_INC for sake of simplicity. I.e., if index is 12 and CAP_INC is 10, we have 12 / 10 == 1; so new capacity will be 2 * 10 == 20.*/
    array_newcap(array, ((index / CAP_INC) + 1) * CAP_INC); 
  }

  array->arr[index] = value;

  ++index;

  if (array->len <= index) {
    array->len = index;
  }

  return newalloc;
}

bool array_appendint(Array *array, uintptr_t value) {
  return array_putint(array, array->len, value);
}

void array_free(Array *array) {

  if (array) {

    if (!array->isSlice) {
      free(array->arr);
    }

    free(array);

  }

}

void array_freeContents(Array *array, void (*freefunc)(void*)) {

  if (array) {

    if (array->isSlice) {
      return;
    }

    freefunc = freefunc ? freefunc : free;
    for (size_t i = 0; i < array->len; ++i) {
      freefunc((void*) array->arr[i]);
    }

  }

}

void array_prune(Array *array) {
  memset(array->arr, 0, array->cap);
  array->len = 0;
}

Array* array_slice(Array *array, size_t start, intmax_t len) {
  Array *ret = array_new(0);

  ret->isSlice = true;
    
  if (len < 0) {
    len = array->len - start;
  }

  if (len > (array->len - start)) {
    len = array->len - start;
  }

  ret->cap = len;
  ret->len = len;

  ret->arr = array->arr + start;

  return ret;
}

bool (*array_append)(Array *array, const void *value) = (bool (*) (Array*,const void*)) array_appendint;
void** (*array_get)(Array *array, size_t index) = (void** (*) (Array*, size_t)) array_getint;
bool (*array_put)(Array *array, size_t index, const void *value) = (bool (*)(Array*,size_t,const void*)) array_putint;

