/*
设计算法:
已知两个整型数组A和B，A中的元素从小到大排列，B中的元素从大到小排列，
A和B不含相同的元素，并已知数组A的元素个数为na，数组B的元素个数为nb，
设计一个算法（源代码）将A和B合并成为一个数组，
合并后的结果放在A中，且A仍保持从小到大排列，不另设新的数组。
要求算法的时间复杂度控制在O(n)/O(na+nb)，辅助空间为O(1)。
*/

#include <stdio.h>
#define DA \
  { 1, 4, 6, 30, 48 }
#define DAN 5

/*
int main() {
  int b[] = {90, 87, 10, 5, 2};
  int a[sizeof(b) / sizeof(int) + DAN] = DA;
  int na = DAN, nb = sizeof(b) / sizeof(int);
  int *pa = a, *pb = b + nb - 1;
  int *dist = a + DAN - 1;
  int cnt = 0;
  int dir = 1;
  while (cnt < nb + DAN) {
    // 选小的
    printf("[cnt %2d] = ", cnt);
    if (*pa < *pb) {
      printf("a[%d]\n", pa - a);
      *(dist += dir) = *(pa++);
    } else {
      printf("b[%d]\n", pb - b);
      *(dist += dir) = *(pb--);
    }
    if (dist == a + DAN + nb) {
      dist = b + nb - 1;
      dir = -1;
    }
    cnt++;
    printf("\ta[]: ");
    for (int i = 0; i < nb + DAN; i++) {
      printf("%d ", a[i]);
    }
    printf("\n\tb[]: ");
    for (int i = 0; i < nb; i++) {
      printf("%d ", b[i]);
    }
    puts("");
  }
  for (int i = 0; i < nb + DAN; i++) {
    printf("%d ", a[i]);
  }
  return 0;
}
*/

void swap(int *i, int *j) {
  int t = *i;
  *i = *j;
  *j = t;
}

int main() {
  int b[] = {90, 87, 10, 5, 2};
  int a[sizeof(b) / sizeof(int) + DAN] = DA;
  int na = DAN, nb = sizeof(b) / sizeof(int);
  // 让b和a在前部分交叉增大
  // 使得a取长的一段，b取短的一段
  int *aa = na >= nb ? a : b, *bb = na < nb ? a : b;
  // 现在轮到a(0)还是b(1)
  // int now = aa[0] <= bb[0] ? 0 : 1;
  if (aa[0] <= bb[0]) swap(aa, bb);
  int cnt_b = 0, index_a = 1;
  while (cnt_b < nb) {
    int index = nb - cnt_b - 1;
    int index_a_now = index_a;
    int need_swap = 0;
    while (aa[index_a_now] < bb[index]) {
      index_a_now++;
      need_swap = 1;
    }
    if (need_swap) {
      swap(aa + index_a, bb + index);
      swap(aa + )
    }
    cnt_b++;
  }
}