#include <stdio.h>

#define PINT(x) printf("[Line %-3d] " #x " = %d\n", __LINE__, x)

int findFather(int i, int j) {
  if (i <= 1 || j <= 1) return 0;
  if (i == j) return i / 2;
  int t = 1;
  // 使得 i 小于等于 j
  if (i > j) {
    int tmp = i;
    i = j;
    j = tmp;
  }
  if (2 <= i && i <= 3) {
    // 特殊判断边界
    t = 2;
  } else {
    // 得到i的高度
    while (t * 2 < i) {
      t *= 2;
    }
  }
  // 降低到同一层
  while (j > t * 2) j /= 2;
  if (i == j) return i / 2;
  while (i != j) {
    i /= 2;
    j /= 2;
  }
  return i;
}

int main() {
  PINT(findFather(10, 17));
  PINT(findFather(2, 11));
  PINT(findFather(4, 8));
  PINT(findFather(4, 4));
  PINT(findFather(2, 3));
  PINT(findFather(100, 100000));
  return 0;
}