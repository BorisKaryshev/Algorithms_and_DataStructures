#include <stdio.h>
#include "structs.h"

LIST_TYPE list_gen(const size_t i) {
    return i;
}
void print_list(pListNode list) {
    for(; list != NULL; list= list->next) {
        printf("%d ", list->value);
    }
    putchar('\n');
}
int list_eq(const LIST_TYPE a, const LIST_TYPE b) {
    return a == b;
}

int main() {
    queue_pt q = create_queue(0, list_gen);
    queue_push(q, 100);
    queue_push(q, 31);
    
    queue_push(q, -852);

    QUEUE_TYPE i;
    while((i = queue_pop(q)) != 0) {
        printf("%d ", i);
    }
    putchar('\n');

    free_queue(q);
    return 0;
}