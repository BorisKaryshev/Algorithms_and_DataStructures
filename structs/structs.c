#include "structs.h"
#include <stdlib.h>
#include <stdio.h>

/* LIST */

void list_push_back(plist_t pl, void *value){
    pListNode list = pl->list;
    if (list == NULL){
        pl->list = malloc(sizeof(struct ListNode));
        list = pl->list;
        list->value = value;
        list->next = NULL;
        list->prev = NULL;
        return;
    }
    
    for (; list->next != NULL; list = list->next);
    list->next = (pListNode)malloc(sizeof(struct ListNode));
    list->next->next = NULL;
    list->next->prev = list;
    list->next->value = value;
}

int list_delete_index(plist_t pl, const size_t index, void (*destroy)(void *)){
    pListNode list = pl->list; 
    for (; index > 0 && list->next != NULL; list = list->next);
    if (index > 0)
        return 0;
    
    list->prev->next = list->next;
    if (list->next != NULL)
        list->next->prev = list->prev;
    destroy(list->value);
    free(list);
    return 1;
}

int list_delete_value(plist_t pl, void *value, int (*is_equal)(void *, void *), void (*destroy)(void *)) {
    pListNode list = pl->list;
    if(is_equal(list->value, value)) {
        pl->list = list->next;
        if(pl->list) {
            pl->list->prev = list->prev;
            if (list->prev) {
                list->prev->next = pl->list;
            }
        }
        destroy(list->value);
        free(list);
    } 
    if(list->next != NULL)
        list = list->next;
    for(;!is_equal(list->value, value) && list->next != NULL; list = list->next);
    if(!is_equal(list->value, value))
        return 0;
    
    list->prev->next = list->next;
    if(list->next != NULL)
        list->next->prev = list->prev;

    destroy(list->value);
    free(list);
    return 1;
}

plist_t create_list(size_t length, void* (*generator)(const size_t i)) {
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

    plist_t out = malloc(sizeof(struct list_s));
    out->list = first;
    return out;
}

void free_list(plist_t pl, void (*destroy)(void *)) {
    pListNode list = pl->list; 
    if (list == NULL){
        free(pl);
        return;
    }
    if (list->next != NULL)
        list = list->next;
    for(; list->next != NULL; list = list->next){
        destroy(list->prev->value);
        free(list->prev);
    }
    destroy(list->value);
    free(list);
    free(pl);
}


void list_push_forward(plist_t pl, void *value) {
    pListNode list = pl->list; 
    list->prev = (pListNode) malloc(sizeof(struct ListNode));
    list->prev->next = list;
    list->prev->prev = NULL;
    list->prev->value = value;
}

static void free_plist(pListNode pnode, void (*destroy)(void*)) {
    pListNode prev = pnode;
    for(;pnode; pnode = pnode->next) {
        destroy(prev->value);
        free(prev);
        prev = pnode;
    }
}
/* STACK */

stack_pt create_stack(size_t length, void* (*generator) (const size_t i)) {
    stack_pt out = (stack_pt) malloc(sizeof(struct stack_s));
    if (length == 0) {
        out->first = NULL;
        return out;
    }
    plist_t tmp = create_list(length, generator);
    out->first = tmp->list;
    free(tmp);
    return out;
}
void free_stack(stack_pt st, void (*destroy)(void*)) {
    free_plist(st->first, destroy);
    st->first = NULL;
    free(st);
}

void* stack_pop(stack_pt st) {
    if (st->first != NULL) {
        pListNode tmp = st->first;
        void *out = st->first->value;
        st->first = tmp->next;
        free(tmp); 
        return out;
    }
    return NULL;
}
void list_for_each(plist_t pl, void(*to_do)(void*)) {
    pListNode list = pl->list;
    for(;list; list = list->next) {
        to_do(list->value);
    }
}


void stack_push(stack_pt st, void* val) {
    if(st->first != NULL) {
        st->first->prev = malloc(sizeof(struct ListNode));
        st->first->prev->next = st->first;
        st->first = st->first->prev;
        st->first->value = val;
        st->first->prev = NULL;
    } else {
        st->first = (pListNode) malloc(sizeof(struct ListNode));
        st->first->next = NULL;
        st->first->prev = NULL;
        st->first->value = val;
    }

}

/* QUEUE */

queue_pt create_queue(size_t length, void* (*generator)(const size_t i)) {
    queue_pt out = (queue_pt) malloc(sizeof(struct queue_s));
    if (length == 0) {
        out->first = NULL;
        out->last = NULL;
        return out;
    }
    plist_t tmp = create_list(length, generator);
    out->first = tmp->list;
    free(tmp);
    pListNode temp = out->first;
    for(; temp->next!=NULL; temp=temp->next);
    out->last = temp;

    return out;
}
void queue_push(queue_pt q, void* val) {
    if (q->first != NULL) {
        pListNode tmp = malloc(sizeof(struct ListNode));
        tmp->value = val;
        tmp->next = q->first;
        tmp->prev = NULL;
        q->first->prev = tmp;
        q->first = tmp;
    } else {
        q->first = (pListNode) malloc(sizeof(struct ListNode));
        q->first->next = NULL;
        q->first->prev = NULL;
        q->first->value = val;
        q->last = q->first;
    }
}
void* queue_pop(queue_pt q){
    if (q->last != NULL) {
        return q->last->value;
    }
    puts("ERROR: queue is empty.");
    return 0;
}

void free_queue(queue_pt q, void (*destroy)(void*)) {
    free_plist(q->first, destroy);
    free(q);
}