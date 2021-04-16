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
  // 争取每一块放到 64b or 128b 内，方便 cpu 移动
  const static size_t block_size = 80;

  // 数据区块
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

  // 仅仅初始化内存，不增加元素
  void init_without_offset(size_t size) {
    head = linked_list<block>::make_head();
    create_blocks(size);
    cnt = head->get_next();
  }

  // 初始化内存并且增加默认值元素
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

  // 由传入的initializer_list初始化数据，仍然有优化空间
  void list_init_copy(std::initializer_list<T> &li) {
    for (auto const &p : li) {
      T t = p;
      this->emplace_back(t);
    }
  }

  // 检查是否在范围内，不在范围内则抛出异常
  void range_check(size_t pos) {
    if (pos >= this->size()) {
      char buf[512];
      sprintf_s(buf, "vector::range_check: pos (which is %zd) >= this->size() (which is %zd)", pos, this->size());
      throw std::out_of_range(buf);
    }
  }

  // 检查是否为空，为空则抛出异常
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

  /*!
   * 向容器末尾添加元素（实际上还是push）
   * @param d 元素的引用
   */
  void emplace_back(T &d) {
    push_back(d);
  }

  /*!
   * 向容器前端添加元素（实际上还是push）
   * @param d 元素的引用
   */
  void emplace_front(T &d) {
    push_front(d);
  }

  /*!
   * 向容器末尾添加元素
   * @param d 元素的引用
   */
  void push_back(T &d) {
    if (offset_end == block_size) {
      block blk;
      cnt->link(blk);
      cnt = cnt->get_next();
      offset_end = 0;
    }
    cnt->get_data()[offset_end++] = d;
  }

  /*!
   * 向容器开头添加元素
   * @param d 元素的引用
   */
  void push_front(T &d) {
    if (offset_start == 0) {
      block blk;
      auto p = linked_list<block>::make(blk);
      head->insert(p);
      offset_start = block_size;
    }
    head->get_next()->get_data()[--offset_start] = d;
  }

  /*!
   * 弹出并且返回容器末端元素
   * @return 容器末端元素实体
   */
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

  /*!
   * 弹出并且返回容器前端元素
   * @return 容器前端元素实体
   */
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

  /*!
   * 获取末端元素引用
   * @return 末端元素引用
   */
  T &back() {
    empty_check();
    return (*this)[this->length() - 1];
  }

  /*!
   * 获取前端元素引用
   * @return 前端元素引用
   */
  T &front() {
    empty_check();
    return (*this)[0];
  }

  /*!
   * 得到容器大小
   * @return 容器大小
   */
  size_t length() {
    //          完整的块            开头部分可能不完整的块       结尾不满足一块的部分
    return (head->length() - 1) * block_size - offset_start + offset_end;
  }

  /*!
   * 判断容器是否为空
   * @return 容器为空吗
   */
  bool empty() {
    return this->length() == 0;
  }

  /*!
   * 获取容器大小
   * @return 容器大小
   */
  size_t size() {
    return this->length();
  }

  /*!
   * 获取容器实际占内存的字节大小
   * @return 实际占内存大小
   */
  size_t size_memory() {
    return head->length() * block_size;
  }

  /*!
   * 按照位置获取元素
   * @param pos 位置
   * @return 元素的引用
   */
  T &at(size_t pos) {
    this->range_check(pos);
    pos += offset_start;
    return this->head->step(pos / block_size)->get_data().at(pos % block_size);
  }

  T &operator[](size_t pos) {
    return this->at(pos);
  }

  /*!
   * 迭代器类，是智能指针类型
   */
  class iterator : public iterator_shared_ptr<linked_list<block>> {
  private:
    size_t offset = 0;
  public:
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
      // 节点相同而且偏移相同才行
      return this->node == s.node && this->offset == s.offset;
    }

    bool operator!=(const iterator &s) {
      return !(this->operator==(s));
    }
  };

  /*!
   * 获取起始迭代器
   * @return 起始迭代器
   */
  iterator begin() const {
    auto p = this->head->get_next();
    return iterator(p, this->offset_start);
  }

  /*!
   * 获取末尾迭代器
   * @return 末尾迭代器
   */
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
