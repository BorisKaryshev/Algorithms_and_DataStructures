#include <stdio.h>
#ifndef SORT
#define SORT

void swap(void *x, void *y, size_t size);

typedef struct {
    void *ptr;
    size_t type_size;
    size_t size;
} array_t;

array_t init_array(size_t sz, 
                   size_t type_size, 
                   void *(*generate)(const size_t i), 
                   void (*destroy_elem)(void *)
);

void destroy_array(array_t a, void (*destroy_elem)(void *));
array_t copy_array(const array_t ar);

void buble_sort(void *begin, void *end, size_t type_size, 
                int (*is_bigger)(const void *a, const void *b)
);

void selection_sort(void *begin, void *end, size_t type_size, 
                int (*is_bigger)(const void *a, const void *b)
);

void insertion_sort(void *begin, void *end, size_t type_size, 
                int (*is_bigger)(const void *a, const void *b)
);

void shell_sort(void *begin, void *end, size_t type_size, 
                int (*is_bigger)(const void *a, const void *b)
);

void countintg_sort(void *begin, void *end, size_t type_size, 
                int (*is_bigger)(const void *a, const void *b)
);

#endif