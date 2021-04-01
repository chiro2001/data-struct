#define MaxSize 40   // 数组容量
typedef int DataType;
typedef struct
{
    DataType data[MaxSize];
    int top; // 栈顶指针
} Stack;     // 结构体类型名

void InitStack(Stack *S);
int StackEmpty(Stack S);
int GetTop(Stack S, DataType *e);
int Push(Stack *S, DataType e);
int Pop(Stack *S, DataType *e);
void StackToArray(Stack S, DataType *seq);
int StackLength(Stack S);
