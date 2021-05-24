#include <cstdio>
#include <vector>

void split_array(int *data, size_t size) {
  size_t i = 0, j = size - 1;
  while (i < j) {
    while (i < j && data[i] < 0) i++;
    while (i < j && data[j] > 0) j--;
    std::swap(data[i], data[j]);
  }
}

int main() {
  int a[] = {-2, 2, -8, 9, 10, -4, 8, -10};
  split_array(a, sizeof(a) / sizeof(int));
  for (int i = 0; i < sizeof(a) / sizeof(int); i++) printf("%d ", a[i]);
  return 0;
}