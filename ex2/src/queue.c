#include "queue.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

// ！！要求使用栈实现队列，因此只能使用stack.h中的函数。不允许直接访问stack的底层实现！！

/**
 * 初始化队列
 * @param q 操作队列
 */
Queue *QueueInit(Queue *q) {
  if (!q) {
    q = (Queue *) malloc(sizeof(Queue));
    myassert(q);
  }
  q->sts = q->str = NULL;
  q->sts = StackInit(q->sts);
  q->str = StackInit(q->sts);
  return q;
}

void QueueDestroy(Queue *q) {
  if (!q) return;
  StackDestroy(q->sts);
  free(q);
}

/**
 * 清空队列 操作队列
 * @param q
 */
void ClearQueue(Queue *q) {
  StackDestroy(q->sts);
  q->sts = q->str = NULL;
  q->sts = StackInit(q->sts);
  q->str = StackInit(q->sts);
}

/**
 *
 * @param q 操作队列
 * @return 返回队列已使用长度
 */
size_t QueueLength(Queue *q) {
  return StackLength(q->str);
}

/**
 * 判断队列是否为空
 * @param q 操作队列
 * @return 如果为空返回1，否则返回0
 */
int QueueEmpty(Queue *q) {
  return StackEmpty(q->sts);
}

int QueueFull(Queue *q) {
  return StackFull(q->sts);
}

/**
 * 入队操作，将元素插入队列，需要处理队列满的情况（队列元素数目最多为 MaxSize）
 * @param q 操作队列
 * @param e 插入的元素
 * @return 如果插入成功返回1，否则返回0
 */
int QueueEn(Queue *q, const DataType *e) {
  // 认真思考，从哪个栈Push元素，时间复杂度如何？能否降低
  // 直接向正向栈写入即可
  return Push(q->sts, e);
}

/**
 * 出队操作，从队列中取出一个元素
 * @param q 操作队列
 * @param e 接受出队元素
 * @return 如果成功取出返回1，否则返回0
 */
int QueueDe(Queue *q, DataType *e) {
  // 认真思考，从哪个栈Pop元素，时间复杂度如何？能否降低
  // 取反向栈的栈顶
  return Pop(q->str, e);
}

/**
 * 获取队列头（不删除元素）
 * @param q 操作队列
 * @param e 接受队列头元素
 * @return 如果成功获取返回1，否则返回0
 */
int QueueGetHead(const Queue *q, DataType *e) {
  return StackGetTop(q->sts, e);
}

/**
 * 获取队列的一个数组拷贝，顺序为从队列头到队列尾
 * @param q 操作队列
 * @param seq 栈中元素的一个拷贝
 */
void QueueToArray(const Queue *q, DataType *seq) {
  // 取决于你的栈如何设计，元素存在哪个队列里。
  // 直接读取正向栈
  StackToArray(q->sts, seq);
}
