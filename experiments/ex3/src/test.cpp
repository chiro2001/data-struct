#include <cstdio>
#include <cstdlib>
#include <memory>
#include "chistring.hpp"
#include "chivector.hpp"
#include "btree.hpp"

using bt = chilib::btree<int>;

int main() {
  chilib::vector<int> line{0, 1, 2, 3, 4, 5, 6, 7, 8};
  auto p = line.begin();
  auto root = std::make_shared<bt>(*p);
  bt::traversal_bfs(root, [&p, &line](std::shared_ptr<bt> &tr, bool *end_all) -> bool {
    if (tr != nullptr) return true;
    tr = std::make_shared<bt>(*p);
    ++p;
    if (p == line.end()) {
      *end_all = true;
      return false;
    }
    return true;
  }, [](std::shared_ptr<bt> &, int, bool *) -> bool { return true; }, true);
  root->traversal(bt::inorder, [](bt &tr) -> bool {
    printf("visit: %d\n", tr.get_data());
    return true;
  });
  return 0;
}
