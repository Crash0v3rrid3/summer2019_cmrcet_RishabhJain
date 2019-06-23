#include <stdlib.h>
#include "stack.h"

StackElement *newStackElement(unsigned int ele) {
	StackElement *sElement = (StackElement *)calloc(1, sizeof(StackElement));
	sElement->element = ele;
	return sElement;
}

void push(Stack *s, unsigned int ele) {
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

unsigned int peek(Stack s) {
	if (s.top)
		return s.top->element;
	return NULL;
}