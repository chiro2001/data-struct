#include <stddef.h>

#define MaxSize 40               // 数组容量
typedef int DataType;
typedef struct Stack_t {
  DataType *base, *baseTop;      // 栈基址和栈最大地址指针（最大地址加一）
  struct Stack_t *reverse;       // 对应反向栈
  DataType *top, *bottom;        // 栈顶指针和栈底指针
  int overflow;                  // 是否栈顶已经回到 base 而栈底还没到 baseTop
  int direction;                 // 栈增加方向
} Stack;                         // 结构体类型名

Stack *StackInit(Stack *src);

void StackDestroy(Stack *s);

int StackEmpty(const Stack *s);

int StackFull(const Stack *s);

int StackGetTop(const Stack *s, DataType *e);

int Push(Stack *s, const DataType *e);

int Pop(Stack *s, DataType *e);

void StackToArray(const Stack *s, DataType *seq);

size_t StackLength(const Stack *s);
