#include "chilib/queue.hpp"

int main() {
  int branch;
  freopen("5_2_input_5.in", "r", stdin);
  scanf("%d", &branch);
  while (branch--) {
    int k, n;
    scanf("%d%d", &k, &n);
    chilib::priority_queue<int, chilib::less<int>> queue;
    chilib::vector<int> stack;
    for (int i = 0; i < n; i++) {
      int t;
      scanf("%d", &t);
      queue.push(t);
    }
    for (int i = 0; i < k; i++) {
      stack.emplace_front(queue.top());
      queue.pop();
    }
    for (const auto &d : stack) printf("%d ", d);
    puts("");
  }
  return 0;
}