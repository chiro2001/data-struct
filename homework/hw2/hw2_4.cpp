#include <stdio.h>

using T = int;
const size_t TREE_MAX_NODE_NUM = 40;
class Tree {
 public:
  T data{};
  Tree *left = nullptr, *right = nullptr;
  Tree(T d) : data(d) {}
  // 利用析构函数自动释放内存
  ~Tree() {
    printf("\t~deleting %d\n", this->data);
    if (this->left) delete this->left;
    if (this->right) delete this->right;
  }
};

class Queue {
 public:
  Tree *data = nullptr;
  Queue *next = nullptr;
  Queue(Tree *p) : data(p) {}
  Queue() {}
  void link(Queue *q) { this->next = q; }
};

void BFS(Tree *t, T val) {
  if (t->data == val) {
    delete t;
    return;
  }
  Queue *front = new Queue(t), *temp = nullptr;
  Queue *top = new Queue(nullptr);
  front->link(top);
  Tree *f = nullptr;
  while (front != top) {
    // pop
    f = front->data;
    printf("visit: %d\n", f->data);
    temp = front;
    front = front->next;
    delete temp;
    if (f->left != nullptr) {
      if (f->left->data == val) {
        delete f->left;
      } else {
        top->data = f->left;
        top->next = new Queue(nullptr);
        top = top->next;
      }
    }
    if (f->right != nullptr) {
      if (f->right->data == val) {
        delete f->right;
      } else {
        top->data = f->right;
        top->next = new Queue(nullptr);
        top = top->next;
      }
    }
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
  right->left = new Tree(4);
  left = left->left;
  left->left = new Tree(7);
  left->right = new Tree(9);
  BFS(root, 4);
  return 0;
}