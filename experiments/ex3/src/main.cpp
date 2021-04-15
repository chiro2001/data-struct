#include <cstdio>
#include <cstdlib>
#include <memory>
#include "chistring.hpp"
#include "chivector.hpp"
#include "btree.hpp"

using bt = chilib::btree<int>;

int main_() {
#if 0
  int n;
  chilib::vector<char> line;
  scanf("%d", &n); // NOLINT(cert-err34-c)
  while (true) {
    char c = (char) getchar();
    if (c == ' ') continue;
    line.emplace_back(c);
    if (!(--n)) break;
  }
  for (char i : line) putchar(i);
  puts("");
#else
//  chilib::vector<char> line{'9', '8', '7', '6', '#', '5', '#', '4', '#', '#', '#', '#', '3'};
//  chilib::vector<char> line_{'1', '2', '3', '4', '5', '6', '7', '8', '9', '#', '#', '#', '0', '#', '#'};
  chilib::vector<int> line{1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, 10, -1, -1};
#endif
  auto p = line.begin();
  auto root = std::make_shared<bt>(*p);
  ++p;
  bt::
  traversal_bfs(root, [&p, &line](std::shared_ptr<bt> &tr, bool &end_all) -> bool {
    if (tr != nullptr) return true;
//    printf("will parse: %d\n", *p);
    bool result = false;
    if (*p != -1) {
      tr = std::make_shared<bt>(*p);
      result = true;
    }
    ++p;
    if (p == line.end()) {
      end_all = true;
//      puts(" e n d _ a l l ! ! ! !");
      return false;
    }
    return result;
  }, [&p](std::shared_ptr<bt> &tr, bool &end_all) -> bool {
    return true;
  }, true);
#if 0
  puts("o u t p u t i n g:");
  puts("\n\n");
  bt::
  traversal_bfs(root,
                [](std::shared_ptr<bt> &tr, bool &) -> bool {
                  if (tr == nullptr) return false;
                  printf("visit: %d\n", tr->get_data());
                  return true;
                },
                [](std::shared_ptr<bt> &tr, bool &end_all) -> bool {
                  return true;
                }, false);
  auto Tree = [](int, void *, void *) -> void * { return nullptr; };
  Tree(1,
       Tree(2,
            Tree(4,
                 Tree(8, nullptr, nullptr),
                 Tree(9, nullptr, nullptr)
            ),
            Tree(5, nullptr, nullptr)
       ),
       Tree(3,
            Tree(6,
                 nullptr,
                 Tree(0, nullptr, nullptr)
            ),
            Tree(7,
                 nullptr,
                 nullptr
            )
       )
  );
#endif
  auto print_orderd = [](bt &tr) -> bool {
    printf("%d ", tr.get_data());
    return true;
  };
  root->traversal_preorder(print_orderd);
  puts("");
  root->traversal_inorder(print_orderd);
  puts("");
  root->traversal_postorder(print_orderd);
  puts("");
  int sum_max = 0;
  root->traversal_dfs([&sum_max](bt &tr, chilib::vector<bt> &stack) -> bool {
    if (tr.is_leaf()) {
      int sum = 0;
      for (auto &t : stack) {
        sum += t.get_data();
//        printf("%d ", t.get_data());
      }
//      printf("= %d\n", sum);
      sum_max = sum_max > sum ? sum_max : sum;
    }
    return true;
  });
  printf("sum_max: %d\n", sum_max);
  int sum_left = 0;
  root->traversal_preorder([&sum_left](bt &tr) -> bool {
    if (tr.is_leaf()) return true;
    // 仅仅统计左子叶
    if (tr.get_left() != nullptr && tr.get_left()->is_leaf()) sum_left += tr.get_left()->get_data();
    return true;
  });
  printf("sum_left = %d\n", sum_left);
  // 交换，用前序遍历
  root->traversal_preorder([](bt &tr) -> bool {
    auto tmp_left = tr.get_left(), tmp_right = tr.get_right();
    // 防止出现环
    tr.get_left() = nullptr;
    tr.get_right() = nullptr;
    tr.get_left() = tmp_right;
    tr.get_right() = tmp_left;
    return true;
  });
  root->traversal_inorder(print_orderd);
  puts("");
  return 0;
}

