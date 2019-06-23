#ifndef _stack_h
#define _stack_h

typedef struct _StackElement {
	char *element;
	struct _StackElement *nextElement;
} StackElement;

typedef struct _Stack {
	StackElement *top;
} Stack;

void push(Stack *, char *);
void pop(Stack *);
char *peek(Stack);

#endif