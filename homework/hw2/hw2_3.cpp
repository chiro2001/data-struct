#include <stdio.h>

using T = int;
const size_t TREE_MAX_NODE_NUM = 40;
class Tree {
 public:
  T data{};
  Tree *left = nullptr, *right = nullptr;
  Tree(T d) : data(d) {}
};

class Queue {
 public:
  Tree *data = nullptr;
  Queue *next = nullptr;
  int depth = -1;
  Queue(Tree *p) : data(p) {}
  Queue(int d) : depth(d) {}
  Queue() {}
  void link(Queue *q) { this->next = q; }
};

int BFS(Tree *t) {
  int depthMax = 0;
  int widthMax = 0;
  Queue *front = new Queue(t), *temp = nullptr;
  front->depth = 0;
  Queue *top = new Queue(1);
  front->link(top);
  Tree *f = nullptr;
  while (front != top) {
    // pop
    f = front->data;
    int depthNow = front->depth;
    if (depthNow > depthMax) depthMax = depthNow;
    // printf("visit: %d | %d\n", depthNow, f->data);
    temp = front;
    front = front->next;
    delete temp;
    if (f->left != nullptr) {
      top->data = f->left;
      top->depth = depthNow + 1;
      top->next = new Queue(nullptr);
      top = top->next;
    }
    if (f->right != nullptr) {
      top->data = f->right;
      top->depth = depthNow + 1;
      top->next = new Queue(nullptr);
      top = top->next;
    }
    // 即将轮到下一层了
    if (front->next != nullptr) {
      if (front->next->depth > depthNow) {
        Queue *t = front;
        // 实际上是下一层的宽度
        int widthNow = 0;
        while (t != top) {
          t = t->next;
          widthNow++;
        }
        // printf("widthNow = %d\n", widthNow);
        if (widthNow > widthMax) widthMax = widthNow;
      }
    }
  }
  return widthMax * depthMax;
}

int main() {
  // 建立一棵树
  Tree *root = new Tree(0);
  root->left = new Tree(1);
  root->right = new Tree(2);
  Tree *left = root->left, *right = root->right;
  left->left = new Tree(4);
  left->right = new Tree(6);
  right->left = new Tree(10);
  left = left->left;
  left->left = new Tree(7);
  left->right = new Tree(9);
  printf("BFS(root) = %d\n", BFS(root));
  return 0;
}