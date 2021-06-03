#include <stdio.h>

void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

#define getleft (index * 2)
#define getright (index * 2 + 1)
#define cmp(a, b) (a > b)

void minHeapify(int *d, int index, int size) {
  int left = getleft, right = getright;
  int target = index;
  while (1) {
    if (left < size && cmp(d[target], d[left])) target = left;
    if (right < size && cmp(d[target], d[right])) target = right;
    if (index == target) break;
    swap(d + target, d + index);
    index = target;
    left = getleft, right = getright;
  }
}

void buildHeap(int *d, int size) {
  for (int i = size / 2; i >= 0; i--) minHeapify(d, i, size);
}

void shiftDown(int *d, int index, int size) {
  int left = getleft, right = getright;
  if (right >= size) {
    if (cmp(d[index], d[left])) {
      swap(&d[left], &d[index]);
      shiftDown(d, left, size);
    }
    return;
  }
  if (cmp(d[right], d[left])) {
    if (cmp(d[index], d[left])) {
      swap(&d[left], &d[index]);
      shiftDown(d, left, size);
    }
  } else {
    if (cmp(d[index], d[right])) {
      swap(&d[right], &d[index]);
      shiftDown(d, right, size);
    }
  }
}

int pop(int *d, int size) {
  int res = d[0];
  d[0] = d[--size];
  shiftDown(d, 0, size);
  return res;
}

int main() {
  int a[] = {1, 7, 3, 6, 43, 3, 9, 5};
  int size = sizeof(a) / sizeof(int);
  buildHeap(a, size);
  for (int i = 0; i < size; i++) printf("%d ", a[i]);
  puts("");
  while (size) {
    printf("%d ", pop(a, size));
    size--;
  }
  return 0;
}