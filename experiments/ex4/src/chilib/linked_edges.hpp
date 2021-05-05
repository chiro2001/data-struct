//
// Created by Chiro on 2021/5/5.
//

#ifndef CHILIB_LINKED_EDGES_HPP
#define CHILIB_LINKED_EDGES_HPP

#include "chivector.hpp"

namespace chilib {

// 链式前向星
class linked_edges : chilib::iterator_base {
public:
  // 边对象
  class edge_node {
  public:
    int to = 0;
    int weight = INT32_MAX;
    int next = 0;
    int line{};

    edge_node() = default;

    explicit edge_node(int to_, int weight_, int next_, int line_) : to(to_), weight(weight_), next(next_),
                                                                     line(line_) {}

    // 重载输出
    friend std::ostream &operator<<(std::ostream &out, const edge_node &node) {
      out << "node(line=" << node.line << ", to=" << node.to << ", weight=" << node.weight << ", next=" << node.next
          << ")";
      return out;
    }
  };

private:
  size_t edge_size;
  size_t node_size = 0;
  std::shared_ptr<chilib::vector<edge_node>> edges = nullptr;
  std::shared_ptr<chilib::vector<int>> heads = nullptr;
  int cnt = 0;
  int m_from{};

  void data_init() {
    heads = std::make_shared<chilib::vector<int>>(edge_size);
    edges = std::make_shared<chilib::vector<edge_node>>(edge_size);
  }


public:
  size_t &get_node_size() { return node_size; }

  /*!
   * 设置容器遍历的边起点节点
   * @param from 起点节点
   * @return 已经改变之后的链式前向星对象引用
   */
  linked_edges &set_from(int from) {
    // 并不线程安全呢
    m_from = from;
    return *(this);
  }

  explicit linked_edges(size_t size) : edge_size(size) { data_init(); }

  void add_edge(int from, int to, int weight, int line) {
    cnt++;
    (*edges)[cnt] = edge_node(to, weight, (*heads)[from], line);
//    std::cout << "add_edge: from=" << from << ",\t" << edges[cnt] << std::endl;
    (*heads)[from] = cnt;
  }

  int get_head_first() {
    int head_first = 0;
    while ((*heads)[head_first] == 0) head_first++;
    return head_first;
  }

  /*!
   * 迭代器类
   */
  class iterator : public iterator_instance<size_t> {
  private:
    chilib::vector<edge_node> &edges;
  public:
    explicit iterator(chilib::vector<edge_node> &edges_, size_t index) : iterator_instance(index), edges(edges_) {}

    iterator &operator++() {
      this->node = edges[this->node].next;
      return *this;
    }

    edge_node &operator*() {
      return edges[this->node];
    }
  };

  /*!
   * 获取起始迭代器
   * @return 起始迭代器
   */
  iterator begin() {
    return iterator(*edges, (*heads)[m_from]);
  }

  /*!
   * 获取末尾迭代器
   * @return 末尾迭代器
   */
  iterator end() {
    return iterator(*edges, 0);
  }
};

}

#endif //CHILIB_LINKED_EDGES_HPP
