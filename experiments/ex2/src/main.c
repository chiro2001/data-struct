#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "utils.h"

void testStack(Stack *s);

void testQueue(Queue *q);

int main(int argc, char **argv) {
  Stack *s = NULL;
  Queue *q = NULL;
  s = StackInit(s);
  q = QueueInit(q);
  FILE *fp_in;
  char in_file[] = "./test_stu.in";
  fp_in = freopen(in_file, "r", stdin);
  myassertM(fp_in, "can not load file!");
  int mode, num, e, status;
  while (~scanf("%d", &mode)) {
    switch (mode) {
      case 0:
        num = 0;
        scanf("%d", &num);
        while (num > 0) {
          num--;
          scanf("%d", &e);
          status = Push(s, &e);
          if (status) {
            printf("Push:%d\n", e);
          }
          testStack(s);
        }
        break;
      case 1:
        num = 0;
        scanf("%d", &num);
        while (num > 0) {
          num--;
          status = Pop(s, &e);
          if (status) {
            printf("Pop:%d\n", e);
          } else {
            printf("Pop failed\n");
          }
          testStack(s);
        }
        break;
      case 2:
        status = StackGetTop(s, &e);
        if (status) {
          printf("GetTop:%d\n", e);
        } else {
          printf("GetTop failed\n");
        }
        testStack(s);
        break;
      case 3:
        status = StackEmpty(s);
        if (status) {
          printf("The Stack is Empty\n");
        } else {
          printf("The Stack is not Empty\n");
        }
        testStack(s);
        break;
      case 4:
        num = 0;
        scanf("%d", &num);
        while (num > 0) {
          num--;
          scanf("%d", &e);
          status = QueueEn(q, &e);
          if (status) {
            printf("EnQueue:%d\n", e);
          } else {
            printf("EnQueue failed\n");
          }
          testQueue(q);
        }
        break;
      case 5:
        num = 0;
        scanf("%d", &num);
        while (num > 0) {
          num--;
          status = QueueDe(q, &e);
          if (status) {
            printf("DeQueue:%d\n", e);
          } else {
            printf("DeQueue failed\n");
          }
          testQueue(q);
        }
        break;
      case 6:
        status = QueueGetHead(q, &e);
        if (status) {
          printf("GetHead:%d\n", e);
        } else {
          printf("GetHead failed\n");
        }
        testQueue(q);
        break;
      case 7:
        status = QueueEmpty(q);
        if (status) {
          printf("The Queue is Empty\n");
        } else {
          printf("The Queue is not Empty\n");
        }
        testQueue(q);
        break;
      default:
        break;
    }
  }
  return 0;
}

void testStack(Stack *s) {
  int seq[MaxSize], i = 0;
  StackToArray(s, seq);
  printf("Stack contains:");
  for (i = 0; i < StackLength(s); i++) {
    printf("%d ", seq[i]);
  }
  printf("\n\n");
}

void testQueue(Queue *q) {
  int seq[MaxSize], i = 0;
  QueueToArray(q, seq);
  printf("Queue contains:");
  for (i = 0; i < QueueLength(q); i++) {
    printf("%d ", seq[i]);
  }
  printf("\n\n");
}

int main__(int argc, const char **argv) {
  Queue *q = NULL;
  q = QueueInit(q);
  DataType d;
  for (int i = 0; i < MaxSize; i++) {
    QueueEn(q, &i);
  }
  for (int i = 0; i < MaxSize; i++) {
    QueueDe(q, &d);
    PINT(d);
  }
  QueueDestroy(q);
}

const Stack *sp = NULL;

