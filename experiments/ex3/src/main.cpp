#include <cstdio>
#include <cstdlib>
#include <memory>
#include "chilib/chistring.hpp"
#include "chilib/chivector.hpp"
#include "chilib/btree.hpp"

using bt = chilib::btree<int>;

// 创建 dot 可视化文件，感兴趣的同学可以学习
void createDotFile(const char *filename, std::shared_ptr<bt> &root, int MaxSize) {
  int err = 0;
  FILE *fp = nullptr;
  err = fopen_s(&fp, filename, "w");    // 文件指针
  if (fp == nullptr || err) {   // 为NULL或者遇到错误则返回
    printf("File cannot open!");
    exit(0);
  }
  fprintf(fp, "digraph G {\n");   // 开头
  // 利用层次遍历构造
  if (root->get_data() != -1)
    bt::traversal_bfs
            (root,
             [&fp](std::shared_ptr<bt> &tr, bool *) -> bool {
               // includes_null 默认为否，此时返回 false 会终止 bfs 向下继续遍历
               if (tr->get_data() == -1) return false;
               fprintf(fp, "N%p [shape=circle, label=\"%d\"];\n", tr.get(), tr->get_data());
               auto parser = [&fp, &tr](std::shared_ptr<bt> &t) {
                 if (t == nullptr) return;
                 if (t->get_data() != -1) {
                     fprintf(fp, "N%p->N%p;\n", tr.get(), t.get());
                   return;
                 }
                 fprintf(fp, "N%p [style=invis, shape=circle, label=\"#\"];\n", t.get());
                 fprintf(fp, "N%p->N%p [style=invis, weight=10];\n", tr.get(), t.get());
               };
               parser(tr->get_left());
               parser(tr->get_right());
               return true;
             },
             [](std::shared_ptr<bt> &tr, int height, bool *end_all) -> bool { return true; }
            );
    // 单独处理空节点
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
  // 改用比较安全的 fopen_s
  err = fopen_s(&fp, "./test.txt", "r");
  if (!fp || err) {
    // perror 在 msvc 环境下会报错，好像是宏定义有问题，暂且未知原因
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
      int size = 0;
      sscanf_s(num, "%d", &size);
      chilib::vector<int> line;
      char *cnt_char = buff;
      // 找到的当前的一个数字
      char buf2[MAX_NUM];
      while (*cnt_char) {
        if (*cnt_char == ' ') {
          cnt_char++;
          continue;
        }
        char *p = cnt_char, *p2 = buf2;
        // 找到的数字填入 buf2
        while (*p != ' ' && *p > 0) *(p2++) = *(p++);
        // 封口
        *p2 = '\0';
        int now = 0;
        if (buf2[0] == '#') now = -1;
        else sscanf_s(buf2, "%d", &now);
        // 添加到节点列表
        line.emplace_back(now);
        cnt_char++;
      }
      // 用宽度优先遍历建树
      auto p = line.begin();
      auto root = std::make_shared<bt>(*p);
      // 单独判断是否为空树
      bool tree_empty = *p == -1;
      if (!tree_empty) {
        ++p;
        bt::traversal_bfs(root, [&p, &line](std::shared_ptr<bt> &tr, bool *end_all) -> bool {
          // 在 includes_null 情况下，需要设置 end_all 结束遍历
          if (tr != nullptr) return true;
          tr = std::make_shared<bt>(*p);
          ++p;
          if (p == line.end()) {
            *end_all = true;
            return false;
          }
          return true;
        }, [](std::shared_ptr<bt> &tr, int height, bool *end_all) -> bool {
          // 这个函数是当遍历完一层之后触发的，height 表示下一层的高度
          // printf("height now: %d\n", height);
          return true;
          // includes_null 表示在遇到空节点的时候是否继续运行，设置为 true 则回调函数可以得到空节点的指针
        }, true);
        // 删除多余的节点
        root->traversal(bt::preother, [](bt &tr) -> bool {
          if (tr.get_data() == -1) {
            // if (tr.get_left() != nullptr) printf("\tdeleting %p\n", tr.get_left().get());
            tr.get_left() = nullptr;
            // if (tr.get_right() != nullptr) printf("\tdeleting %p\n", tr.get_right().get());
            tr.get_right() = nullptr;
          }
          return true;
        });
      }
      /** 任务一 */
      // 定义一个用来打印的回调函数，能够被各种复用
      auto print_order = [](bt &tr) -> bool {
        if (tr.get_data() != -1)
          printf("%d ", tr.get_data());
        return true;
      };
      printf("Answer for task 1 is: \n");
      printf("preOrderTraverse is:");
      if (!tree_empty)
        // 按照传入一个函数的方式进行调用，解耦出遍历过程和处理过程
        root->traversal(bt::preother, print_order);
      printf("\ninOrderTraverse is:");
      if (!tree_empty)
        root->traversal(bt::inorder, print_order);
      printf("\npostOrderTraverse is:");
      if (!tree_empty)
        root->traversal(bt::postoder, print_order);
      puts("");
      /** 通过 graphviz 可视化，勿删，助教测试使用 */
      if (use_graphviz) {
        plot(root, i, size, "tree");
      }
      /** 任务二 */
      int max_path_sum = 0;
      if (!tree_empty)
        root->traversal_dfs(bt::preother, [&max_path_sum](bt &tr, chilib::vector<bt> &stack) -> bool {
          // 遇到叶子节点就向上计算经过的栈
          if (tr.is_leaf()) {
            int sum = 0;
            for (auto &t : stack) sum += t.get_data();
            max_path_sum = max_path_sum > sum ? max_path_sum : sum;
          }
          return true;
        });
      printf("Answer for task 2 is : %d \n", max_path_sum);
      /** 任务三 */
      int weight_sum = 0;
      if (!tree_empty)
        root->traversal(bt::preother, [&weight_sum](bt &tr) -> bool {
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
        root->traversal(bt::preother, [](bt &tr) -> bool {
          auto tmp_left = tr.get_left(), tmp_right = tr.get_right();
          // 防止智能指针出现环
          tr.get_left() = nullptr;
          tr.get_right() = nullptr;
          tr.get_left() = tmp_right;
          tr.get_right() = tmp_left;
          return true;
        });
      printf("inOrderTraverse for task 4 is:");
      if (!tree_empty)
        root->traversal(bt::inorder, print_order);
      printf("\n\n");
      /** 通过 graphviz 可视化，勿删，助教测试使用 */
      if (use_graphviz) {
        plot(root, i, size, "invert_tree");
      }
      // 释放内存（其实是自动的）
      root = nullptr;
      i++;
    }
  }
  return 0;
}
