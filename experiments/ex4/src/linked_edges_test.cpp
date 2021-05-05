#include <cstdio>
#include "chilib/linked_edges.hpp"

int main() {
  // 最大节点数
  const int EDGES_MAX = 3;
  chilib::linked_edges edges(EDGES_MAX);
  // 添加边
  edges.add_edge(1, 2, 1);
  edges.add_edge(2, 1, 1);
  // 遍历边
  for (int from = 1; from < EDGES_MAX; from++)
    for (const auto &edge: edges.set_from(from))
      printf("%d ==%d==> %d\n", from, edge.weight, edge.to);
  return 0;
}