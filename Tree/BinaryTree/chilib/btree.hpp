//
// Created by Chiro on 2021/4/6.
//

#ifndef CHILIB_BTREE_HPP
#define CHILIB_BTREE_HPP

#include <memory>

using std::shared_ptr;

namespace chilib {

typedef int T;

class btree {
public:
  explicit btree(T val) : data(val) {}

  T data{};
  // 是否线索化
  bool is_in_threading = false;
  // 是否有左右孩子，在线索化的时候使用
  bool tag_left = false, tag_right = false;
  shared_ptr<btree> child_left = nullptr, child_right = nullptr;

  // 前序遍历 + 中序遍历建树

  // 中序遍历本树，遍历到 callback 返回 false 为止
  void traversal_inorder(bool (&callback)(btree &)) {
    if (!is_in_threading) {
      if (child_left != nullptr)
        child_left->traversal_inorder(callback);
      if (!callback(*this)) return;
      if (child_right != nullptr)
        child_right->traversal_inorder(callback);
    }
  }
};

}

#endif //CHILIB_BTREE_HPP
