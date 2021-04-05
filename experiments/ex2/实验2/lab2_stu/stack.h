#define MaxSize 40   // ��������
typedef int DataType;
typedef struct
{
    DataType data[MaxSize];
    int top; // ջ��ָ��
} Stack;     // �ṹ��������

void InitStack(Stack *S);
int StackEmpty(Stack S);
int GetTop(Stack S, DataType *e);
int Push(Stack *S, DataType e);
int Pop(Stack *S, DataType *e);
void StackToArray(Stack S, DataType *seq);
int StackLength(Stack S);
