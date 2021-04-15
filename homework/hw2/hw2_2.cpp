#include <stdio.h>

using T = int;
const size_t TREE_MAX_NODE_NUM = 10;
class Tree {
 public:
  T data{};
  Tree *left = nullptr, *right = nullptr;
  Tree(T d) : data(d) {}
};

// 对树做前序遍历，每个节点都执行一遍某动作
template <typename F>
bool treeAplly(Tree *t, Tree **stack, Tree **&top, F const &f) {
  if (!t) return true;
  if (stack != nullptr) *(top++) = t;
  if (!f(t)) return false;
  if (!(treeAplly(t->left, stack, top, f) &&
        treeAplly(t->right, stack, top, f)))
    return false;
  if (stack != nullptr) top--;
  return true;
}

void findAllFathers(Tree *t, T val) {
  // 寻找到的目标节点
  Tree *node = nullptr;
  // 访问栈
  Tree **stack = new Tree *[TREE_MAX_NODE_NUM];
  Tree **top = stack;
  treeAplly(t, stack, top, [&val, &node](Tree *tr) -> bool {
    // 访问到目标节点就停止，栈保留到从根节点到目标节点的路径
    // printf("visit: %d\n", tr->data);
    if (tr->data == val) {
      node = tr;
      return false;
    }
    return true;
  });
  // 找不到
  if (node == nullptr) return;
  Tree **p = stack;
  // 一路向下打印父亲节点
  while (p != top - 1) {
    printf("%d%s", (*p)->data, (p == top - 2 ? "\n" : " -> "));
    p++;
  }
}

int main() {
  // 建立一棵树
  Tree *root = new Tree(0);
  root->left = new Tree(1);
  root->right = new Tree(2);
  Tree *left = root->left, *right = root->right;
  left->left = new Tree(4);
  left->right = new Tree(6);
  left = left->left;
  left->left = new Tree(7);
  left->right = new Tree(9);
  findAllFathers(root, 9);
  return 0;
}