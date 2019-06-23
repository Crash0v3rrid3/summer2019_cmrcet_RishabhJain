#include <stdio.h>
#include <stdlib.h>
#include "string/string_compress.h"
#include "linked_list/linkedlist_block_reverse.h"
#include "matrix/matrix_transpose.h"
#include "matrix/matrix_spiral_approach_1.h"
#include "linked_list/linked_list_number_sum.h"
#include "sorting/colour_sort_approach_1.h"
#include "sorting/colour_sort_approach_2.h"
#include "sorting/colour_sort_approach_3.h"
#include "linked_list/clone_random_linked_list_approach_1.h"
#include "linked_list/clone_random_linked_list_approach_2.h"
#include "linked_list/find_loop_in_linked_list.h"
#include "testcases.h"

void printTestCaseStatus(const char *funcName, int condition) {
    printf("%s: %s\n", funcName, condition ? "Passed" : "Failed");
}

void stringCompressTestCase1 () {
    char str[] = "aaabbcccd";
    char expectedOutput[] = "a3b2c3d";
    char *actualOutput = compress(str);

    printTestCaseStatus(__func__, !strcmp(expectedOutput, actualOutput));
}

void stringCompressTestCase2 () {
    char str[] = "abc";
    char expectedOutput[] = "abc";
    char *actualOutput = compress(str);

    printTestCaseStatus(__func__, !strcmp(expectedOutput, actualOutput));
}

void stringCompressTestCase3 () {
    char str[] = "aaaaaaaaaa";
    char expectedOutput[] = "a10";
    char *actualOutput = compress(str);

    printTestCaseStatus(__func__, !strcmp(expectedOutput, actualOutput));
}

void stringCompressTestCase4 () {
    char str[] = "a";
    char expectedOutput[] = "a";
    char *actualOutput = compress(str);

    printTestCaseStatus(__func__, !strcmp(expectedOutput, actualOutput));
}

void stringCompressTestCase5 () {
    char str[] = "ababababab";
    char expectedOutput[] = "ababababab";
    char *actualOutput = compress(str);

    printTestCaseStatus(__func__, !strcmp(expectedOutput, actualOutput));
}

void stringCompressTestCase6 () {
    char str[] = "$$$$###@@!";
    char expectedOutput[] = "$4#3@2!";
    char *actualOutput = compress(str);

    printTestCaseStatus(__func__, !strcmp(expectedOutput, actualOutput));
}

void stringCompressTestCase7 () {
    char str[] = "aaa   bbb ";
    char expectedOutput[] = "a3 3b3 ";
    char *actualOutput = compress(str);

    printTestCaseStatus(__func__, !strcmp(expectedOutput, actualOutput));
}

void stringCompressTestCase8 () {
    char str[] = "";
    char expectedOutput[] = "";
    char *actualOutput = compress(str);

    printTestCaseStatus(__func__, !strcmp(expectedOutput, actualOutput));
}

//Helper functions Start
Node *strToLL(const char *str1) {
    char *str = (char *)str1;
    Node *head = (Node *)calloc(sizeof(Node), 1), *ptr = head;
    head->data = *str++;

    while(*str) {
        ptr->next = (Node *)calloc(sizeof(Node), 1);
        ptr = ptr->next;
        ptr->data = *str++;
    }

    return head;
}

int areLLEqual(Node *ll1, Node *ll2) {
    while(ll1 && ll2) {
        if(ll1->data != ll2->data) return 0;
        ll1 = ll1->next;
        ll2 = ll2->next;
    }

    return ll1 == ll2;
}

//Helper functions end

void blockReverseTestCase1() {
    Node *head = 0;
    int k = 2;
    Node *expectedOutput = 0;
    Node *actualOutput = blockReverse(head, k);

    printTestCaseStatus(__func__, areLLEqual(expectedOutput, actualOutput));

}

void blockReverseTestCase2() {
    Node *head = strToLL("12345");
    int k = 2;
    Node *expectedOutput = strToLL("21435");
    Node *actualOutput = blockReverse(head, k);

    printTestCaseStatus(__func__, areLLEqual(expectedOutput, actualOutput));
}

