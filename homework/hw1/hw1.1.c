#include <stdio.h>

typedef int ElemType;

ElemType FindMid(ElemType *s1, ElemType *s2, size_t length) {
  ElemType mid;
  for (size_t i = 0; i < length; i++) {
    if (*s1 < *s2) {
      mid = *s1;
      s1++;
    } else {
      mid = *s2;
      s2++;
    }
  }
  return mid;
}

int main() {
  int s1[] = {11, 13, 15, 17, 19}, s2[] = {2, 4, 6, 8, 20};
  printf("%d\n", FindMid(s1, s2, sizeof(s1) / sizeof(int)));
  return 0;
}