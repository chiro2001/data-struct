//
// Created by Chiro on 2021/5/1.
// 优先队列
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

// 默认使用 less 的比较函数
template<typename T, typename Cmp = less<T>>
class priority_queue {
private:
  Cmp cmp;
  vector <T> data;

  constexpr static size_t father(size_t index) { return index / 2; }

  constexpr static size_t left(size_t index) { return index * 2; }

  constexpr static size_t right(size_t index) { return index * 2 + 1; }

  void empty_check() {
    if (empty()) throw std::out_of_range("priority_queue::empty_check: can not pop from empty container");
  }

  /*!
   * 上浮操作
   * @param index 操作节点
   */
  void shift_up(size_t index) {
    if (cmp(data[father(index)], data[index])) {
      std::swap(data[index], data[father(index)]);
      shift_up(father(index));
    }
  }

  /*!
   * 下沉操作
   * @param index 操作节点
   */
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
  /*!
   * 向队列尾部添加元素
   * @param d 元素
   */
  void push(T d) {
    data.emplace_back(d);
    shift_up(data.length() - 1);
  }

  /*!
   * 取队列头元素
   * @return 元素引用
   */
  T &top() {
    empty_check();
    return data[0];
  }

  /*!
   * 弹出队列头元素
   */
  void pop() {
    empty_check();
    T back = data[data.length() - 1];;
    data.pop_back();
    if (data.empty()) return;
    data[0] = back;
    shift_down(0);
  }

  /*!
   * 判断队列是否为空
   * @return 是否为空
   */
  bool empty() { return data.empty(); }

  /*!
   * 取得队列长度
   * @return 队列长度
   */
  size_t size() { return data.size(); }
};

}

#endif //CHILIB_QUEUE_HPP
