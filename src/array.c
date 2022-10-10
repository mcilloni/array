#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

#define CAP_INC_FACTOR 10
static struct allocator default_alloc = {
    .alloc = &calloc,
    .free = &free,
};

static inline const uintptr_t* ptr_rerange(
    const uintptr_t *ptr,
    const ptrdiff_t diff,
    const uintptr_t *const start,
    const uintptr_t *const end
) {
    const uintptr_t* diffptr = ptr + diff;

    // catch overflow, could use some ASM to optimize
    ptr = diffptr < ptr ? end : diffptr;

    ptr = ptr > end ? end : ptr;

    ptr = ptr < start ? start : ptr;

    return ptr;
}

uintptr_t* array_appendint(struct array *const array, const uintptr_t value) {
    return array_insertint(array, (size_t) slice_len(array->content), value);
}

uintptr_t* array_appendptr(struct array *const array, void *const value) {
    return array_appendint(array, (uintptr_t) value);
}

size_t array_cap(const struct array *const array) {
    return array->cap;
}

void array_clear(struct array *const array) {
    memset(array->content.arr, 0, array->cap);
    array->content.len = 0;
}

void array_deinit(struct array *const array) {
    if (array->alloc) {
        (array->alloc->free(array->content.arr));
    }
}

void array_deinit_items(struct array *const array, void (*const freefunc)(void*)) {
    if (array->content.arr) {
        for (
            uintptr_t *it = slice_begin(array->content), *const end = slice_end(array->content);
            it < end;
            ++it
        ) {
            if (*it) {
                freefunc((void*) *it);
            }
        }
    }
}

void array_deinit_with(struct array *const array, void (*const freefunc)(void*)) {
    array_deinit_items(array, freefunc);
    array_deinit(array);
}

struct array* array_fromintslice(struct array *const dest, const struct slice from) {
    return array_fromintslice_with(dest, from, &default_alloc);
}

struct array* array_fromintslice_with(
    struct array *const dest,
    const struct slice from,
    const struct allocator *const alloc
) {
    *dest = (struct array) {0};

    if (!array_init_with(dest, from.len, alloc)) {
        return NULL;
    }

    if (from.arr) {
        memcpy(dest->content.arr, from.arr, from.len * sizeof *from.arr);
        dest->content.len = from.len;
    }

    assert(dest->cap == (size_t) dest->content.len);
    assert(dest->content.len == from.len);

    return dest;
}

struct array* array_fromslice(
    struct array *const dest,
    const struct slice from,
    copyfunc *const cf,
    freefunc *const ff
) {
    return array_fromslice_with(dest, from, cf, ff, &default_alloc);
}

struct array* array_fromslice_with(
    struct array *const dest,
    const struct slice from,
    copyfunc *const cf,
    freefunc *const ff,
    const struct allocator *const alloc
) {
    *dest = (struct array) {0};

    if (!array_init_with(dest, from.len, alloc)) {
        return NULL;
    }

    uintptr_t *it = slice_begin(from), *const end = slice_end(from), *dit = slice_begin(dest->content);

    for (
        ;
        it < end;
        ++it, ++dit
    ) {
        void *const copy = cf((void*) *it);

        if (!copy) {
            goto fail;
        }

        *dit = (uintptr_t) copy;
    }

    dest->content.len = from.len;

    assert(dest->cap == (size_t) dest->content.len);
    assert(dest->content.len == from.len);

    return dest;

fail:
    --dit;

    for (const uintptr_t *const beg = slice_begin(dest->content); dit >= beg; --dit) {
        if (*dit) {
            ff((void*) *dit);
        }
    }

    array_deinit(dest);

    return NULL;
}

struct array* array_init(struct array *const dest, const size_t cap) {
    return array_init_with(dest, cap, &default_alloc);
}

struct array* array_init_with(struct array *const dest, const size_t cap, const struct allocator *const alloc) {
    uintptr_t *area = NULL;

