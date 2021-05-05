#include <stdio.h>
#include <string.h>

typedef unsigned char u8;

// 邻接表结构
typedef struct {
  int edges[32][32];
  int tails[32];
} LinkedEdge;

// 加边操作
void add_edge_directed(LinkedEdge *e, int from, int to) {
  e->edges[from][e->tails[from]++] = to;
}

// 加双向边操作
void add_edges(LinkedEdge *e, int from, int to) {
  add_edge_directed(e, from, to);
  add_edge_directed(e, to, from);
}

// 递归式DFS遍历
int dfs(LinkedEdge *e, int u, int v, u8 *visit, int *path, int path_tail) {
  printf("visit: %d\n", u);
  if (u == v) return path_tail;
  for (int i = 0; i < e->tails[u]; i++) {
    int to = e->edges[u][i];
    u8 *vis = visit + (to / 8 + to % 8);
    u8 offset = 1 << (to % 8);
    if (*vis & offset) continue;
    path[path_tail++] = to;
    *vis |= offset;
    int tail = dfs(e, e->edges[u][i], v, visit, path, path_tail);
    if (tail) return tail;
    *vis &= ~offset;
    path_tail--;
  }
  return 0;
}

// 非递归式DFS遍历
int dfs_loop(LinkedEdge *e, int u, int v, u8 *visit, int *path) {
  int tail = 0;
  // 用栈的方式遍历，添加起点
  path[tail++] = u;
  // 记录起点已经访问
  visit[u / 8 + u % 8] |= 1 << (u % 8);
  while (tail) {
    // 取栈顶
    int from = path[tail - 1];
    printf("visit: %d\n", from);
    u8 will_pop = 1;
    // 到达目的地
    if (from == v) break;
    for (int i = 0; i < e->tails[from]; i++) {
      int to = e->edges[from][i];
      // 需要操作的对应字节
      u8 *vis = visit + (to / 8 + to % 8);
      // 需要操作的对应字节
      u8 offset = 1 << (to % 8);
      // 判断第 to 位是否置1
      if (!(*vis & offset)) {
        // visit 的第 to 位置1
        *vis |= offset;
        // 记录路径
        path[tail++] = to;
        will_pop = 0;
        // 记录一次就跳出，这样才叫DFS
        break;
      }
    }
    // 只有在找不到继续向下的路径的时候才会弹出（抛弃）这个节点
    if (will_pop) tail--;
  }
  // 返回路径长度，找不到则返回 0
  return tail;
}

int main() {
  LinkedEdge e;
  // 初始化为0就好
  memset((void *)&e, 0, sizeof(LinkedEdge));
  // 建立图，图同 2.3
  add_edges(&e, 0, 1);
  add_edges(&e, 0, 2);
  add_edges(&e, 1, 2);
  add_edges(&e, 1, 3);
  add_edges(&e, 3, 7);
  add_edges(&e, 1, 4);
  add_edges(&e, 2, 4);
  add_edges(&e, 2, 5);
  add_edges(&e, 5, 6);
  add_edges(&e, 4, 6);
  add_edges(&e, 6, 7);
  // 从 0 到 7
  int u = 0, v = 7;
  int path[512];
  u8 visit[40] = {0};
  // int path_tail = 0;
  // path[path_tail++] = u;
  // visit[u / 8 + u % 8] |= 1 << (u % 8);
  // path_tail = dfs(&e, u, v, visit, path, path_tail);
  if (u == v) {
    puts("起点终点重合");
    return 0;
  }
  int path_tail = dfs_loop(&e, u, v, visit, path);
  if (!path_tail) {
    // 打印路径
    printf("path: ");
    for (int i = 0; i < path_tail; i++) printf("%d ", path[i]);
    puts("");
  } else {
    puts("找不到路径!");
  }
  return 0;
}