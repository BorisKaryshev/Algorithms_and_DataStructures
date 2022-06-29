#include <time.h>
#include "structs.h"
#include "trees.h"

#define SIZE 30

BIN_TREE_TYPE gen(const size_t i) {
        return rand()%SIZE;
}
int cmp(const BIN_TREE_TYPE a, const BIN_TREE_TYPE b) {
    return a < b;
}

void print(BIN_TREE_TYPE *val) {
    printf("%2d ", *val);
}

int main() { 
    for(int i = 0; i < 10000; i++){
        pb_tree_t out = create_bin_tree(10000, gen, cmp);
        
        int x = rand()%SIZE;
        while(bin_tree_delete(x, out));
        
        free_bin_tree(out);
        if (i%100 == 0) 
            printf("|%d|\n", i);
    }
    return 0;
}