int main() {
  const int SIZE = 128;
  const int MAX_NUM = 10;
  char buff[SIZE];
  char num[MAX_NUM];
  const bool use_graphviz = false;
  /**
   * ===============================================================
   * ||                   End Configuration                       ||
   * ===============================================================
   */
  FILE *fp = fopen("./test.txt", "r");
  if (!fp) {
    perror("打开文件时发生错误");
    return -1;
  } else {
    int i = 0;
    /**
     * ===============================================================
     * ||                   Read data here                          ||
     * ===============================================================
     */
    while (fgets(num, MAX_NUM, fp) && fgets(buff, SIZE, fp)) {
      char *nextline = strchr(buff, '\n');          //查找换行符
      if (nextline)                            //如果find不为空指针
        *nextline = '\0';                    //就把一个空字符放在这里
      printf("Case %d, data: %s, nodes number: %s", i, buff, num);
      int size = atoi(num); // NOLINT(cert-err34-c)
      chilib::vector<int> line;
      char *cnt_char = buff;
      char buf2[MAX_NUM];
      while (*cnt_char) {
        if (*cnt_char == ' ') {
          cnt_char++;
          continue;
        }
        char *p = cnt_char, *p2 = buf2;
        while (*p != ' ' && *p > 0) *(p2++) = *(p++);
        *p2 = '\0';
        int now = 0;
        if (buf2[0] == '#') now = -1;
        else now = atoi(buf2); // NOLINT(cert-err34-c)
        line.emplace_back(now);
        cnt_char++;
      }
      for (const auto &p : line) {
//        printf("%d ", p);
      }
      // 建树
      auto p = line.begin();
      auto root = std::make_shared<bt>(*p);
      bool tree_empty = *p == -1;
      if (!tree_empty) {
        ++p;
        bt::traversal_bfs(root, [&p, &line](std::shared_ptr<bt> &tr, bool &end_all) -> bool {
          if (tr != nullptr) return true;
//          printf("will parse: %d\n", *p);
          bool result = false;
          if (*p != -1) {
            tr = std::make_shared<bt>(*p);
            result = true;
          }
          ++p;
          if (p == line.end()) {
            end_all = true;
//            puts(" e n d _ a l l ! ! ! !");
            return false;
          }
          return result;
        }, [&p](std::shared_ptr<bt> &tr, bool &end_all) -> bool {
          return true;
        }, true);
      }
      /** 任务一 */
      auto print_orderd = [](bt &tr) -> bool {
        printf("%d ", tr.get_data());
        return true;
      };
      printf("Answer for task 1 is: \n");
      printf("preOrderTraverse is:");
      if (!tree_empty)
        root->traversal_preorder(print_orderd);
      printf("\ninOrderTraverse is:");
      if (!tree_empty)
        root->traversal_inorder(print_orderd);
      printf("\npostOrderTraverse is:");
      if (!tree_empty)
        root->traversal_postorder(print_orderd);
      puts("");
      /** 通过 graphviz 可视化，勿删，助教测试使用 */
      if (use_graphviz) {
//        plot(tree_root, i, size, "tree");
      }
      /** 任务二 */
      int max_path_sum = 0;
      if (!tree_empty)
        root->traversal_dfs([&max_path_sum](bt &tr, chilib::vector<bt> &stack) -> bool {
          if (tr.is_leaf()) {
            int sum = 0;
            for (auto &t : stack) {
              sum += t.get_data();
//        printf("%d ", t.get_data());
            }
//      printf("= %d\n", sum);
            max_path_sum = max_path_sum > sum ? max_path_sum : sum;
          }
          return true;
        });
      printf("Answer for task 2 is : %d \n", max_path_sum);
      /** 任务三 */
      int weight_sum = 0;
      if (!tree_empty)
        root->traversal_preorder([&weight_sum](bt &tr) -> bool {
          if (tr.is_leaf()) return true;
          // 仅仅统计左子叶
          if (tr.get_left() != nullptr && tr.get_left()->is_leaf()) weight_sum += tr.get_left()->get_data();
          return true;
        });
      printf("Answer for task 3 is : %d \n", weight_sum);
      /** 任务四 */
      // 交换，用前序遍历
      if (!tree_empty)
        root->traversal_preorder([](bt &tr) -> bool {
          auto tmp_left = tr.get_left(), tmp_right = tr.get_right();
          // 防止出现环
          tr.get_left() = nullptr;
          tr.get_right() = nullptr;
          tr.get_left() = tmp_right;
          tr.get_right() = tmp_left;
          return true;
        });
      printf("inOrderTraverse for task 4 is:");
      if (!tree_empty)
        root->traversal_inorder(print_orderd);
      printf("\n\n");
      /** 通过 graphviz 可视化，勿删，助教测试使用 */
      if (use_graphviz) {
//        plot(invert_tree_root, i, size, "invert_tree");
      }
      // 释放内存（其实是自动的）
      root = nullptr;
      i++;
    }
  }
  return 0;
}