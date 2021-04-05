#include "stack.h"

typedef struct {
  Stack *sts, *str;       // 正向栈和反向栈
} Queue;

Queue* QueueInit(Queue *q);
void QueueDestroy(Queue *q);
int QueueGetHead(const Queue *q, DataType *e);
int QueueEmpty(Queue *q);
int QueueEn(Queue *q, const DataType *e);
int QueueDe(Queue *q, DataType *e);
void QueueClear(Queue *q);
size_t QueueLength(Queue *q);
void QueueToArray(const Queue *q, DataType *seq);
