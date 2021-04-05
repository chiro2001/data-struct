#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"
#include "utils.h"

// 关于反向栈：一对栈互相共用一段数据空间，
// 在一段栈上的操作改变栈顶指针的同时，会改变对应反向栈的栈基址指针。
// 数据空间是循环的。

/**
 * 初始化一个栈
 * @param src原栈
 * @return new stack
 * @note 传入 src 如果已经初始化，则返回一个反向栈。
 */
Stack *sp_ = NULL;

Stack *StackInit(Stack *src) {
  // 未初始化
  if (!src) {
    src = (Stack *) malloc(sizeof(Stack));
    src->base = (DataType *) malloc(sizeof(DataType) * (MaxSize + 2));
    myassert(src->base);
    src->baseTop = src->base + MaxSize - 1;
    src->direction = 1;
    src->top = src->base - src->direction;
    src->bottom = src->base;
    src->overflow = 0;
    src->reverse = NULL;
    if (!sp_) sp_ = src;
    return src;
  }
  // 已经初始化，需要检查是否已经有反向栈
  if (src->reverse) return NULL;
  Stack *re = (Stack *) malloc(sizeof(Stack));
  src->reverse = re;
  re->reverse = src;
  re->direction = -src->direction;
  re->base = src->baseTop;
  re->baseTop = src->base;
  re->top = src->bottom;
  re->bottom = src->top;
  re->overflow = src->overflow;
  return re;
}

/**
 * 返回栈已使用长度
 * @param S 操作栈
 * @return 返回栈的长度
 */
size_t StackLength(const Stack *s) {
  // 特殊判断
  if (!s->overflow && s->top + s->direction == s->bottom) return 0;
  if (s->overflow && s->top == s->baseTop && s->bottom == s->base) return 0;
  // 注意方向和是否overflow
  if (s->overflow) return MaxSize - (s->bottom - s->top) * s->direction;
  return (s->top - s->bottom + s->direction) * s->direction;
}

/**
 * 获取栈的一个数组拷贝，顺序为从栈底到栈顶
 * @param S 操作栈
 * @param seq 栈中元素的一个拷贝
 */
void StackToArray(const Stack *s, DataType *seq) {
  DataType *data = seq;
  int passedOverFlow = 0;
  for (DataType *p = s->bottom; !StackEmpty(s) && (p != s->top + s->direction || (s->overflow && !passedOverFlow)); p += s->direction) {
    if (p == s->baseTop + s->direction && s->overflow) {
      p = s->base;
      passedOverFlow = 1;
    }
    *(data++) = *p;
  }

}

/**
 * 判断栈是否空，为空返回1，否则返回0
 * @param S
 * @return
 */
int StackEmpty(const Stack *s) {
  return StackLength(s) == 0;
}

/**
 * 判断栈是否满，为空返回1，否则返回0
 * @param S
 * @return
 */
int StackFull(const Stack *s) {
  return StackLength(s) == MaxSize;
}

/**
 * 向栈插入一个元素
 * @param S 操作栈
 * @param e 操作数
 * @return 成功返回1，否则返回0
 */
int Push(Stack *s, const DataType *e) {
  if (StackFull(s)) return 0;
#ifdef DEBUG
  printf(LOGFMT "Push(%s, %d)\n", __func__, __LINE__, (sp_ == s ? "S" : "R"), *e);
#endif
  Stack *r = s->reverse;
  s->top += s->direction;
  if (r) r->bottom -= r->direction;
  if (s->top == s->baseTop + s->direction) {
    s->top = s->base;
    s->overflow = !s->overflow;
    r->overflow = s->overflow;
    if (r) r->bottom = r->baseTop;
    LOG("OverFlow!");
  }
  *s->top = *e;
  return 1;
}

/**
 * 从栈中弹出一个元素
 * @param S 操作栈
 * @param e 接受栈弹出的值
 * @return 成功返回1，否则返回0
 */
int Pop(Stack *s, DataType *e) {
  if (StackEmpty(s)) return 0;
  Stack *r = s->reverse;
  *e = *s->top;
#ifdef DEBUG
  printf(LOGFMT "Pop(%s, %d)\n", __func__, __LINE__, (sp_ == s ? "S" : "R"), *e);
#endif
  s->top -= s->direction;
  if (r) r->bottom += r->direction;
  if (s->top == s->base - s->direction) {
    s->top = s->baseTop;
    s->overflow = !s->overflow;
    r->overflow = s->overflow;
    if (r) r->bottom = r->base;
    LOG("OverFlow!");
  }
  return 1;
}

/**
 * 获取栈顶元素，不删除该元素
 * @param S 操作栈
 * @param e 接受栈顶元素
 * @return 成功返回1，否则返回0
 */
int StackGetTop(const Stack *s, DataType *e) {
  if (StackEmpty(s)) return 0;
  *e = *s->top;
  return 1;
}

void StackDestroy(Stack *s) {
  if (!s) return;
  if (s->base) free(s->base);
  if (s->reverse) free(s->reverse);
  sp_ = NULL;
  free(s);
}