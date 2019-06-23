#ifndef _stack_h
#define _stack_h

typedef struct _StackElement {
	unsigned int element;
	struct _StackElement *nextElement;
} StackElement;

typedef struct _Stack {
	StackElement *top;
} Stack;

void push(Stack *, unsigned int);
void pop(Stack *);
unsigned int peek(Stack);

#endif