#include "sort.h"
#include <stdlib.h>
#include <stddef.h>

array_t init_array(size_t sz, ARRAY_TYPE (*generate)(const size_t i)) {
    array_t tmp;
    tmp.size = sz;
    tmp.ptr = (ARRAY_TYPE*) malloc(sz*sizeof(ARRAY_TYPE));
    for(size_t i = 0; i < sz; i++)
        tmp.ptr[i] = generate(i+1);

    return tmp;
}

void destroy_array(array_t a) {
    free(a.ptr);
}

array_t copy_array(const array_t ar) {
    array_t out;
    out.ptr = (ARRAY_TYPE *) malloc(ar.size*sizeof(ARRAY_TYPE));
    out.size = ar.size;
    for(size_t i = 0; i < out.size; i++)
        out.ptr[i] = ar.ptr[i]; 

    return out;
}