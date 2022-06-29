#include <stdio.h>
#ifndef SORT
#define SORT

#define SWAP(a, b) {typeof(a) tmp = a; a = b; b = tmp;}

#define ARRAY_TYPE int
typedef struct {
    ARRAY_TYPE *ptr;
    size_t size;
} array_t;

array_t init_array(size_t, ARRAY_TYPE (*generate)(const size_t i));

void destroy_array(array_t);
array_t copy_array(const array_t ar);

void buble_sort(array_t, int (*is_bigger)(const ARRAY_TYPE a, const ARRAY_TYPE b));

void selection_sort(array_t ar, int (*is_bigger)(const ARRAY_TYPE a, const ARRAY_TYPE b));

void insertion_sort(array_t ar, int (*is_bigger)(const ARRAY_TYPE a, const ARRAY_TYPE b));

void shell_sort(array_t ar, int (*is_bigger)(const ARRAY_TYPE a, const ARRAY_TYPE b));

void countintg_sort(array_t ar, int (*is_bigger)(const ARRAY_TYPE a, const ARRAY_TYPE b));

#endif