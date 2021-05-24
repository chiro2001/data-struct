#include <stdio.h>
#include <stdlib.h>

static inline void swap(int *a, int *b) {
  static int t = 0;
  t = *a;
  *a = *b;
  *b = t;
}

void quick_sort(int *data, int start, int end) {
  if (start >= end) return;
  int key = data[start];
  int i = start, j = end - 1;
  while (i < j) {
    // 找到右边第一个比key小的数
    while (i < j && data[j] > key) j--;
    if (i < j) {
      data[i] = data[j];
      i++;
    }
    // 找到左边第一个比key大的数
    while (i < j && data[i] < key) i++;
    if (i < j) {
      data[j] = data[i];
      j--;
    }
  }
  data[i] = key;
//  printf("now: ");
//  for (int k = 0; k <= right - left; k++) printf("%d ", data[k]);
//  puts("");
  quick_sort(data, start, i - 1);
  quick_sort(data, j + 1, end);
}

/*
5
2 4 1 3 5
6
1 2 5 3 8 9
*/

int main() {
  int n, branch;
  freopen("5_1_input_5.in", "r", stdin);
  scanf("%d", &branch);
  while (branch--) {
    scanf("%d", &n);
    int *data = malloc(sizeof(int) * n);
    if (!data) return 1;
    for (int i = 0; i < n; i++)
      scanf("%d", data + i);
    quick_sort(data, 0, n - 1);
    printf("%d\n", data[n / 2]);
//    printf("now: ");
//    for (int k = 0; k < n; k++) printf("%d ", data[k]);
//    puts("");
    free(data);
  }
  return 0;
}