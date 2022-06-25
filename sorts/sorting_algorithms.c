#include "sort.h"

void buble_sort(array_t ar, int (*is_bigger)(const array_t arr, const size_t i, const size_t j)) {
    for(size_t i = ar.size; i > 0; i--) {
        for(size_t j = 0; j < i; j++) {
            if (is_bigger(ar, j, j+1))
                SWAP(ar.ptr[j], ar.ptr[j+1]);
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

void insertion_sort(array_t ar, int (*is_bigger)(const array_t arr, const size_t i, const size_t j)) {
    for(size_t i = 1; i < ar.size; i++) {
        for(size_t j = i; j > 0 && is_bigger(ar, j-1, j); j--) {
            SWAP(ar.ptr[j], ar.ptr[j-1]);
        }
    }
}

void shell_sort(array_t ar, int (*is_bigger)(const array_t arr, const size_t i, const size_t j)) {
    for(size_t step = ar.size/2; step > 0; step /=2)
        for(size_t i = step; i < ar.size; i++) 
            for(size_t j = i; j >= step && is_bigger(ar, j-step, j); j--)
                SWAP(ar.ptr[j-step], ar.ptr[j]);
}