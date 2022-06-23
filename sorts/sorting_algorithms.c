#include "sort.h"

void buble_sort(array_t ar, int (*is_bigger)(const array_t arr, const size_t i, const size_t j)) {
    for(size_t i = 0; i < ar.size; i++) {
        for(size_t j = i; j < ar.size; j++) {
            if (is_bigger(ar, i, j)) {
                SWAP(ar.ptr[i], ar.ptr[j]); 
            }
        }
    }
}

void selection_sort(array_t ar, int (*is_bigger)(const array_t arr, const size_t i, const size_t j)) {
    for (size_t i = 0 ; i < ar.size; i++) {
        ARRAY_TYPE min = ar.ptr[i];
        size_t index = i;
        
        for (size_t j = i + 1; j < ar.size; j++)
            if (is_bigger(ar, index, j)) {
                min = ar.ptr[j];
                index = j;
            }
        SWAP(ar.ptr[i], ar.ptr[index]);
    }
}

