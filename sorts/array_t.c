#include "sort.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

array_t init_array(size_t sz, 
                   size_t type_size, 
                   void *(*generate)(const size_t i), 
                   void (*destroy_elem)(void *)
) {
    array_t tmp;
    tmp.size = sz;
    tmp.ptr = malloc(sz*type_size);
    tmp.type_size = type_size;
    char *p = tmp.ptr;
    for(size_t i = 0; i < sz; i++){
        void *q = generate(i+1);
        swap(q, p, type_size);
        p += type_size;
        destroy_elem(q);
    }

    return tmp;
}

void destroy_array(array_t a, void (*destroy_elem)(void *)) {
    for(char *p = a.ptr; p != a.ptr + (a.size + 1)*a.type_size; p += a.type_size)
        destroy_elem(p);

    free(a.ptr);
}

array_t copy_array(const array_t ar) {
    array_t out;
    out.size = ar.size;
    out.type_size = ar.type_size;
    out.ptr = malloc(ar.size * ar.type_size);
    for(size_t i = 0; i < ar.size*ar.type_size; i+=ar.type_size) {
        memcpy(out.ptr+i, ar.ptr+i, ar.type_size);
    }

    return out;
}