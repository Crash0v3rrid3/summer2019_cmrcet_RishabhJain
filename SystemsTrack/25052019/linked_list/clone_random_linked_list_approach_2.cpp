#include "clone_random_linked_list_approach_2.h"

void createNewRandomListInZigZagForm(RNode *head) {
    RNode *ptr = head, *next;

    while(ptr) {
        next = ptr->next;
        ptr->next = newRNode(ptr->data);
        ptr->next->next = next;
        ptr = next;
    }
}

void updateRandomPointersInNewList(RNode *head) {
    RNode *oldNode, *newNode;

    while(oldNode) {
        newNode->random = oldNode->random ? oldNode->random->next : 0;
        oldNode = newNode->next;
        newNode = oldNode->next;
    }
}

RNode *resetTheNewZigZagListToTwoLists(RNode *head) {
    RNode *finalHead = head->next;
    RNode *oldNode = head, *newNode = head->next, *aux;

    while(oldNode) {
        aux = newNode->next;
        oldNode->next = newNode->next;
        oldNode = oldNode->next;
        if(oldNode) {
            newNode->next = oldNode->next;
            newNode = newNode->next;
        }
    }

    return finalHead;
}

RNode *cloneRandomLinkedListApproach2(RNode *head) {
    createNewRandomListInZigZagForm(head);
    updateRandomPointersInNewList(head);
    return resetTheNewZigZagListToTwoLists(head);
}