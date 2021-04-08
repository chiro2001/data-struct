//
// Created by Chiro on 2021/4/6.
//

#ifndef BINARY_TREE_CHIVECTOR_HPP
#define BINARY_TREE_CHIVECTOR_HPP

#include "linked_list.hpp"
#include "iterator_base.hpp"
#include <stdexcept>
#include <utility>
#include <initializer_list>

namespace chilib {
// 自己实现一个 vector 吧

template<typename T>
class vector : public iterator_base {
private:
  // 块链中每块大小
  // 争取每一块放到 64b or 128b 内
  const static size_t block_size = 80;

  // 数据区
  class block {
  private:
    void memory_set() {
      memset(mdata, 0, sizeof(mdata));
    }

  public:
    T mdata[block_size]{};

    block() {
      memory_set();
    };

    T &at(size_t pos) {
      return mdata[pos];
    }

    T &operator[](size_t pos) {
      return at(pos);
    }
  };

  linked_list_p <block> head = nullptr;
  linked_list_p <block> cnt = nullptr;
  size_t offset = 0;

  void create_blocks(size_t size) {
    block blk;
    size_t blk_size = size / block_size + 1;
    while (blk_size--) {
      head->emplace_back(&blk);
    }
  }

  void init(size_t size) {
    head = linked_list<block>::make_head();
    create_blocks(size);
    cnt = head->get_next();
    this->offset = size;
  }

  void memory_copy(T *src, size_t size, linked_list_p <block> target = nullptr) {
    if (!size) return;
    auto data = src;
    if (target == nullptr) target = this->head->get_next();
    auto p = std::move(target);
    while (size > block_size) {
      if (p == nullptr) return;
      memcpy(&p->get_data(), data, block_size);
      p = p->get_next();
      data += block_size;
      size -= block_size;
    }
    memcpy(&p->get_data(), data, size);
  }

  void list_copy(std::initializer_list<T> li) {
    auto it = this->begin();
    for (auto const &p : li) {
      *it = p;
      ++it;
    }
  }

  void range_check(size_t pos) {
    if (pos >= this->size()) {
      char buf[512];
      sprintf(buf, "vector::range_check: pos (which is %d) >= this->size() (which is %d)", pos, this->size());
      throw std::out_of_range(buf);
    }
  }

public:
  explicit vector(size_t size) {
    init(size);
  }

  explicit vector() {
    init(0);
  }

  vector(std::initializer_list<T> li) {
    auto s = li.size();
    init(s);
    list_copy(li);
  }

  void emplace_back(T &d) {
    push_back(d);
  }

  void emplace_back(T d) {
    push_back(d);
  }

  void emplace_back(T *d) {
    push_back(*d);
  }

  void push_back(T &d) {
    block blk;
    if (offset == block_size) {
      cnt->link(&blk);
      cnt = cnt->get_next();
      offset = 0;
    }
    cnt->get_data()[offset++] = d;
  }

  size_t length() {
    return head->length() * block_size + offset - block_size;
  }

  size_t size() {
    return this->length();
  }

  T &at(size_t pos) {
    this->range_check(pos);
    return this->head->step(pos / block_size)->get_data().at(pos % block_size);
  }

  class iterator : public iterator_shared_ptr<linked_list < block>>

  {
    private:
    size_t offset = 0;
    public:
//    size_t offset = 0;
    explicit iterator(linked_list_p<block>
    &d) : iterator_shared_ptr < linked_list < block >> (d) {}
    explicit iterator(linked_list_p<block>
    &d, size_t
    ofst) : iterator_shared_ptr < linked_list < block >> (d), offset(ofst)
    {}

    T &operator*() { return this->node->get_data().at(this->offset); }

    T *operator->() { return &(operator*()); }

    iterator &operator++()
    override{
            offset++;
            if (this->offset == block_size) {
              this->node = this->node->get_next();
              offset = 0;
            }
            return *this;
    }

    bool operator==(const iterator &s) {
      return this->node == s.node && this->offset == s.offset;
    }

    bool operator!=(const iterator &s) {
      return !(this->operator==(s));
    }
  };

  iterator begin() {
    auto p = this->head->get_next();
    return iterator(p, 0);
  }

  iterator end() {
    auto p = this->head->get_tail();
    return iterator(p, this->offset);
  }


};
}
//namespace chilib {
//typedef int T;
//// 块链中每块大小
//const size_t block_size = 128;
//class vector : public linked_list<T[block_size]> {
//
//};
//}
#endif //BINARY_TREE_CHIVECTOR_HPP
