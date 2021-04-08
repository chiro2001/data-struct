//
// Created by Chiro on 2021/4/6.
//

#ifndef CHILIB_BASE_ITERATOR_HPP
#define CHILIB_BASE_ITERATOR_HPP

#include <memory>

namespace chilib {

// 基础可迭代对象
typedef int T;

class iterator_base {
public:
  template<class T>
  class iterator_ptr {
  public:
    using ref = T &;
    using ptr = T *;
    using self = iterator_ptr<T>;
    T *node;

    explicit iterator_ptr(T &d) : node(&d) {}

    ref operator*() { return *node; }

    ptr operator->() { return &(operator*()); }

    // ++it
    virtual self &operator++() = 0;

    bool operator==(const self &s) {
      return node == s.node;
    }

    bool operator!=(const self &s) {
      return node != s.node;
    }
  };

  template<class T>
  class iterator_shared_ptr {
  public:
    using ref = T &;
    using ptr = T *;
    using self = iterator_shared_ptr<T>;
    std::shared_ptr<T> node;

    explicit iterator_shared_ptr(std::shared_ptr<T> &d) : node(d) {}

    ref operator*() { return *node; }

    ptr operator->() { return &(operator*()); }

    // ++it
    virtual self &operator++() = 0;

    bool operator==(const self &s) {
      return node == s.node;
    }

    bool operator!=(const self &s) {
      return node != s.node;
    }
  };
};


}

#endif //CHILIB_BASE_ITERATOR_HPP
