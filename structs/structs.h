#ifndef STRUCTS
#define STRUCTS
#include <stddef.h>

#define LIST_TYPE int 
typedef struct ListNode {
    LIST_TYPE value;
    struct ListNode *next;
    struct ListNode *prev;
}* pListNode;

void list_push_back(pListNode list, const LIST_TYPE value);
void list_push_forward(pListNode list, const LIST_TYPE value);

int list_delete_index(pListNode list, const size_t index);

int list_delete_value(pListNode list, const LIST_TYPE value, int (*is_equal)(const LIST_TYPE a, const LIST_TYPE b));

pListNode create_list(size_t length, LIST_TYPE (*generator)(const size_t i));
void free_list(pListNode list);

#define STACK_TYPE LIST_TYPE
typedef struct stack_s {
    pListNode first;
}* stack_pt;

stack_pt create_stack(size_t length, STACK_TYPE (*generator) (const size_t i));
void free_stack(stack_pt);

STACK_TYPE stack_pop(stack_pt);
void stack_push(stack_pt, STACK_TYPE);

#define QUEUE_TYPE LIST_TYPE
typedef struct queue_s {
    pListNode first, last;
}* queue_pt;

queue_pt create_queue(size_t length, QUEUE_TYPE (*generator)(const size_t i));
void queue_push(queue_pt q, QUEUE_TYPE val);
QUEUE_TYPE queue_pop(queue_pt q);
void free_queue(queue_pt q);


#endif