#include "random_linked_list.h"

RNode *newRNode(char data){
    RNode *node = new RNode;
    node->data = data;

    return node;
}