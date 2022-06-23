#include "structs.h"
#include <stdlib.h>
#include <stdio.h>

void list_push_back(pListNode list, const LIST_TYPE value){
    if (list == NULL)
        return;
    
    for (; list->next != NULL; list = list->next);
    list->next = (pListNode)malloc(sizeof(struct ListNode));
    list->next->next = NULL;
    list->next->prev = list;
    list->next->value = value;
}

int list_delete_index(pListNode list, const size_t index){
    for (; index > 0 && list->next != NULL; list = list->next);
    if (index > 0)
        return 0;
    
    list->prev->next = list->next;
    if (list->next != NULL)
        list->next->prev = list->prev;
    free(list);
    return 1;
}

int list_delete_value(pListNode list, const LIST_TYPE value) {
    for(;list->value != value && list->next != NULL; list = list->next);
    if(list->value != value)
        return 0;
    
    list->prev->next = list->next;
    if(list->next != NULL)
        list->next->prev = list->prev;

    free(list);
    return 1;
}

pListNode create_list(size_t length, LIST_TYPE (*generator)(const size_t i)) {
    pListNode first = (pListNode) malloc(sizeof(struct ListNode));
    first->prev = NULL;
    pListNode current = first;
    for(size_t i = 0; i < length-1; i++) {
        current->value = generator(i+1);
        current->next = (pListNode) malloc(sizeof(struct ListNode));
        current->next->prev = current;
        current = current->next;
    }
    current->value = generator(length);
    current->next = NULL;

    return first;
}

void free_list(pListNode list) {
    if (list == NULL)
        return;
    if (list->next != NULL)
        list = list->next;
    for(; list->next != NULL; list = list->next);
        free(list->prev);
    free(list);
}