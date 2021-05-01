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
#include <typeinfo>

namespace chilib {
using std::shared_ptr;

template<typename T>
class btree {
private:
  T data{};
  // 是否线索化
  bool is_in_threading = false;
  // 是否有左右孩子，在线索化的时候使用
  bool tag_left = false, tag_right = false;
  // 层数
  int height = -1;
  // 左右孩子
  shared_ptr<btree> child_left = nullptr, child_right = nullptr;

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

  T &get_data() { return data; }

  shared_ptr<btree> &get_left() { return child_left; }

  shared_ptr<btree> &get_right() { return child_right; }

  bool &get_is_threading() { return is_in_threading; }

  bool &get_tag_left() { return tag_left; }

  bool &get_tag_right() { return tag_right; }

  int &get_height() { return height; }

  bool is_leaf() {
    return this->get_left() == nullptr && this->get_right() == nullptr;
  }

  /*!
   *
   */
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
  template<typename F>
  constexpr void traversal(const traversal_type &method, F const &callback, vector <btree<T>> *stack = nullptr) {
    if (stack != nullptr) stack->emplace_back(*this);
    if (method == preother) {
      if (!callback(*this)) return;
      if (child_left != nullptr) child_left->traversal(method, callback);
      if (child_right != nullptr) child_right->traversal(method, callback);
    } else if (method == inorder) {
      if (child_left != nullptr) child_left->traversal(method, callback);
      if (!callback(*this)) return;
      if (child_right != nullptr) child_right->traversal(method, callback);
    } else if (method == postoder) {
      if (child_left != nullptr) child_left->traversal(method, callback);
      if (child_right != nullptr) child_right->traversal(method, callback);
      if (!callback(*this)) return;
    }
    if (stack != nullptr) stack->pop_back();
  }

#if 0
  /*!
   * 中序遍历本树，遍历到 callback 返回 false 为止
   * @tparam F bool(btree<T>&)
   * @param callback
   */
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

  /*!
   * 前序遍历本树，遍历到 callback 返回 false 为止
   * @tparam F bool(btree<T>&)
   * @param callback
   */
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

  /*!
   * 后序遍历本树，遍历到 callback 返回 false 为止
   * @tparam F bool(btree<T>&)
   * @param callback
   */
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
#endif

  /*!
   * 广度优先遍历
   * @tparam F1 bool(btree<T>&, bool*)
   * @tparam F2 bool(btree<T>&, int&, bool*)
   * @param tree 目标树
   * @param on_item 每个元素的回调
   * @param on_layer 每一层的回调
   * @param includes_null=false 是否在元素回调中包含空节点
   */
  template<typename F1, typename F2>
  static void
  traversal_bfs(shared_ptr<btree> &tree, F1 const &on_item, F2 const &on_layer, const bool includes_null = false) {
    if (tree == nullptr) return;
    vector<shared_ptr<btree> *> q;
    int height = 0;
    tree->height = height;
    shared_ptr<btree> *tree_temp = &tree;
    q.emplace_back(tree_temp);
    bool new_layer = false;
    bool end_all = false;
    while (!q.empty()) {
      shared_ptr<btree> *top = q.front();
      if (!on_item(*top, &end_all)) {
        if (end_all) return;
        q.pop_front();
        continue;
      }
      if (end_all) return;
      if (new_layer) {
        if (!on_layer(*top, height + 1, &end_all)) {
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
      shared_ptr<btree> &left = (*top)->get_left(), &right = (*top)->get_right();
      if ((*top)->height > height) height = (*top)->height;
      auto push_if = [&q, &includes_null, &height](shared_ptr<btree> &child) {
        if (child != nullptr || includes_null) {
          shared_ptr<btree> *tmp = &child;
          if (!includes_null) (*tmp)->height = height + 1;
          q.emplace_back(tmp);
        }
      };
      push_if(left);
      push_if(right);
      q.pop_front();
      if (!q.empty()) {
        // 即将到下一层
        if (!includes_null && (*q[0])->height > height) {
          new_layer = true;
        }
      }
    }
  }

  /*!
   * 深度优先遍历（准备栈）
   * @tparam F bool(btree<T>&, vector<btree<T>>&)
   * @param callback 回调函数
   */
  template<typename F>
  constexpr void traversal_dfs(const traversal_type method, F const &callback) {
    vector <btree<T>> stack;
    // 利用闭包特性传递栈信息
    auto wrapper = [&stack, &callback](btree<T> &tr) -> bool {
      return callback(tr, stack);
    };
    traversal(method, wrapper, &stack);
  }

#if 0
  /*!
   * 深度优先遍历（含栈）
   * @tparam F bool(btree<T>&, vector<btree<T>>&)
   * @param callback 回调函数
   * @param stack 调用栈
   */
  template<typename F>
  void traversal_dfs_inside(const traversal_type method, F const &callback, vector <btree<T>> &stack) {
    stack.emplace_back(*this);
    if (method == preother) {
      if (!callback(*this, stack)) return;
      if (this->get_left() != nullptr) this->get_left()->traversal_dfs_inside(method, callback, stack);
      if (this->get_right() != nullptr) this->get_right()->traversal_dfs_inside(method, callback, stack);
    } else if (method == inorder) {
      if (this->get_left() != nullptr) this->get_left()->traversal_dfs_inside(method, callback, stack);
      if (!callback(*this, stack)) return;
      if (this->get_right() != nullptr) this->get_right()->traversal_dfs_inside(method, callback, stack);
    } else if (method == postoder) {
      if (this->get_left() != nullptr) this->get_left()->traversal_dfs_inside(method, callback, stack);
      if (this->get_right() != nullptr) this->get_right()->traversal_dfs_inside(method, callback, stack);
      if (!callback(*this, stack)) return;
    }
    stack.pop_back();
  }
#endif
};

}

#endif //CHILIB_BTREE_HPP
