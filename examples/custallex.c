#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

// dumb nonsensical allocator-like interface

#define ALLOC_MAX 256
#define ALLOC_AREAS 32

static_assert(!(ALLOC_AREAS % 32), "areas not a multiple of 32");
static_assert(!(ALLOC_MAX % 32), "size not a multiple of 32");

static char alloc_area[ALLOC_AREAS * ALLOC_MAX] = {0};
static ptrdiff_t area_usage[ALLOC_AREAS / sizeof(ptrdiff_t)] = {1};
static char 
    *const alloc_beg = &alloc_area[0],
    *const alloc_end = &alloc_area[ALLOC_AREAS * ALLOC_MAX],
    *alloc_next = &alloc_area[0];

static void area_free(const int ix) {
    const int bucket_ix = ix / sizeof *area_usage;
    const int bit_ix = ix % sizeof *area_usage;

    area_usage[bucket_ix] &= ~(((ptrdiff_t) 0x1) << bit_ix);
}

static void area_grab(const int ix) {
    const int bucket_ix = ix / sizeof *area_usage;
    const int bit_ix = ix % sizeof *area_usage;

    area_usage[bucket_ix] |= ((ptrdiff_t) 0x1) << bit_ix;
}

static bool area_is_busy(const int ix) {
    const int bucket_ix = ix / sizeof *area_usage;
    const int bit_ix = ix % sizeof *area_usage;

    return (area_usage[bucket_ix] >> bit_ix) & 0x1;
}

void* dummy_calloc(const size_t nmemb, const size_t size) {
    const size_t wanted = nmemb * size;

    if (wanted > ALLOC_MAX) {
        return NULL;
    }

    if (!alloc_next) {
        return NULL;
    }

    char *const ret = alloc_next;
    ptrdiff_t ix = (ret - alloc_beg) / ALLOC_MAX;
    area_grab(ix);

    // find next area
    char *next = ret;
    for (;;) {
        next += ALLOC_MAX;

        if (next == ret) {
            next = NULL;

            break;
        }

        if (next > alloc_end) {
            next = alloc_beg;
        }

        ix = (next - alloc_beg) / ALLOC_MAX;

        if (!area_is_busy(ix)) {
            break;
        }
    }

    alloc_next = next;

    return ret;
}

void dummy_free(void *const area) {
    const char *const y = area;

    assert(y >= alloc_beg && y < alloc_end);

    const ptrdiff_t ix = (y - alloc_beg) / ALLOC_MAX;

    area_free(ix);
}

static const struct allocator dummy_alloc = {
    .alloc = dummy_calloc,
    .free = dummy_free,
};

void printsslice(const struct slice s) {
    fputs("[ ", stdout);

    const uintptr_t *it = slice_begin(s), *const fin = slice_end(s);
    if (it != fin) {
        printf("%s", (const char*) *it++);

        for (; it < fin; ++it) {
            printf(", %s", (const char*) *it);
        }
    }

    puts(" ]");
}

int main(void) {
    struct array arr = {0};
    
    if (!array_init_with(&arr, 3, &dummy_alloc)) {
        fputs("error: alloc failed\n", stderr);
    }

    array_append(&arr, (void*) "lol");
    array_append(&arr, (void*) "folz");
    array_append(&arr, (void*) "gogo");
    array_append(&arr, (void*) "poddo");

    printsslice(arr.content);

    array_deinit(&arr);

    return EXIT_SUCCESS;
}
