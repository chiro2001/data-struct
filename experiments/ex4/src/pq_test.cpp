//
// Created by Chiro on 2021/5/1.
//

#include <cstdio>
#include <cstdlib>
#include "chilib/queue.hpp"
#include <queue>

int main() {
  chilib::priority_queue<int, chilib::less<int>> q;
  std::priority_queue<int, std::vector<int>, std::less<> > qs;
  for (int i = 1000; i >= 0; i--) {
    int t = rand() % 1000;
    q.push(t);
    qs.push(t);
//    printf("pushed: %d\n", i);
  }
  while (!qs.empty()) {
//    printf("q.empty(): %s\n", q.empty() ? "true" : "false");
    printf("%d %d", q.top(), qs.top());
    if (q.top() != qs.top()) {
      printf("\t%d != %d\n", q.top(), qs.top());
//      break;
    } else puts("");
    q.pop();
    qs.pop();
  }
//  chilib::vector<int> v;
//  for (int i = 1; i >= 0; i--) {
//    v.emplace_back(i);
//    printf("pushed: %d\n", i);
//  }
//  while (!v.empty()) {
//    try {
//      printf("%d len(%d)\n", v[v.size() - 1], v.size());
//      v.pop_back();
//    } catch (std::out_of_range &e) {
//      fprintf(stderr, "len(%d) ERR: %s\n", v.size(), e.what());
//    }
//  }
  return 0;
}