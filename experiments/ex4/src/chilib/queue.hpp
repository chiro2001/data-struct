//
// Created by Chiro on 2021/5/1.
//

#ifndef CHILIB_QUEUE_HPP
#define CHILIB_QUEUE_HPP

#include "chivector.hpp"

namespace chilib {

template<typename T>
struct less {
  // 伪函数
  constexpr bool operator()(const T &x, const T &y) const { return x < y; }
};

template<typename T>
struct greater {
  // 伪函数
  constexpr bool operator()(const T &x, const T &y) const { return x > y; }
};

class priority_queue {
public:
  using T = int;
  vector<T> data;

  constexpr static size_t father(size_t index) { return index / 2; }

  void shift_up(size_t index) {
    if (data[index] < data[])
  }
};

}

#endif //CHILIB_QUEUE_HPP
