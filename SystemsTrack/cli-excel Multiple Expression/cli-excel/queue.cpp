#include "queue.h"
#include "parser.h"
#include <stdlib.h>

QueueElement *newQueueElement(char *ele) {
	QueueElement *qElement = (QueueElement *)calloc(1, sizeof(QueueElement));
	qElement->element = cloneStr(ele);
	return qElement;
}

void enqueueFront(Queue *q, char *ele) {
	if (!q->rear && !q->front) {
		q->rear = q->front = newQueueElement(ele);
		return;
	}
	QueueElement *node = newQueueElement(ele);
	node->nextElement = q->front;
	q->front = node;
}
void enqueueRear(Queue *q, char *ele) {
	if (!q->rear && !q->front) {
		q->rear = q->front = newQueueElement(ele);
		return;
	}
	QueueElement *node = newQueueElement(ele);
	q->rear->nextElement = node;
	q->rear = q->rear->nextElement;
}
char *dequeueFront(Queue *q) {
	if (!q->rear && !q->front) {
		return NULL;
	}
	QueueElement *node = q->front;
	if (q->rear == q->front) {
		q->front = NULL;
		q->rear = NULL;
	}	else	{
		q->front = q->front->nextElement;
	}
	
	return node->element;
}
char *dequeueRear(Queue *q) {
	if (!q->rear && !q->front) {
		return NULL;
	}
	QueueElement *node = q->rear;
	if (q->rear == q->front) {
		q->front = NULL;
		q->rear = NULL;
	}	else	{
		QueueElement *front = q->front, *prev = front;

		while (front != q->rear) {
			prev = front;
			front = front->nextElement;
		}
		q->rear = prev;
	}

	return node->element;
}