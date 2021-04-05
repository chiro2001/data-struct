#include "stack.h"

typedef struct
{
    Stack stack1;
    Stack stack2;
} Queue;

void InitQueue(Queue *Q);
int GetHead(Queue Q, DataType *e);
int QueueEmpty(Queue Q);
int EnQueue(Queue *Q, DataType e);
int DeQueue(Queue *Q, DataType *e);
void ClearQueue(Queue *Q);
int QueueLength(Queue Q);
void QueueToArray(Queue Q, DataType *seq);
