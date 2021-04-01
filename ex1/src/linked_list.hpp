#include <iostream>
#include <memory>

namespace chilib {

// 单向链表，C++智能指针实现
template<typename T>
class linked_list {
public:
  linked_list() = default;

  linked_list(linked_list<T> &li) = default;

  explicit linked_list(T &d) : data(d) { this->constructor(); }

  ~linked_list() { this->destructor(); }

  void constructor() {
//    std::cout << "linked_list constructor data = " << this->data << std::endl;
  }

  void destructor() {
//    std::cout << "linked_list destructor data = " << this->data << std::endl;
  }

  std::shared_ptr<linked_list<T>> next = nullptr;  // 指向下一个节点的指针
  T data{};                                        // 当前数据

  // @prog    : 得到链表尾部
  // @notice  : 只有自己一个节点的时候也返回nullptr
  std::shared_ptr<linked_list<T>> get_tail() {
    auto p = this->next;
    if (p == nullptr) return nullptr;
    while (p->next != nullptr) p = p->next;
    return p;
  }

  // @prog    : 连接一个节点到this节点后面
  // @notice  : 这个传入的节点之后的数据被释放
  auto link(T &d) {
    std::shared_ptr<linked_list<T>> p = std::make_shared<linked_list<T>>(d);
//    std::cout << "linked_list link to data " << p->data << std::endl;
    this->next = p;
    return p;
  }

  // @prog    : 接一个已经存在的链表串到本节点之后
  // @notice  : 本节点之后的数据释放
  //            参数节点之后的数据保留
  std::shared_ptr<linked_list<T>> link(std::shared_ptr<linked_list<T>> &p) {
    if (p == nullptr) return nullptr;
//    std::cout << "linked_list link to node " << p->data << std::endl;
    this->next = p;
    return p;
  }

  // @prog     : 接一个已经存在的链表串到本节点之后
  // @notice   : 本节点之后的数据接到参数节点之后
  //             参数节点之后的数据保留到串中
  std::shared_ptr<linked_list<T>> append(std::shared_ptr<linked_list<T>> &p) {
    if (p == nullptr) return nullptr;
//    std::cout << "linked_list link to node " << p->data << std::endl;
    auto tail = p->get_tail();
    tail->next = this->next;
    this->next = p;
    return p;
  }

  // @prog    : 获取往下几个节点的指针
  // @notice  : 不能返回自己的指针
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
};

// 外部可以使用 linked_list_p 做指针简单名字
template<typename T>
using linked_list_p = std::shared_ptr<linked_list<T>>;

// @prog    : 从对象生成一个节点
template<typename T>
auto linked_list_make(T &d) {
  return std::make_shared<linked_list<T>>(d);
}

// @prog    : 检查是否存在环
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

// @prog    : 打印整条链表的数据
template<typename T>
void linked_list_print(linked_list_p<T> &head) {
  auto p = head;
  while (p != nullptr) {
    std::cout << p->data << std::endl;
    p = p->next;
  }
}

}  // namespace chilib