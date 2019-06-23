#include "linkedlist_block_reverse.h"

int doKNodesExist(Node *head, int k) {
    int num = 0;

    while(head && num != k) {
        head = head->next;
        num++;
    }

    return num == k;
}

Node *reverseKNodes(Node *ptr, int k, Node **nextHead) {
    Node *ptrNext = ptr->next, *tmp = ptrNext;

    while (k > 1) {
        k--;
        tmp = ptrNext->next;    // Backup 3rd node
        ptrNext->next = ptr;    // reverse
        ptr = ptrNext;          // Move to next
        ptrNext = tmp;          // Move to next
    }
    *nextHead = tmp;            // Pointer to head of next block

    return ptr;                 // return new head of current block
}

Node *blockReverse(Node *head, int k) {
    Node *ptr = head, *nextHead, *newHead, *prevTail = 0, *prevHead = 0, *finalHead = 0;

    while(1) {
        if(doKNodesExist(ptr, k)) {
            newHead = reverseKNodes(ptr, k, &nextHead);             // Reverse k nodes and get newHead, newTail, nextHead
            if(prevTail) prevTail->next = newHead;                  // Connect prevTail to nextHead

            prevTail = ptr;                                         // tail of previous block
            prevHead = newHead;                                     // head of previous block
            ptr = nextHead;                                         // Move to next block

            if(!finalHead) finalHead = prevHead;                    // assign finalHead
        }   else    {
            break;
        }
    }

    if(prevTail) prevTail->next = nextHead;                         // If n % k != 0 && n > k

    return finalHead ? finalHead : ptr;
}