void blockReverseTestCase3() {
    Node *head = strToLL("12345");
    int k = 7;
    Node *expectedOutput = strToLL("12345");
    Node *actualOutput = blockReverse(head, k);

    printTestCaseStatus(__func__, areLLEqual(expectedOutput, actualOutput));
}

void blockReverseTestCase4() {
    Node *head = strToLL("12345");
    int k = 5;
    Node *expectedOutput = strToLL("54321");
    Node *actualOutput = blockReverse(head, k);

    printTestCaseStatus(__func__, areLLEqual(expectedOutput, actualOutput));
}

void blockReverseTestCase5() {
    Node *head = strToLL("12345");
    int k = 1;
    Node *expectedOutput = strToLL("12345");
    Node *actualOutput = blockReverse(head, k);

    printTestCaseStatus(__func__, areLLEqual(expectedOutput, actualOutput));
}

//Helper functions Start

int areMatricesEqual(int **arr1, int **arr2, int m, int n) {
    int rowItr, columnItr;

    for(rowItr = 0; rowItr < m; ++rowItr) {
        for(columnItr = 0; columnItr < n; ++columnItr) {
            if(arr1[rowItr][columnItr] != arr2[rowItr][columnItr])
                return 0;
        }
    }

    return 1;
}

//Helper functions end

void matrixTransposeTestCase1() {
    int arr[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int m = 3, n = 3, i, j;
    int **pass = (int **)malloc(sizeof(int *) * m);

    for(i = 0; i < m; ++i) {
        pass[i] = arr[i];
    }

    int expectedOutput[3][3] = {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}}, **actualOutput = matrixTranspose(pass, m, n);
    pass = (int **)malloc(sizeof(int *) * n);
    for(i = 0; i < n; ++i) {
        pass[i] = expectedOutput[i];
    }

    printTestCaseStatus(__func__, areMatricesEqual(actualOutput, pass, n, m));
}

void matrixTransposeTestCase2() {
    int arr[4][1] = {{1}, {2}, {3}, {4}};
    int m = 4, n = 1, i, j;
    int **pass = (int **)malloc(sizeof(int *) * m);

    for(i = 0; i < m; ++i) {
        pass[i] = arr[i];
    }

    int expectedOutput[1][4] = {{1, 2, 3, 4}}, **actualOutput = matrixTranspose(pass, m, n);
    pass = (int **)malloc(sizeof(int *) * n);
    for(i = 0; i < n; ++i) {
        pass[i] = expectedOutput[i];
    }
    printTestCaseStatus(__func__, areMatricesEqual(actualOutput, pass, n, m));
}

//Helper functions Start

int areArraysEqual(int *arr1, int *arr2, int size) {
    while(size--) {
        if(*arr1 != *arr2)
            return 0;
        arr1++;
        arr2++;
    }

    return 1;
}

//Helper functions end

