#include <stdio.h>

typedef int ElemType;

#define myMax(x, y) (x > y ? x : y)
#define myMin(x, y) (x < y ? x : y)

void FindMaxMin1(ElemType *data, size_t size, ElemType *max, ElemType *min) {
  // 二分法
  ElemType mx[2], mn[2];
  // 临界条件
  if (size == 2) {
    *max = myMax(data[0], data[1]);
    *min = myMin(data[0], data[1]);
    return;
  } else if (size == 1) {
    *max = data[0];
    *min = data[0];
    return;
  }
  FindMaxMin1(data, size / 2, mx, mn);
  FindMaxMin1(data + size / 2, size / 2 + size % 2, mx + 1, mn + 1);
  *max = myMax(mx[0], mx[1]);
  *min = myMin(mn[0], mn[1]);
}

void FindMaxMin2(ElemType *data, size_t size, ElemType *max, ElemType *min) {
  *max = *data, *min = *data;
  while (size) {
    if (*data > *max) *max = *data;
    if (*data < *min) *min = *data;
    data++;
    size--;
  }
}

int main() {
  int data[] = {1, 2, 3, 4, 10, 6, 7, 8, 9, 11};
  int max, min;
  FindMaxMin2(data, sizeof(data) / sizeof(int), &max, &min);
  printf("max = %d\tmin = %d\n", max, min);
}