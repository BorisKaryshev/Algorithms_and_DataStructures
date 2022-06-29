#include "structs.h"
#include <stdlib.h>
#include <stdio.h>

/* LIST */

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

int list_delete_value(pListNode list, const LIST_TYPE value, int (*is_equal)(const LIST_TYPE a, const LIST_TYPE b)) {
    if(list->next != NULL)
        list = list->next;
    for(;!is_equal(list->value, value) && list->next != NULL; list = list->next);
    if(!is_equal(list->value, value))
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

void list_push_forward(pListNode list, const LIST_TYPE value) {
    list->prev = (pListNode) malloc(sizeof(struct ListNode));
    list->prev->next = list;
    list->prev->prev = NULL;
    list->prev->value = value;
}

/* STACK */

stack_pt create_stack(size_t length, STACK_TYPE (*generator) (const size_t i)) {
    stack_pt out = (stack_pt) malloc(sizeof(struct stack_s));
    if (length == 0) {
        out->first = NULL;
        return out;
    }
    out->first = create_list(length, generator);
    return out;
}
void free_stack(stack_pt st) {
    free_list(st->first);
    st->first = NULL;
    free(st);
}

STACK_TYPE stack_pop(stack_pt st) {
    if (st->first != NULL) {
        STACK_TYPE val = st->first->value;
        pListNode tmp = st->first;
        st->first = tmp->next;
        if (st->first != NULL)
            st->first->prev = NULL;
        free(tmp);
        

        return val;
    }
    puts("ERROR: stack is empty.");
    return 0;
}

void stack_push(stack_pt st, STACK_TYPE val) {
    if(st->first != NULL) {
        list_push_forward(st->first, val);
        st->first = st->first->prev;
    } else {
        st->first = (pListNode) malloc(sizeof(struct ListNode));
        st->first->next = NULL;
        st->first->prev = NULL;
        st->first->value = val;
    }

}

/* QUEUE */

queue_pt create_queue(size_t length, QUEUE_TYPE (*generator)(const size_t i)) {
    queue_pt out = (queue_pt) malloc(sizeof(struct queue_s));
    if (length == 0) {
        out->first = NULL;
        out->last = NULL;
        return out;
    }
    out->first = create_list(length, generator);
    pListNode tmp = out->first;
    for(; tmp->next!=NULL; tmp=tmp->next);
    out->last = tmp;

    return out;
}
void queue_push(queue_pt q, QUEUE_TYPE val) {
    if (q->first != NULL) {
        list_push_forward(q->first, val);
        q->first = q->first->prev;
    } else {
        q->first = (pListNode) malloc(sizeof(struct ListNode));
        q->first->next = NULL;
        q->first->prev = NULL;
        q->first->value = val;
        q->last = q->first;
    }
}
QUEUE_TYPE queue_pop(queue_pt q){
    if (q->last != NULL) {
        QUEUE_TYPE out = q->last->value;
        pListNode tmp = q->last;
        q->last = tmp->prev;
        if(q->last != NULL)
            q->last->next = NULL;
        else
            q->first = NULL;
        free(tmp);

        return out;
    }
    puts("ERROR: queue is empty.");
    return 0;
}

void free_queue(queue_pt q) {
    free_list(q->first);
    free(q);
}