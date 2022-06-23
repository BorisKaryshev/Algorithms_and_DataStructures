#include <stdio.h>
#include "structs.h"

LIST_TYPE list_gen(const size_t i) {
    return i%13;
}
void print_list(pListNode list) {
    for(; list != NULL; list= list->next) {
        printf("%d ", list->value);
    }
    putchar('\n');
}

int main() {
    pListNode ls = create_list(30, list_gen);
    
    print_list(ls);
    while(list_delete_value(ls, 8));
    print_list(ls);

    free_list(ls);
    return 0;
}