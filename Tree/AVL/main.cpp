#include <cstdio>
#include "chilib/tree.hpp"

using namespace chilib;
using bt = binary_tree<int>;

int main() {
  vector<int> line{9, 8, 7, 6, -1, 5, -1, 4, -1, -1, -1, -1, 3, -1};
  // 用宽度优先遍历建树
  auto p = line.begin();
  auto root = std::make_shared<bt>(*p);
//  ++p;
//  bt::traversal_bfs(root, [&p, &line](std::shared_ptr<bt> &tr, bool *end_all) -> bool {
//    // 在 includes_null 情况下，需要设置 end_all 结束遍历
//    if (tr != nullptr) return true;
//    tr = std::make_shared<bt>(*p);
//    ++p;
//    if (p == line.end()) {
//      *end_all = true;
//      return false;
//    }
//    return true;
//  }, [](std::shared_ptr<bt> &tr, int height, bool *end_all) -> bool {
//    return true;
//  }, true);
//  root->traversal(bt::preother, [](bt &tr) -> bool {
//    if (tr.get_data() == -1) {
//      tr.get_left() = nullptr;
//      tr.get_right() = nullptr;
//    }
//    return true;
//  });
//  root->traversal(bt::preother, [](bt &tr) -> bool {
//    if (tr.get_left() != nullptr && tr.get_left()->get_data() == -1) {
//      tr.get_left() = nullptr;
//    }
//    if (tr.get_right() != nullptr && tr.get_right()->get_data() == -1) {
//      tr.get_right() = nullptr;
//    }
//    return true;
//  });
//  auto print_it = [](bt &tree) -> bool {
//    printf("visit: %d\n", tree.get_data());
//    return true;
//  };
//  root->traversal(bt::postoder, print_it);
//  puts("#####################");
//  bt r;
//  traversal_dfs(preother, r, [](bt &tr, vector<bt *> &stack) {
//    printf("visit: %d\n", tr.get_data());
//    printf("  stack: ");
//    for (auto p : stack) {
//      printf("%d ", p->get_data());
//    }
//    puts("");
//    return true;
//  });
  traversal(preother, *root, [](bt &tree) -> bool {
    printf("visit: %d\n", tree.get_data());
    return true;
  });
  return 0;
}