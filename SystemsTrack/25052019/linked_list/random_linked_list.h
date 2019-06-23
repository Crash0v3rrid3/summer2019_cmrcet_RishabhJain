#ifndef random_linked_list_h
#define random_linked_list_h
typedef struct _RNode {
    char data;
    struct _RNode *next, *random;
} RNode;

RNode *newRNode(char);
#endif