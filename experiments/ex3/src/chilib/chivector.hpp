//
// Created by Chiro on 2021/4/6.
//

#ifndef BINARY_TREE_CHIVECTOR_HPP
#define BINARY_TREE_CHIVECTOR_HPP

#include <iostream>
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
    T mdata[block_size];

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

  linked_list_p<block> head = nullptr;
  linked_list_p<block> cnt = nullptr;
  size_t offset_end = 0, offset_start = 0;

  void create_blocks(size_t size) {
    block blk;
    size_t blk_size = size / block_size + 1;
    while (blk_size--) {
      head->emplace_back(&blk);
    }
  }

  void init_without_offset(size_t size) {
    head = linked_list<block>::make_head();
    create_blocks(size);
    cnt = head->get_next();
  }

  void init(size_t size) {
    init_without_offset(size);
    // 移动尾指针会导致增加默认的元素
    this->offset_end = size % block_size;
  }

//  void memory_copy(T &src, size_t size, linked_list_p<block> target = nullptr) {
//    if (!size) return;
//    auto data = src;
//    if (target == nullptr) target = this->head->get_next();
//    auto p = std::move(target);
//    while (size > block_size) {
//      if (p == nullptr) return;
//      memcpy(&p->get_data(), data, block_size);
//      p = p->get_next();
//      data += block_size;
//      size -= block_size;
//    }
//    memcpy(&p->get_data(), data, size);
//  }

  void list_init_copy(std::initializer_list<T> &li) {
    for (auto const &p : li) {
      T t = p;
      this->emplace_back(t);
    }
  }

  void range_check(size_t pos) {
    if (pos >= this->size()) {
      char buf[512];
      sprintf_s(buf, "vector::range_check: pos (which is %zd) >= this->size() (which is %zd)", pos, this->size());
      throw std::out_of_range(buf);
    }
  }

  void empty_check() {
    if (empty()) throw std::out_of_range("vector::empty_check: can not pop from empty container");
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
    init_without_offset(s);
    list_init_copy(li);
  }

  void emplace_back(T &d) {
    push_back(d);
  }

  void emplace_front(T &d) {
    push_front(d);
  }

  void push_back(T &d) {
    if (offset_end == block_size) {
      block blk;
      cnt->link(blk);
      cnt = cnt->get_next();
      offset_end = 0;
    }
    cnt->get_data()[offset_end++] = d;
  }

  void push_front(T &d) {
    if (offset_start == 0) {
      block blk;
      auto p = linked_list<block>::make(blk);
      head->insert(p);
      offset_start = block_size;
    }
    head->get_next()->get_data()[--offset_start] = d;
  }

  T pop_back() {
    empty_check();
    T d = cnt->get_data()[--offset_end];
    if (offset_end == 0) {
      if (head->get_next() != cnt) {
        // TODO: 减小复杂度
        cnt = head->get_tail_pre();
        cnt->get_next() = nullptr;
      }
    }
    return d;
  }

  T &back() {
    empty_check();
    return (*this)[this->length() - 1];
  }

  T &front() {
    empty_check();
    return (*this)[0];
  }

  T pop_front() {
    empty_check();
    T d = head->get_next()->get_data()[offset_start++];
    if (offset_start == block_size) {
      if (head->get_next() != cnt) {
        head->erase_next();
      }
    }
    return d;
  }

  size_t length() {
    //          完整的块            开头部分可能不完整的块       结尾不满足一块的部分
    return (head->length() - 1) * block_size - offset_start + offset_end;
  }

  bool empty() {
    return this->length() == 0;
  }

  size_t size() {
    return this->length();
  }

  T &at(size_t pos) {
    this->range_check(pos);
    pos += offset_start;
    return this->head->step(pos / block_size)->get_data().at(pos % block_size);
  }

  T &operator[](size_t pos) {
    return this->at(pos);
  }

  class iterator : public iterator_shared_ptr<linked_list<block>> {
  private:
    size_t offset = 0;
  public:
//    size_t offset = 0;
    explicit iterator(linked_list_p<block>
                      &d) : iterator_shared_ptr<linked_list<block >>(d) {}

    explicit iterator(linked_list_p<block>
                      &d, size_t
                      ofst) : iterator_shared_ptr<linked_list<block >>(d), offset(ofst) {}

    T &operator*() { return this->node->get_data().at(this->offset); }

    T *operator->() { return &(operator*()); }

    iterator &operator++()
    override {
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

  iterator begin() const {
    auto p = this->head->get_next();
    return iterator(p, this->offset_start);
  }

  iterator end() const {
    auto p = this->head->get_tail();
    if (this->offset_end != block_size) {
      return iterator(p, this->offset_end);
    } else {
      p = p->get_next();
      return iterator(p, 0);
    }
  }


};
} // namespace chilib

#endif //BINARY_TREE_CHIVECTOR_HPP
