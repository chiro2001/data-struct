#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

void testStack(Stack S);
void testQueue(Queue Q);

int main(int argc, char **argv)
{
    Stack *S = (Stack *)malloc(sizeof(Stack));
    Queue *Q = (Queue *)malloc(sizeof(Queue));
    InitStack(S);
    InitQueue(Q);
    FILE *fp_in;
    char in_file[] = "./test_stu.in";
    fp_in = freopen(in_file, "r", stdin);
    if (fp_in == NULL)
    {
        printf("can not load file!");
        return 1;
    }
    int mode, num, e, status;
    while (~scanf("%d", &mode))
    {
        switch (mode)
        {
        case 0:
            num = 0;
            scanf("%d", &num);
            while (num > 0)
            {
                num--;
                scanf("%d", &e);
                status = Push(S, e);
                if(status)
                {
                    printf("Push:%d\n", e);
                }
                testStack(*S);
            }
            break;
        case 1:
            num = 0;
            scanf("%d", &num);
            while (num > 0)
            {
                num--;
                status = Pop(S, &e);
                if(status)
                {
                    printf("Pop:%d\n", e);
                }
                else
                {
                    printf("Pop failed\n");
                }
                testStack(*S);
            }
            break;
        case 2:
            status = GetTop(*S, &e);
            if(status)
            {
                printf("GetTop:%d\n", e);
            }
            else
            {
                printf("GetTop failed\n");
            }
            testStack(*S);
            break;
        case 3:
            status = StackEmpty(*S);
            if(status)
            {
                printf("The Stack is Empty\n");
            }
            else
            {
                printf("The Stack is not Empty\n");
            }
            testStack(*S);
            break;
        case 4:
            num = 0;
            scanf("%d", &num);
            while (num > 0)
            {
                num--;
                scanf("%d", &e);
                status = EnQueue(Q, e);
                if(status)
                {
                    printf("EnQueue:%d\n", e);
                }
                else
                {
                    printf("EnQueue failed\n");
                }
                testQueue(*Q);
            }
            break;
        case 5:
            num = 0;
            scanf("%d", &num);
            while (num > 0)
            {
                num--;
                status = DeQueue(Q, &e);
                if(status)
                {
                    printf("DeQueue:%d\n", e);
                }
                else
                {
                    printf("DeQueue failed\n");
                }
                testQueue(*Q);
            }
            break;
        case 6:
            status = GetHead(*Q, &e);
            if(status)
            {
                printf("GetHead:%d\n", e);
            }
            else
            {
                printf("GetHead failed\n");
            }
            testQueue(*Q);
            break;
        case 7:
            status = QueueEmpty(*Q);
            if(status)
            {
                printf("The Queue is Empty\n");
            }
            else
            {
                printf("The Queue is not Empty\n");
            }
            testQueue(*Q);
            break;
        default:
            break;
        }
    }
    return 0;
}

void testStack(Stack S)
{
    int seq[MaxSize], i = 0;
    StackToArray(S, seq);
    printf("Stack contains:");
    for (i = 0; i < StackLength(S); i++)
    {
        printf("%d ", seq[i]);
    }
    printf("\n\n");
}

void testQueue(Queue Q)
{
    int seq[MaxSize], i = 0;
    QueueToArray(Q, seq);
    printf("Queue contains:");
    for (i = 0; i < QueueLength(Q); i++)
    {
        printf("%d ", seq[i]);
    }
    printf("\n\n");
}
