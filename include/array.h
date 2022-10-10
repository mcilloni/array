#if !defined(_ARRAY_H)
#define _ARRAY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DISPATCH_INTPTRFN(X, INTFN, PTRFN) _Generic( \
    (X), \
    signed char: INTFN, \
    short: INTFN, \
    int: INTFN, \
    long: INTFN, \
    long long: INTFN, \
    unsigned char: INTFN, \
    unsigned short: INTFN, \
    unsigned: INTFN, \
    unsigned long: INTFN, \
    unsigned long long: INTFN, \
    default: PTRFN \
)

struct slice {
    uintptr_t *arr;
    ptrdiff_t len;
};

#define SLICE_MAX PTRDIFF_MAX
#define SLICE_MIN PTRDIFF_MIN

struct allocator {
    void* (*alloc)(size_t nmemb, size_t size);
    void (*free)(void *ptr);
};

struct array {
    struct slice content;
    size_t cap;
    const struct allocator *alloc;
};

typedef void* (copyfunc)(void*);
typedef void (freefunc)(void*);

#define array_append(A, X) (DISPATCH_INTPTRFN(X, array_appendint, array_appendptr)(A, X))
uintptr_t* array_appendint(struct array *a, uintptr_t value);
uintptr_t* array_appendptr(struct array *a, void *value);
size_t array_cap(const struct array *a);
void array_clear(struct array *a);
void array_deinit(struct array *a);
void array_deinit_items(struct array *a, freefunc *ff);
void array_deinit_with(struct array *a, freefunc *ff);
struct array* array_fromintslice(struct array *dest, struct slice from);
struct array* array_fromintslice_with(struct array *dest, struct slice from, const struct allocator *alloc);
struct array* array_fromslice(struct array *dest, struct slice from, copyfunc *cf, freefunc *ff);
struct array* array_fromslice_with(struct array *dest, struct slice from, copyfunc *cf, freefunc *ff, const struct allocator *alloc);
struct array* array_init(struct array *dest, size_t cap);
struct array* array_init_with(struct array *dest, size_t cap, const struct allocator *alloc);
#define array_insert(A, X) (DISPATCH_INTPTRFN(X, array_insertint, array_insertptr)(A, X))
uintptr_t* array_insertint(struct array *a, size_t index, uintptr_t value);
uintptr_t* array_insertptr(struct array *a, size_t index, void *value);
struct array* array_resize(struct array *a, size_t newcap, freefunc *ff);

uintptr_t* slice_begin(struct slice s);
uintptr_t* slice_end(struct slice s);
struct slice slice_fromstatic(ptrdiff_t len, uintptr_t pointer[len]);
uintptr_t* slice_getint(struct slice s, size_t index);
void** slice_getptr(struct slice s, size_t index);
ptrdiff_t slice_len(struct slice s);
struct slice slice_range(struct slice s, ptrdiff_t start, ptrdiff_t end);

#endif
