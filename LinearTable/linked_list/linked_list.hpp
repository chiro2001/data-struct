#include <memory>
#include <iostream>

namespace chilib {

// ~双向~链表，C++智能指针实现
template<typename T>
class linked_list {
public:
  linked_list() = default;

  linked_list(linked_list<T> &li) = default;

  explicit linked_list(T &d) : data(d) { this->constructor(); }

  // 可能造成无法确定参数类型
//  explicit linked_list(T d) : data(d) { this->constructor(); }

  ~linked_list() { this->destructor(); }

  void constructor() {
    std::cout << "linked_list constructor data = " << this->data << std::endl;
  }

  void destructor() {
    std::cout << "linked_list destructor data = " << this->data << std::endl;
  }

  // 只有自己一个节点的时候也返回nullptr
  std::shared_ptr<linked_list<T>> get_tail() {
    auto p = this->next;
    if (p == nullptr) return nullptr;
    while (p->next != nullptr)
      p = p->next;
    return p;
  }

  // 接一个新节点到这个节点之后，本节点之后的数据被释放
  auto link(T &d) {
    std::shared_ptr<linked_list<T>> p = std::make_shared<linked_list<int>>(d);
    std::cout << "linked_list link to data " << p->data << std::endl;
    this->next = p;
    return p;
  }

  // 接一个已经存在的链表串到本节点之后
  // 本节点之后的数据接到参数节点之后
  // 参数节点之后的数据保留到串中
  std::shared_ptr<linked_list<T>> append(std::shared_ptr<linked_list<T>> &p) {
    if (p == nullptr) return nullptr;
    std::cout << "linked_list link to node " << p->data << std::endl;
    auto tail = p->get_tail();
    tail->next = this->next;
    this->next = p;
    return p;
  }

  // 接一个已经存在的链表串到本节点之后
  // 本节点之后的数据释放
  // 参数节点之后的数据保留
  std::shared_ptr<linked_list<T>> link(std::shared_ptr<linked_list<T>> &p) {
    if (p == nullptr) return nullptr;
    std::cout << "linked_list link to node " << p->data << std::endl;
    this->next = p;
    return p;
  }

  // 获取往下几个节点的指针
  // 不能返回自己的指针
  std::shared_ptr<linked_list<T>> step(size_t s) {
    if (!s) return nullptr;
    auto p = this->next;
    while (s) {
      if (p == nullptr) return p;
      p = p->next;
      s--;
    }
    return p;
  }

  // 防止出现循环引用现象
  // 暂时无法实现完全的双向链表功能
  // std::weak_ptr<linked_list<T>> pre;
  std::shared_ptr<linked_list<T>> next = nullptr;
  T data{};

};

// 外部可以使用 linked_list_p 做指针简单名字
template<typename T>
using linked_list_p = std::shared_ptr<linked_list<T>>;

// 从对象生成一个节点
template<typename T>
auto linked_list_make(T &d) {
  return std::make_shared<linked_list<int>>(d);
}

// 检查是否存在环
template<typename T>
bool linked_list_check_circulation(linked_list_p<T> &p) {
  // 快慢指针法
  auto p1 = p, p2 = p;
  do {
    p1 = p1->step(1);
    p2 = p2->step(2);
    if (p1 == nullptr || p2 == nullptr) return false;
  } while (p1 != p2);
  return true;
}

}   // namespace chilib