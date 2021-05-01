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

template<typename T, typename Cmp = less<T>>
class priority_queue {
private:
  Cmp cmp;
  vector <T> data;

  constexpr static size_t father(size_t index) { return index / 2; }

  constexpr static size_t left(size_t index) { return index * 2; }

  constexpr static size_t right(size_t index) { return index * 2 + 1; }

  void range_check(size_t pos) {
    if (pos >= this->data.size()) {
      char buf[512];
      sprintf(buf, "priority_queue::range_check: pos (which is %d) >= this->size() (which is %d)", pos,
              this->data.size());
      throw std::out_of_range(buf);
    }
  }

  void empty_check() {
    if (empty()) throw std::out_of_range("priority_queue::empty_check: can not pop from empty container");
  }

  void shift_up(size_t index) {
    if (cmp(data[father(index)], data[index])) {
      std::swap(data[index], data[father(index)]);
      shift_up(father(index));
    }
  }

  void shift_down(size_t index) {
    if (left(index) >= data.length()) return;
    if (right(index) >= data.length()) {
      if (cmp(data[index], data[left(index)])) {
        std::swap(data[left(index)], data[index]);
        shift_down(left(index));
      }
      return;
    }
    if (cmp(data[right(index)], data[left(index)])) {
      if (cmp(data[index], data[left(index)])) {
        std::swap(data[left(index)], data[index]);
        shift_down(left(index));
      }
    } else {
      if (cmp(data[index], data[right(index)])) {
        std::swap(data[right(index)], data[index]);
        shift_down(right(index));
      }
    }
  }
public:
  void push(T d) {
    data.emplace_back(d);
    shift_up(data.length() - 1);
  }

  T &top() {
    empty_check();
    return data[0];
  }

  void pop() {
    empty_check();
    T back = data[data.length() - 1];;
    data.pop_back();
    if (data.empty()) return;
    data[0] = back;
    shift_down(0);
  }

  bool empty() { return data.empty(); }

  size_t size() { return data.size(); }
};

}

#endif //CHILIB_QUEUE_HPP
