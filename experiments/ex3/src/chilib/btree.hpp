//
// Created by Chiro on 2021/4/6.
//

#ifndef CHILIB_BTREE_HPP
#define CHILIB_BTREE_HPP

#include <memory>
#include <utility>
#include <sstream>
#include "chivector.hpp"
#include <iostream>

namespace chilib {
using std::shared_ptr;

template<typename T>
class btree {
public:
  explicit btree(T val) : data(val) {}

  explicit btree(const T *val) : data(*val) {}

  btree() = default;

  // 重载输出
  friend std::ostream &operator<<(std::ostream &out, btree &tr) {
    out << "Tree(" << tr.get_data() << ", ";
    if (tr.child_left != nullptr) out << *(tr.child_left) << ", ";
    else out << "nullptr, ";
    if (tr.child_right != nullptr) out << *(tr.child_right);
    else out << "nullptr";
    out << ")";
    return out;
  }

  static shared_ptr<btree> make(T &d) {
    auto p = new btree(d);
    return std::shared_ptr<btree>(p);
  }

  T data{};
  // 是否线索化
  bool is_in_threading = false;
  // 是否有左右孩子，在线索化的时候使用
  bool tag_left = false, tag_right = false;
  // 层数
  int height = -1;
  shared_ptr<btree> child_left = nullptr, child_right = nullptr;

  T &get_data() { return data; }

  shared_ptr<btree> &get_left() { return child_left; }

  shared_ptr<btree> &get_right() { return child_right; }

  bool is_leaf() {
    return this->get_left() == nullptr && this->get_right() == nullptr;
  }

  // 中序遍历本树，遍历到 callback 返回 false 为止
  template<typename F>
  void traversal_inorder(F const &callback) {
    if (!is_in_threading) {
      if (child_left != nullptr)
        child_left->traversal_inorder(callback);
      if (!callback(*this)) return;
      if (child_right != nullptr)
        child_right->traversal_inorder(callback);
    }
  }

  // 前序遍历本树
  template<typename F>
  void traversal_preorder(F const &callback) {
    if (!is_in_threading) {
      if (!callback(*this)) return;
      if (child_left != nullptr)
        child_left->traversal_preorder(callback);
      if (child_right != nullptr)
        child_right->traversal_preorder(callback);
    }
  }

  // 后序遍历本树
  template<typename F>
  void traversal_postorder(F const &callback) {
    if (!is_in_threading) {
      if (child_left != nullptr)
        child_left->traversal_postorder(callback);
      if (child_right != nullptr)
        child_right->traversal_postorder(callback);
      if (!callback(*this)) return;
    }
  }

  // 广度优先遍历
  template<typename F1, typename F2>
  static void
  traversal_bfs(shared_ptr<btree> &tree, F1 const &on_item, F2 const &on_layer, bool includes_null = false) {
    if (tree == nullptr) return;
    vector<shared_ptr<btree> *> q;
    int height = 0;
    tree->height = height;
//    std::cout << "got tree: " << *tree << std::endl;
    shared_ptr<btree> *tree_temp = &tree;
    q.emplace_back(tree_temp);
    bool new_layer = false;
    bool end_all = false;
    while (!q.empty()) {
//      std::cout << "    tree now: " << *tree << std::endl;
      shared_ptr<btree> *top = q.front();
//      if (*top != nullptr) std::cout << "got top: " << **top << std::endl;
//      else std::cout << "got top: nullptr" << std::endl;
      if (!on_item(*top, end_all)) {
        if (end_all) return;
        q.pop_front();
        continue;
      }
      if (end_all) return;
//      std::cout << "\ttop now: " << **top << std::endl;
      if (new_layer) {
        if (!on_layer(*top, height + 1, end_all)) {
          if (end_all) return;
          continue;
        }
        new_layer = false;
        if (end_all) return;
      }
      if (includes_null && *top == nullptr) {
        q.pop_front();
        continue;
      }
      shared_ptr<btree> left = (*top)->get_left(), right = (*top)->get_right();
      if ((*top)->height > height) height = (*top)->height;
      if ((*top)->get_left() != nullptr || includes_null) {
        shared_ptr<btree> *tmp = &(*top)->get_left();
        if (!includes_null) (*tmp)->height = height + 1;
        q.emplace_back(tmp);
      }
      if ((*top)->get_right() != nullptr || includes_null) {
        shared_ptr<btree> *tmp = &(*top)->get_right();
        if (!includes_null) (*tmp)->height = height + 1;
        q.emplace_back(tmp);
      }
      q.pop_front();
      if (!q.empty()) {
        // 即将到下一层
        if (!includes_null && (*q[0])->height > height) {
          new_layer = true;
        }
      }
    }
  }

  // 深度优先遍历
  template<typename F>
  void traversal_dfs_(F const &callback, vector<btree<T>> &stack) {
    stack.emplace_back(*this);
    if (!callback(*this, stack)) return;
    if (this->get_left() != nullptr) this->get_left()->traversal_dfs_(callback, stack);
    if (this->get_right() != nullptr) this->get_right()->traversal_dfs_(callback, stack);
    stack.pop_back();
  }

  template<typename F>
  void traversal_dfs(F const &callback) {
    vector<btree<T>> stack;
    traversal_dfs_(callback, stack);
  }
};

}

#endif //CHILIB_BTREE_HPP