void matrixSpiralApproach1TestCase1() {
    int arr[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int m = 2, n = 3, i;
    int **pass = (int **)malloc(sizeof(int *) * n);

    for(i = 0; i < n; ++i) {
        pass[i] = arr[i];
    }

    int expectedOutput[] = {1, 2, 3, 6, 5, 4},
    *actualOutput = matrixSpiralApproach1(pass, m, n);

    printTestCaseStatus(__func__, areArraysEqual(actualOutput, expectedOutput, m * n));
}

//Helper functions Start

LLNumberNode *strToLLNumberList(const char *str1) {
    if(!*str1) return NULL;
    char *str = (char *)str1;
    LLNumberNode *root = newLLNumberNode(*str - '0'), *ptr = root;
    str++;

    while(*str) {
        ptr->next = newLLNumberNode(*str - '0');
        ptr = ptr->next;
        str++;
    }

    return root;
}

int areTwoLLNumbersEqual(LLNumberNode *num1, LLNumberNode *num2) {
    while(num1 && num2) {
        if(num1->data != num2->data)
            return 0;
        num1 = num1->next;
        num2 = num2->next;
    }

    return num1 == num2;
}

//Helper functions end

void linkedListNumberSumTestCase1() {
    LLNumberNode *num1 = strToLLNumberList("1234");
    LLNumberNode *num2 = strToLLNumberList("123");
    LLNumberNode *expectedOutput = strToLLNumberList("1357"), *actualOutput = addTwoLLNumbers(num1, num2);

    printTestCaseStatus(__func__, areTwoLLNumbersEqual(expectedOutput, actualOutput));
}

void linkedListNumberSumTestCase2() {
    LLNumberNode *num1 = strToLLNumberList("9999");
    LLNumberNode *num2 = strToLLNumberList("1");
    LLNumberNode *expectedOutput = strToLLNumberList("10000"), *actualOutput = addTwoLLNumbers(num1, num2);

    printTestCaseStatus(__func__, areTwoLLNumbersEqual(expectedOutput, actualOutput));
}

void linkedListNumberSumTestCase3() {
    LLNumberNode *num1 = strToLLNumberList("");
    LLNumberNode *num2 = strToLLNumberList("");
    LLNumberNode *expectedOutput = strToLLNumberList(""), *actualOutput = addTwoLLNumbers(num1, num2);

    printTestCaseStatus(__func__, areTwoLLNumbersEqual(expectedOutput, actualOutput));
}

void colourSortApproach1TestCase1() {
    int arr[] = {0, 0, 0, 0}, size = sizeof(arr) / sizeof(arr[0]);
    int expectedOutput[] = {0, 0, 0, 0};
    colorSortApproach1(arr, size);

    printTestCaseStatus(__func__, areArraysEqual(expectedOutput, arr, size));
}

void colourSortApproach1TestCase2() {
    int arr[] = {1, 1, 0, 0}, size = sizeof(arr) / sizeof(arr[0]);
    int expectedOutput[] = {0, 0, 1, 1};
    colorSortApproach1(arr, size);

    printTestCaseStatus(__func__, areArraysEqual(expectedOutput, arr, size));
}

void colourSortApproach1TestCase3() {
    int arr[] = {1, 0, 0, 1}, size = sizeof(arr) / sizeof(arr[0]);
    int expectedOutput[] = {0, 0, 1, 1};
    colorSortApproach1(arr, size);
    
    printTestCaseStatus(__func__, areArraysEqual(expectedOutput, arr, size));
}

void colourSortApproach1TestCase4() {
    int arr[] = {0, 1, 0, 1, 0, 0, 1, 1}, size = sizeof(arr) / sizeof(arr[0]);
    int expectedOutput[] = {0, 0, 0, 0, 1, 1, 1, 1};
    colorSortApproach1(arr, size);
    
    printTestCaseStatus(__func__, areArraysEqual(expectedOutput, arr, size));
}

void colourSortApproach2TestCase1() {
    int arr[] = {0, 0, 0, 0}, size = sizeof(arr) / sizeof(arr[0]);
    int expectedOutput[] = {0, 0, 0, 0};
    colorSortApproach2(arr, size);

    printTestCaseStatus(__func__, areArraysEqual(expectedOutput, arr, size));
}

void colourSortApproach2TestCase2() {
    int arr[] = {1, 1, 0, 0}, size = sizeof(arr) / sizeof(arr[0]);
    int expectedOutput[] = {0, 0, 1, 1};
    colorSortApproach2(arr, size);

    printTestCaseStatus(__func__, areArraysEqual(expectedOutput, arr, size));
}

void colourSortApproach2TestCase3() {
    int arr[] = {1, 0, 0, 1}, size = sizeof(arr) / sizeof(arr[0]);
    int expectedOutput[] = {0, 0, 1, 1};
    colorSortApproach2(arr, size);
    
    printTestCaseStatus(__func__, areArraysEqual(expectedOutput, arr, size));
}

void colourSortApproach2TestCase4() {
    int arr[] = {0, 1, 0, 1, 0, 0, 1, 1}, size = sizeof(arr) / sizeof(arr[0]);
    int expectedOutput[] = {0, 0, 0, 0, 1, 1, 1, 1};
    colorSortApproach2(arr, size);
    
    printTestCaseStatus(__func__, areArraysEqual(expectedOutput, arr, size));
}

void colourSortApproach3TestCase1() {
    int arr[] = {0, 0, 0, 0}, size = sizeof(arr) / sizeof(arr[0]);
    int expectedOutput[] = {0, 0, 0, 0};
    colorSortApproach3(arr, size);

    printTestCaseStatus(__func__, areArraysEqual(expectedOutput, arr, size));
}

void colourSortApproach3TestCase2() {
    int arr[] = {1, 1, 0, 0}, size = sizeof(arr) / sizeof(arr[0]);
    int expectedOutput[] = {0, 0, 1, 1};
    colorSortApproach3(arr, size);

    printTestCaseStatus(__func__, areArraysEqual(expectedOutput, arr, size));
}

void colourSortApproach3TestCase3() {
    int arr[] = {1, 0, 0, 1}, size = sizeof(arr) / sizeof(arr[0]);
    int expectedOutput[] = {0, 0, 1, 1};
    colorSortApproach3(arr, size);
    
    printTestCaseStatus(__func__, areArraysEqual(expectedOutput, arr, size));
}

void colourSortApproach3TestCase4() {
    int arr[] = {0, 1, 0, 1, 0, 0, 1, 1}, size = sizeof(arr) / sizeof(arr[0]);
    int expectedOutput[] = {0, 0, 0, 0, 1, 1, 1, 1};
    colorSortApproach3(arr, size);
    
    printTestCaseStatus(__func__, areArraysEqual(expectedOutput, arr, size));
}

//Helper functions Start

RNode *strToRandomNode(const char *str1) {
    if(!*str1) return 0;
    char *str = (char *)str1;
    RNode *head = newRNode(*str++), *ptr = head;
    
    while(*str) {
        ptr->next = newRNode(*str++);
        ptr = ptr->next;
    }

    return head;
}

int areRandomLinkedListsEqual(RNode *head1, RNode *head2) {
    while(head1 && head2) {
        if(head1->data != head2->data) 
            return 0;
        if(head1->random && head2->random){
            if(head1->random->data != head2->random->data)
                return 0;
        }   else if(head1->random || head2->random) {
            return 0;
        }
            
        head1 = head1->next;
        head2 = head2->next;
    }

    return 1;
}

//Helper functions end

void cloneRandomLinkedListApproach1TestCase1() {
    RNode *head = strToRandomNode("12345");
    head->random = head->next->next;
    head->next->next->random = head;
    head->next->next->next->random = head->next;
    RNode *expectedOutput = head, *actualOutput = cloneRandomLinkedListApproach1(head);

    printTestCaseStatus(__func__, areRandomLinkedListsEqual(expectedOutput, actualOutput));
}

void cloneRandomLinkedListApproach1TestCase2() {
    RNode *head = strToRandomNode("11111");
    head->random = head->next->next;
    head->next->next->random = head;
    head->next->next->next->random = head->next;
    RNode *actualOutput = cloneRandomLinkedListApproach1(head);
    int flag = 1;
    flag = (actualOutput->random == actualOutput->next->next)
            & (actualOutput->next->next->random == actualOutput)
            & (actualOutput->next->next->next->random == actualOutput->next);

    printTestCaseStatus(__func__, flag);
}

void cloneRandomLinkedListApproach2TestCase1() {
    RNode *head = strToRandomNode("12345");
    head->random = head->next->next;
    head->next->next->random = head;
    head->next->next->next->random = head->next;
    RNode *expectedOutput = head, *actualOutput = cloneRandomLinkedListApproach2(head);

    printTestCaseStatus(__func__, areRandomLinkedListsEqual(expectedOutput, actualOutput));
}

void cloneRandomLinkedListApproach2TestCase2() {
    RNode *head = strToRandomNode("11111");
    head->random = head->next->next;
    head->next->next->random = head;
    head->next->next->next->random = head->next;
    RNode *actualOutput = cloneRandomLinkedListApproach2(head);
    int flag = 1;
    flag = (actualOutput->random == actualOutput->next->next)
            & (actualOutput->next->next->random == actualOutput)
            & (actualOutput->next->next->next->random == actualOutput->next);

    printTestCaseStatus(__func__, flag);
}

void findLoopInLinkedListTestCase1() {
    Node *head = strToLL("1234");
    head->next->next->next->next = head->next;
    int expectedOutput = 1, actualOutput = findLoopInLinkedList(head);

    printTestCaseStatus(__func__, expectedOutput == actualOutput);
}

void findLoopInLinkedListTestCase2() {
    Node *head = strToLL("1234");
    int expectedOutput = 0, actualOutput = findLoopInLinkedList(head);

    printTestCaseStatus(__func__, expectedOutput == actualOutput);
}