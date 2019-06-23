#include "clone_random_linked_list_approach_1.h"
#include <map>

using namespace std;

RNode *cloneSingleNode(RNode *node) {
    RNode *result = (RNode *)calloc(sizeof(RNode), 1);

    result->data = node->data;

    return result;
}

RNode *cloneRandomLinkedListApproach1(RNode *head) {
    map<RNode *, RNode *> llMap;
    RNode *resultHead = cloneSingleNode(head), *headPtr = head->next, *resultPtr = resultHead;
    llMap.insert( { head, resultHead } );

    while(headPtr) {
        resultPtr->next = cloneRandomLinkedListApproach1(headPtr);
        resultPtr = resultPtr->next;
        llMap.insert( { headPtr, resultPtr } );
        headPtr = headPtr->next;
    }

    headPtr = head; resultPtr = resultHead;

    while(headPtr) {
        if(headPtr->random)
            resultPtr->random = llMap.find(headPtr->random)->second;
        else
            resultPtr->random = 0;

        headPtr = headPtr->next;
        resultPtr = resultPtr->next;
    }

    return resultHead;
}

