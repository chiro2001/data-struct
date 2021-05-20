//
// Created by Chiro on 2021/4/6.
//

#ifndef CHILIB_TREE_HPP
#define CHILIB_TREE_HPP

#include <memory>
#include <utility>
#include <sstream>
#include "chivector.hpp"
#include "queue.hpp"
#include <iostream>
#include <typeinfo>

namespace chilib {
using std::shared_ptr;
using std::weak_ptr;

template<typename T>
class binary_tree {
protected:
  T data{};
  // 层数
  int height = -1;
  // 左右孩子
  shared_ptr<binary_tree> child_left = nullptr, child_right = nullptr;

public:
  explicit binary_tree(T &&val) : data(std::forward<T>(val)) {}

  explicit binary_tree(T &val) : data(val) {}

  binary_tree() = default;

  // 重载输出
  friend std::ostream &operator<<(std::ostream &out, binary_tree &tr) {
    out << "Tree(" << tr.get_data() << ", ";
    if (tr.child_left != nullptr) out << *(tr.child_left) << ", ";
    else out << "nullptr, ";
    if (tr.child_right != nullptr) out << *(tr.child_right);
    else out << "nullptr";
    out << ")";
    return out;
  }

  static shared_ptr<binary_tree> make(T &&d) {
    auto p = new binary_tree(d);
    return std::shared_ptr<binary_tree>(p);
  }

  static auto make(T &d) {
    return make(std::forward<T>(d));
  }

  T &get_data() { return data; }

  auto &get_left() { return child_left; }

  auto &get_right() { return child_right; }

  int &get_height() { return height; }

  bool is_leaf() {
    return this->get_left() == nullptr && this->get_right() == nullptr;
  }
};

enum traversal_type {
  inorder, preother, postoder
};

/*!
   * 按照指定顺序遍历树
   * @tparam F
   * @param method [preorder, inorder, postoder]
   * @param callback bool(btree<T> &)
   * @return
   */
template<typename Tree, typename F>
void traversal(const traversal_type &method, Tree &tree, F const &callback, vector<Tree *> *stack = nullptr) {
  const auto traversal_if = [&method, &callback, &stack](std::shared_ptr<Tree> &child) {
    if (child != nullptr) traversal(method, callback, stack);
  };
  if (stack != nullptr) stack->emplace_back(&tree);
  if (method == preother) {
    if (!callback(tree)) return;
    traversal_if(tree.get_left());
    traversal_if(tree.get_right());
  } else if (method == inorder) {
    traversal_if(tree.get_left());
    if (!callback(tree)) return;
    traversal_if(tree.get_right());
  } else if (method == postoder) {
    traversal_if(tree.get_left());
    traversal_if(tree.get_right());
    if (!callback(tree)) return;
  }
  if (stack != nullptr) stack->pop_back();
}

/*!
 * 广度优先遍历
 * @tparam F1 bool(btree<T>&, bool*)
 * @tparam F2 bool(btree<T>&, int&, bool*)
 * @param tree 目标树
 * @param on_item 每个元素的回调
 * @param on_layer 每一层的回调
 * @param includes_null=false 是否在元素回调中包含空节点
 */
//template<typename F1, typename F2>
//static void
//traversal_bfs(shared_ptr<binary_tree> &tree, F1 const &on_item, F2 const &on_layer,
//              const bool includes_null = false) {
//  if (tree == nullptr) return;
//  vector<shared_ptr<binary_tree> *> q;
//  int height = 0;
//  tree->height = height;
//  shared_ptr<binary_tree> *tree_temp = &tree;
//  q.emplace_back(tree_temp);
//  bool new_layer = false;
//  bool end_all = false;
//  while (!q.empty()) {
//    shared_ptr<binary_tree> *top = q.front();
//    if (!on_item(*top, &end_all)) {
//      if (end_all) return;
//      q.pop_front();
//      continue;
//    }
//    if (end_all) return;
//    if (new_layer) {
//      if (!on_layer(*top, height + 1, &end_all)) {
//        if (end_all) return;
//        continue;
//      }
//      new_layer = false;
//      if (end_all) return;
//    }
//    if (includes_null && *top == nullptr) {
//      q.pop_front();
//      continue;
//    }
//    shared_ptr<binary_tree> &left = (*top)->get_left(), &right = (*top)->get_right();
//    if ((*top)->height > height) height = (*top)->height;
//    auto push_if = [&q, &includes_null, &height](shared_ptr<binary_tree> &child) {
//      if (child != nullptr || includes_null) {
//        if (!includes_null) child->height = height + 1;
//        q.emplace_back(&child);
//      }
//    };
//    push_if(left);
//    push_if(right);
//    q.pop_front();
//    if (!q.empty()) {
//      // 即将到下一层
//      if (!includes_null && (*q[0])->height > height) {
//        new_layer = true;
//      }
//    }
//  }
//}

/*!
 * 深度优先遍历（准备栈）
 * @tparam F bool(btree<T>&, vector<btree<T>*>&)
 * @param callback 回调函数
 */
//template<typename F, typename Tree>
//void traversal_dfs(const traversal_type method, Tree &tree, F const &callback) {
//  vector<Tree *> stack;
//  // 利用闭包特性传递栈信息
//  auto wrapper = [&stack, &callback](Tree &tr) -> bool {
//    return callback(tr, stack);
//  };
//  traversal(method, tree, wrapper, &stack);
//}

class search_tree : public binary_tree<int> {
public:
  using T = int;
  using Cmp = less<T>;
  Cmp cmp;
  bool is_empty = true;

  void insert(T &&d) {
    if (is_empty) {
      data = d;
      is_empty = false;
      return;
    }
    if (cmp(data, d)) {
      if (get_left() == nullptr) {
        get_left() = make(d);
      } else {

      }
    } else {
      get_right() = make(d);
    }
  }

  void insert(T &d) { insert(std::forward<T>(d)); }
};

class avl_tree {
public:
  using T = int;
  vector<T> data;

  avl_tree() = default;


};


}

#endif //CHILIB_TREE_HPP


/*
 * void chilib::traversal(const chilib::traversal_type&, Tree&, const F&, chilib::vector<Tree*>*)
 *  [
 *    with Tree = const chilib::traversal_dfs(chilib::traversal_type, Tree&, const F&) [
 *      with Tree = chilib::binary_tree<int>;
 *      F = main()::<lambda(bt&, chilib::vector<chilib::binary_tree<int>*>&)>
 *    ]::<lambda(chilib::binary_tree<int>&)>;
 *      F = chilib::vector<chilib::binary_tree<int>*>*
 *  ]
 *
 * */