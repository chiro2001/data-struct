//
// 常用算法
// Created by Chiro on 2021/5/21.
//

#ifndef CHILIB_ALGO_HPP
#define CHILIB_ALGO_HPP

#include "queue.hpp"

namespace chilib {

template<typename T, typename Cmp = chilib::less<T>>
void qsort(chilib::vector<T> &data, int start = -1, int end = -1) {
  if (start == -1 & end == -1) start = 0, end = data.size() - 1;
  if (start >= end) return;
  T key = data[start];
  int i = start, j = end;
  while (i < j) {
    // 找到右边第一个比key小的数
    while (i < j && Cmp()(key, data[j])) j--;
    if (i < j) {
      data[i] = data[j];
      i++;
    }
    // 找到左边第一个比key大的数
    while (i < j && Cmp()(data[i], key)) i++;
    if (i < j) {
      data[j] = data[i];
      j--;
    }
  }
  data[i] = key;
  qsort(data, start, i - 1);
  qsort(data, j + 1, end);
}
}

#endif //CHILIB_ALGO_HPP
