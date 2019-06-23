#include "stack.h"
#include "parser.h"
#include <stdlib.h>

StackElement *newStackElement(char *ele) {
	StackElement *sElement = (StackElement *)calloc(1, sizeof(StackElement));
	sElement->element = cloneStr(ele);
	return sElement;
}

void push(Stack *s, char *ele) {
	if (!s->top) {
		s->top = newStackElement(ele);
		return;
	}
	StackElement *node = newStackElement(ele);
	node->nextElement = s->top;
	s->top = node;
}

void pop(Stack *s) {
	if (s->top)
		s->top = s->top->nextElement;
}

char *peek(Stack s) {
	if (s.top)
		return s.top->element;
	return NULL;
}