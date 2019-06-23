#include <stdlib.h>
#ifndef linked_list_number_sum_h
#define linked_list_number_sum_h

typedef struct _LLNumberNode {
    int data;
    struct _LLNumberNode *next;
} LLNumberNode;

LLNumberNode *addTwoLLNumbers(LLNumberNode *, LLNumberNode *);
LLNumberNode *newLLNumberNode(int data);

#endif
