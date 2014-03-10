#if !defined(_ARRAY_H)
#define _ARRAY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct array_struct;
typedef struct array_struct Array;
typedef void (*freefunc)(void*);

extern bool (*array_append)(Array *array, const void *value);
bool array_appendint(Array *array, uintptr_t value);
size_t array_cap(Array *array);
void array_free(Array *array);
void array_freeContents(Array *array, void (*freefunc)(void*));
extern void** (*array_get)(Array *array, size_t index);
uintptr_t* array_getint(Array *array, size_t index);
size_t array_len(Array *array);
Array* array_new(size_t cap);
void array_newcap(Array *array, size_t newcap);
void array_prune(Array *array);
extern bool (*array_put)(Array *array, size_t index, const void *value);
bool array_putint(Array *array, size_t index, uintptr_t value);
Array* array_slice(Array *array, size_t start, intmax_t len);

#endif

