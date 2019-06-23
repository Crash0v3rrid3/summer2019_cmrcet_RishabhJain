#ifndef _queue_h
#define _queue_h

typedef struct _QueueElement {
	char *element;
	struct _QueueElement *nextElement;
} QueueElement;

typedef struct _Queue {
	QueueElement *front;
	QueueElement  *rear;
} Queue;

void enqueueFront(Queue *, char *);
void enqueueRear(Queue *, char *);
char *dequeueFront(Queue *);
char *dequeueRear(Queue *);

#endif