#include "linked_list_number_sum.h"

LLNumberNode *newLLNumberNode(int data) {
    LLNumberNode *node = (LLNumberNode *)calloc(sizeof(LLNumberNode), 1);
    node->data = data;

    return node;
}

LLNumberNode *_addTwoLNumbers(LLNumberNode *num1, LLNumberNode *num2, int size1, int size2, int *carry) {
    if(size1 == 0 && size2 == 0) return NULL;
    if(size1 > size2) {
        LLNumberNode *next = _addTwoLNumbers(num1->next, num2, size1 - 1, size2, carry);
        LLNumberNode *currentNode = newLLNumberNode((num1->data + *carry) % 10);
        *carry = (num1->data + *carry) / 10;
        currentNode->next = next;
        return currentNode;
    }   else if(size1 < size2)  {
        LLNumberNode *next = _addTwoLNumbers(num1, num2->next, size1, size2 - 1, carry);
        LLNumberNode *currentNode = newLLNumberNode((num2->data + *carry) % 10);
        *carry = (num2->data + *carry) / 10;
        currentNode->next = next;
        return currentNode;
    }   else    {
        LLNumberNode *next = _addTwoLNumbers(num1->next, num2->next, size1 - 1, size2 - 1, carry);
        LLNumberNode *currentNode = newLLNumberNode((num1->data + num2->data + *carry) % 10);
        *carry = (num1->data + num2->data + *carry) / 10;
        currentNode->next = next;
        return currentNode;
    }
}

int sizeOfLL(LLNumberNode *node) {
    int size = 0;

    while(node) {
        size++;
        node = node->next;
    }

    return size;
}

LLNumberNode *addTwoLLNumbers(LLNumberNode *num1, LLNumberNode *num2) {
    int size1 = sizeOfLL(num1), size2 = sizeOfLL(num2), carry = 0;
    LLNumberNode *newHead = _addTwoLNumbers(num1, num2, size1, size2, &carry);

    if (carry != 0) {
        LLNumberNode *head = newLLNumberNode(carry);
        head->next = newHead;
        return head;
    }

    return newHead;
}
