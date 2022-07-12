#ifndef STRUCTS
#define STRUCTS
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* LIST */

typedef struct ListNode {
    void *value;
    struct ListNode *next;
    struct ListNode *prev;
}* pListNode;

typedef struct list_s {
    pListNode list;
} *plist_t;

void list_push_back(plist_t list, void *value);
void list_push_forward(plist_t list, void *value);

int list_delete_index(plist_t list, const size_t index, void (*destroy)(void *));

int list_delete_value(plist_t list, void *value, int (*is_equal)(void *, void *), void (*destroy)(void *));

plist_t create_list(size_t length, void* (*generator)(const size_t i));
void free_list(plist_t list, void (*destroy)(void *));

void list_for_each(plist_t list, void(*to_do)(void*));

/* STACK */

typedef struct stack_s {
    pListNode first;
}* stack_pt;

stack_pt create_stack(size_t length, void* (*generator) (const size_t i));
void free_stack(stack_pt, void (*destroy)(void*));

void* stack_pop(stack_pt);
void stack_push(stack_pt, void *);

/* QUEUE */

typedef struct queue_s {
    pListNode first, last;
}* queue_pt;

queue_pt create_queue(size_t length, void* (*generator)(const size_t i));
void queue_push(queue_pt q, void* val);
void* queue_pop(queue_pt q);
void free_queue(queue_pt q, void (*destroy)(void*));


#ifdef __cplusplus
}
#endif

#endif