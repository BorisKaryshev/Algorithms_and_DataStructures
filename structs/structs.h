#ifndef STRUCTS
#define STRUCTS
#include <stddef.h>

#define LIST_TYPE long long 
typedef struct ListNode {
    LIST_TYPE value;
    struct ListNode *next;
    struct ListNode *prev;
}* pListNode;

void list_push_back(pListNode list, const LIST_TYPE value);

int list_delete_index(pListNode list, const size_t index);

int list_delete_value(pListNode list, const LIST_TYPE value);

pListNode create_list(size_t length, LIST_TYPE (*generator)(const size_t i));
void free_list(pListNode list);
#endif