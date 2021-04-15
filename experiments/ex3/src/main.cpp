#include <cstdio>
#include <cstdlib>
#include <memory>
#include "chistring.hpp"
#include "chivector.hpp"
#include "btree.hpp"

using bt = chilib::btree<int>;

// 创建 dot 可视化文件，感兴趣的同学可以学习
void createDotFile(const char *filename, std::shared_ptr<bt> &root, int MaxSize) {
  int err = 0;
  FILE *fp = nullptr;
  err = fopen_s(&fp, filename, "w");    // 文件指针
  if (err || !fp) {
    std::cerr << "无法打开文件 " << filename << " !";
    exit(1);
  }
  if (fp == nullptr) {   // 为NULL则返回
    printf("File cannot open!");
    exit(0);
  }
  fprintf(fp, "digraph G {\n");   // 开头
  // 利用层次遍历构造
  if (root->get_data() != -1)
    bt::
    traversal_bfs(root,
                  [&fp](std::shared_ptr<bt> &tr, bool &end_all) -> bool {
                    if (tr->get_data() == -1) return false;
                    fprintf(fp, "N%p [shape=circle, label=\"%d\"];\n", tr.get(), tr->get_data());

                    auto parser = [&fp, &tr](std::shared_ptr<bt> &t) {
                      if (t == nullptr) return;
                      if (t->get_data() != -1) {
                        fprintf(fp, "N%p->N%p;\n", tr.get(), t.get());
                        return;
                      }
                      fprintf(fp, "N%p [shape=circle, label=\"#\"];\n", t.get());
                      fprintf(fp, "N%p->N%p;\n", tr.get(), t.get());
                    };
                    parser(tr->get_left());
                    parser(tr->get_right());
                    return true;
                  },
                  [](std::shared_ptr<bt> &tr, int height, bool &end_all) -> bool { return true; }
    );
  else fprintf(fp, "NULL_NODE [shape=circle, label=\"#\"];\n");
  fprintf(fp, "}\n"); // 结尾
  fclose(fp); // 关闭IO
}

// 绘制二叉树图片,调用 createDotFile 并使用 system 执行命令
void plot(std::shared_ptr<bt> &tree_root, int i, int size, const char *name) {
  char tree_filename[50], paint_tree[100];
  sprintf_s(tree_filename, "./%s_%d.dot", name, i);
  createDotFile(tree_filename, tree_root, size);
  sprintf_s(paint_tree, "dot -Tpng %s -o ./%s_%d.png", tree_filename, name, i);
  // puts(paint_tree);
  system(paint_tree);
}

int main() {
  const int SIZE = 128;
  const int MAX_NUM = 10;
  char buff[SIZE];
  char num[MAX_NUM];
  const bool use_graphviz = true;
  /**
   * ===============================================================
   * ||                   End Configuration                       ||
   * ===============================================================
   */
  int err;
  FILE *fp = nullptr;
  err = fopen_s(&fp, "./test.txt", "r");
  if (!fp || err) {
    std::cerr << "打开文件时发生错误! ";
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
//          if (*p != -1) {
          tr = std::make_shared<bt>(*p);
          result = true;
//          }
          ++p;
          if (p == line.end()) {
            end_all = true;
//            puts(" e n d _ a l l ! ! ! !");
            return false;
          }
          return result;
        }, [](std::shared_ptr<bt> &tr, int height, bool &end_all) -> bool {
          printf("height now: %d\n", height);
          return true;
        }, true);
      }
      /** 任务一 */
      auto print_orderd = [](bt &tr) -> bool {
        if (tr.get_data() != -1)
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
        plot(root, i, size, "tree");
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
          if (tr.get_data() == -1) return true;
          if (tr.is_leaf()) return true;
          // 仅仅统计左子叶
          if (tr.get_left() != nullptr && tr.get_left()->is_leaf() && tr.get_left()->get_data() != -1)
            weight_sum += tr.get_left()->get_data();
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
        plot(root, i, size, "invert_tree");
      }
#if 0
      puts("$$$$$$$$\n\n");
      bt::traversal_bfs(root, [](std::shared_ptr<bt> &tr, bool &end_all) -> bool {
                          if (tr->get_data() == -1) return true;
                          printf("  visit: %d @ %d\n", tr->get_data(), tr->height);
                          return true;
                        },
                        [](std::shared_ptr<bt> &tr, int height, bool &end_all) -> bool {
                          printf("height now: %d\n", height);
                          return true;
                        });
#endif
      // 释放内存（其实是自动的）
      root = nullptr;
      i++;
    }
  }
  return 0;
}