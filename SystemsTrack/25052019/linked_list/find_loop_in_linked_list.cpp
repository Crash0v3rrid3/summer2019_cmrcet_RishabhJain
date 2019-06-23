#include "find_loop_in_linked_list.h"

int findLoopInLinkedList(Node *head) {
    if(!head) return 0;
    Node *slow = head, *fast = head;

    while(slow && fast) {
        slow = slow->next;
        fast = fast->next;
        if(fast)
            fast = fast->next;
        if(slow == fast) return 1;
    }

    return 0;
}