void printStackData(const Stack *s) {
#ifdef DEBUG
  if (!sp) sp = s;
  LOGS((sp == s) ? "S Start" : "R Start");
  for (DataType *p = s->base; p != s->baseTop + s->direction; p += s->direction) {
    printf("%c   ", ((s->top == s->bottom && s->top == p) ? 'P' : (p == s->top ? 'T' : (p == s->bottom ? 'B' : ' '))));
    fflush(stdout);
  }
  puts("");
  fflush(stdout);
  for (DataType *p = s->base; p != s->baseTop + s->direction; p += s->direction) {
    printf("%x  ", (int) ((size_t) p & 0xFF));
    fflush(stdout);
  }
  puts("");
  for (DataType *p = s->base; p != s->baseTop + s->direction; p += s->direction) {
    printf(((-100 < *p && *p < 100) ? "%-3d " : "%-3c "), ((-100 < *p && *p < 100) ? *p : (int) '%'));
    fflush(stdout);
  }
  puts("");
  LOGS((sp == s) ? "S End" : "R End");
#endif
}

void printStack(const Stack *s) {
#ifdef DEBUG
  printStackData(s);
  LOGS((sp == s) ? "S Start" : "R Start");
  if (s->bottom == s->top + s->direction && !s->overflow) {
    printf("<Empty Stack: %p>\n", s);
    fflush(stdout);
    return;
  }
  int passedOverFlow = 0;
  for (DataType *p = s->bottom; !StackEmpty(s) && (p != s->top + s->direction || (s->overflow && !passedOverFlow)); p += s->direction) {
    if (p == s->baseTop + s->direction && s->overflow) {
      p = s->base;
      passedOverFlow = 1;
    }
    printf("%x  ", (int) ((size_t) p & 0xFF));
    fflush(stdout);
  }
  passedOverFlow = 0;
  puts("");
  fflush(stdout);
  for (DataType *p = s->bottom; p != s->top + s->direction || (s->overflow && !passedOverFlow); p += s->direction) {
    if (p == s->baseTop + s->direction) {
      printf(" | ");
      p = s->base;
      passedOverFlow = 1;
    }
    printf(((-100 < *p && *p < 100) ? "%-3d " : "%-3c "), ((-100 < *p && *p < 100) ? *p : (int) '%'));
    fflush(stdout);
  }
  puts("");
  fflush(stdout);
  LOGS((sp == s) ? "S End" : "R End");
#endif
}

int mainStackTest(int argc, char **argv) {
  DataType dat[MaxSize];
  DataType d;
  Stack *s = NULL, *r = NULL;
  for (int i = 0; i < MaxSize; i++) dat[i] = i;
  s = StackInit(s);
  r = StackInit(s);
  PINT((int) StackLength(s));
  PINT(StackEmpty(s));
  printStack(s);
  printStack(r);
  for (int i = 0; i < MaxSize; i++) {
    Push(s, dat + i);
    printStack(s);
    printStack(r);
  }
  PINT((int) StackLength(s));
  PINT((int) StackLength(r));
  PINT((int) StackFull(s));
  PINT((int) StackFull(r));
  PINT(Pop(r, &d));
  printStack(s);
  printStack(r);
  d = -3;
  PINT(Push(s, &d));
  printStack(s);
  printStack(r);
  PINT(Pop(r, &d));
  printStack(s);
  printStack(r);
  d = -4;
  PINT(Push(s, &d));
  printStack(s);
  printStack(r);
  for (int i = 0; i < MaxSize; i++) {
    Pop(r, &d);
    PINT(d);
    PINT((int) StackLength(s));
    PINT((int) StackLength(r));
    printStack(s);
    printStack(r);
  }
  StackDestroy(s);
  sp = NULL;
  s = StackInit(NULL);
  r = StackInit(s);

  for (int i = 0; i < MaxSize; i++)
    Push(s, &i);
  printStack(s);
  for (int i = 0; i < MaxSize - 2; i++) {
    PINT(Pop(r, &d));
  }
  printStack(r);
  PINT(Pop(r, &d));
  printStack(r);
  PINT(Pop(r, &d));
  PINT((int) StackLength(r));
  printStack(r);
  return 0;
}