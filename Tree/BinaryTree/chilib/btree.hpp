//
// Created by Chiro on 2021/4/6.
//

#ifndef CHILIB_BTREE_HPP
#define CHILIB_BTREE_HPP

#include <memory>

using std::shared_ptr;

namespace chilib {

class btree {
public:
  using T = char;

  explicit btree(T val) : data(val) {}

  explicit btree(const T *val) : data(*val) {}

  btree() = default;

  static shared_ptr<btree> make(T &d) {
    auto p = new btree(d);
    return std::shared_ptr<btree>(p);
  }

  T data{};
  // 是否线索化
  bool is_in_threading = false;
  // 是否有左右孩子，在线索化的时候使用
  bool tag_left = false, tag_right = false;
  shared_ptr<btree> child_left = nullptr, child_right = nullptr;

  T &get_data() { return data; }

  shared_ptr<btree> &get_left() { return child_left; }

  shared_ptr<btree> &get_right() { return child_right; }

  // 前序遍历 + 中序遍历建树

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
};

}

#endif //CHILIB_BTREE_HPP