    if (cap) {
        area = alloc->alloc(cap, sizeof *dest->content.arr);

        if (!area) {
            return NULL;
        }
    }

    *dest = (struct array){
        .content = {
            .arr = area,
            .len = 0U,
        },
        .cap = cap,
        .alloc = alloc,
    };

    return dest;
}

uintptr_t* array_insertint(struct array *const array, const size_t index, const uintptr_t value) {
    if (array->cap <= index) {
        // cap is rounded to a multiple of CAP_INC in order to avoid reallocating constantly
        // i.e., if index is 12 and CAP_INC is 10, the new capacity will be 2 * 10 == 20.
        if (!array_resize(array, ((index / CAP_INC_FACTOR) + 1) * CAP_INC_FACTOR, NULL)) {
            return NULL;
        } 
    }

    if (index > PTRDIFF_MAX) {
        return NULL;
    }

    const ptrdiff_t six = index;

    const ptrdiff_t curlen = slice_len(array->content);

    uintptr_t *const loc = slice_begin(array->content) + six;
    if (six < curlen) {
        // move all elements to make room at `index`
        const size_t after = curlen - six;

        memmove(loc + 1, loc, after * sizeof *loc);
    }

    *loc = value;

    if (curlen <= six) {
        array->content.len = six + 1;
    }

    return loc;
}

uintptr_t* array_insertptr(struct array *const array, const size_t index, void *const value) {
    return array_insertint(
        array,
        index,
        value ? (uintptr_t) value : (uintptr_t) 0 // ensure nullptrs are 0s
    );
}

struct array* array_resize(struct array *const array, const size_t newcap, freefunc *const ff) {
    assert(array->content.len >= 0);

    const struct allocator *const alloc = array->alloc;
    uintptr_t *oldarr = array->content.arr;
    const size_t oldlen = array->content.len;

    const size_t lowercap = (array->cap > newcap) ? newcap : array->cap;
    const ptrdiff_t newlen = (oldlen > newcap) ? newcap : oldlen;

    uintptr_t *newarr = alloc->alloc(newcap, sizeof(uintptr_t));

    memmove(newarr, oldarr, lowercap * sizeof(uintptr_t));

    // deallocate elements that went out of bounds

    if (ff) {
        for (
            uintptr_t *it = oldarr + newlen, *const oldend = oldarr + oldlen;
            it < oldend;
            ++it
        ) {
            ff((void*) *it);
        }
    }

    alloc->free(oldarr);

    array->content = (struct slice) {
        .arr = newarr,
        .len = newlen,
    };

    array->cap = newcap;

    return array;
}

uintptr_t* slice_begin(const struct slice s) {
    return s.arr;
}

uintptr_t* slice_end(const struct slice s) {
    return s.arr + s.len;
}

struct slice slice_fromstatic(const ptrdiff_t len, uintptr_t pointer[len]) {
    return (struct slice) {
        .arr = pointer,
        .len = len,
    };
}

uintptr_t* slice_getint(const struct slice s, const size_t index) {
    assert(s.len >= 0);

#if !defined(NDEBUG)
    if (index >= (size_t) s.len) {
        return NULL;
    }
#endif

    return s.arr + index;
}

void** slice_getptr(const struct slice s, const size_t index) {
    return (void**) slice_getint(s, index);
}

ptrdiff_t slice_len(const struct slice s) {
    return s.len;
}

struct slice slice_range(const struct slice s, const ptrdiff_t start, const ptrdiff_t end) {
    uintptr_t *const beg = slice_begin(s), *const fin = slice_end(s);
    
    uintptr_t 
        *const nbeg = (uintptr_t*) ptr_rerange(beg, start, beg, fin),
    *const nfin = (uintptr_t*) ptr_rerange(beg, end, beg, fin);

    ptrdiff_t nlen = nfin - nbeg;

    return (struct slice) {
        .arr = nbeg,
        .len = nlen < 0 ? 0 : nlen,
    };
}
