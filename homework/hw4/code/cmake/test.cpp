#include "chilib/queue.hpp"

int main() {
  chilib::priority_queue<int> q;
  chilib::vector<int> v{5, 4, 3, 2, 1};
  for (const auto &i : v) {
    q.push(i);
  }
}