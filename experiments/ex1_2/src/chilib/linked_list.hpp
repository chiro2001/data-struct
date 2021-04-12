// Last modified at 2021-04-12 by Chiro

#ifndef CHILIB_LINKED_LIST_HPP
#define CHILIB_LINKED_LIST_HPP

#include <memory>
#include <stdexcept>
#include <initializer_list>
#include "iterator_base.hpp"
#include "chistring.hpp"

namespace chilib {

// 单向链表，C++智能指针实现
template<typename T>
class linked_list : public iterator_base {
private:
  std::shared_ptr<linked_list<T>> next = nullptr;  // 指向下一个节点的指针
  T data{};                                        // 当前数据

  void range_check(size_t pos) {
    if (pos >= this->size()) {
      char buf[512];
      sprintf(buf, "linked_list::range_check: pos (which is %d) >= this->size() (which is %d)", pos, this->size());
      throw std::out_of_range(buf);
    }
  }

public:
  linked_list() = default;

  linked_list(linked_list<T> &li) = default;

  explicit linked_list(T &d) : data(d) {}

  ~linked_list() = default;

  auto& get_next() { return next; }

  T &get_data() { return data; }

  /*!
   * 得到链表大小（长度）
   * @return 大小（长度）
   * @note 会忽略头结点
   */
  size_t size() {
    size_t s = 0;
    auto p = this->next;
    if (p == nullptr) return 0;
    while (p != nullptr) {
      p = p->next;
      s++;
    }
    return s;
  }

  /*!
   * 得到链表长度
   * @return 长度
   * @note 会忽略头结点
   */
  size_t length() {
    return this->size();
  }

  /*!
   * 得到链表尾部指针
   * @return
   */
  std::shared_ptr<linked_list<T>> get_tail() {
    auto p = this->next;
    if (p == nullptr) return nullptr;
    while (p->next != nullptr) p = p->next;
    return p;
  }

  /*!
   * 得到链表尾部的前一个指针
   * @return 节点指针
   */
  std::shared_ptr<linked_list<T>> get_tail_pre() {
    auto p = this->next;
    if (p == nullptr) return nullptr;
    if (p->next == nullptr) return nullptr;
    while (p->next->next != nullptr) p = p->next;
    return p;
  }

  /*!
   * 连接一个节点到this节点后面
   * @param d 所要添加的节点数据
   * @return 节点对应指针
   * @note 这个传入的节点之后的数据被释放
   */
  auto link(T &d) {
    auto p = linked_list::make(d);
    this->link(p);
    return p;
  }

  /*!
   * 连接一个节点到this节点后面
   * @param d 所要添加的节点数据
   * @return 节点对应指针
   * @note 这个传入的节点之后的数据被释放
   */
  auto link(T *d) {
    auto p = linked_list::make(*d);
    this->link(p);
    return p;
  }

  /*!
   * 接一个已经存在的链表串到本节点之后
   * @param p 链表串
   * @return 连接完成的链表串
   * @note 本节点之后的数据释放，参数节点之后的数据保留
   */
  std::shared_ptr<linked_list<T>> link(std::shared_ptr<linked_list<T>> &p) {
    if (p == nullptr) return nullptr;
    this->next = p;
    return p;
  }

  /*!
   * 接一个已经存在的链表串到本节点之后
   * @param p 链表串
   * @return 连接完成的链表串
   * @note 本节点之后的数据接到参数节点之后，参数节点之后的数据保留到串中
   */
  std::shared_ptr<linked_list<T>> append(std::shared_ptr<linked_list<T>> &p) {
    if (p == nullptr) return nullptr;
    auto tail = p->get_tail();
    tail->next = this->next;
    this->next = p;
    return p;
  }

  /*!
   * 获取往下几个节点的指针
   * @param s 往下 s + 1 个节点
   * @return 节点指针
   * @note 不能返回自己的指针
   */
  std::shared_ptr<linked_list<T>> step(size_t s = 0) {
    auto p = this->next;
    while (s--) {
      if (p == nullptr) return p;
      p = p->next;
    }
    return p;
  }

/*!
 * 从单个数据形成一个单节点链表
 * @deprecated 应该留出头指针
 * @param d 数据
 * @return 链表首节点
 */
  static auto make(T &d) {
    return std::make_shared<linked_list<T>>(d);
  }

  static auto make(std::initializer_list<T> li) {
    auto h = make_head();
    if (li.size() == 0) return h;
    auto p = h;
    for (auto d : li) {
      p = p->link(d);
    }
    return h;
  }

/*!
 * 形成链表头指针
 * @return 链表首节点
 */
  static auto make_head() {
    return std::make_shared<linked_list<T>>();
  }

/*!
 * 寻找链表是否存在环
 * @param p 链表
 * @return 是否存在环
 */
  static bool check_circulation(std::shared_ptr<linked_list<T>> &p) {
    // 快慢指针法
    auto p1 = p, p2 = p;
    do {
      p1 = p1->step(0);
      p2 = p2->step(1);
      if (p1 == nullptr || p2 == nullptr) return false;
    } while (p1 != p2);
    return true;
  }

/*!
 * 迭代器
 */
  class iterator : public iterator_shared_ptr<linked_list<T>> {
  public:
    explicit iterator(std::shared_ptr<linked_list<T>> d) : iterator_shared_ptr<linked_list<T>>(d) {}

    iterator &operator++() override {
      this->node = this->node->next;
      return *this;
    }
  };

/*!
 * 得到开始迭代器
 * @note 由于没法指向自己，所以只能指向下一个。使用的时候空出头结点
 * @return 迭代器
 */
  iterator begin() { return iterator(this->next); }

/*!
 * 得到尾部迭代器
 * @return 迭代器
 */
  iterator end() { return iterator(this->get_tail()->next); }

/*!
 * 向尾部添加元素
 * @param d 元素
 * @return 添加的节点指针
 */
  auto push_back(T &d) {
    auto p = this->make(d);
    auto tail = this->get_tail();
    if (tail == nullptr) this->link(p);
    else this->get_tail()->link(p);
    return p;
  }

/*!
 * 向尾部添加元素
 * @param d 元素
 * @return 添加的节点指针
 */
  auto emplace_back(T &d) {
    return this->push_back(d);
  }

/*!
 * 向尾部添加元素
 * @param d 元素
 * @return 添加的节点指针
 */
  auto emplace_back(T d) {
    return this->push_back(d);
  }

/*!
 * 向尾部添加元素
 * @param d 元素
 * @return 添加的节点指针
 */
  auto emplace_back(T *d) {
    return this->push_back(*d);
  }

/*!
 * 删除尾部元素
 */
  void pop_back() noexcept {
    auto pre = this->get_tail_pre();
    if (pre == nullptr) {
      if (this->next != nullptr) {
        this->next = nullptr;
      }
      return;
    }
    pre->next = nullptr;
  }

  T &at(size_t pos) {
    this->range_check(pos);
    auto p = this->step(pos);
    return p->data;
  }

  iterator insert(const iterator &it, T &d) {
    auto n = (*it)->get_next();
    auto p = (*it)->link(d);
    p->get_next() = n;
  }
};

// 外部可以使用 linked_list_p 做指针简单名字
template<typename T>
using linked_list_p = std::shared_ptr<linked_list<T>>;


}  // namespace chilib

#endif //CHILIB_LINKED_LIST_HPP