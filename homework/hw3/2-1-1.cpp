#include <cstdio>
#include <iostream>
#include <queue>

/*
一个连通图采用邻接表作为存储结构，设计一个算法，
实现从顶点v出发的深度优先遍历的非递归过程。
*/

// 数据结构：栈
class Stack {
  int *data = nullptr;
  int tail = 0;
  int size_m = 0;

public:
  // 默认构造函数，用于 new 数组
  explicit Stack() = default;

  // 带参数构造函数，用于初始化内存空间
  explicit Stack(int size_) {
    if (size_ == 0) throw std::out_of_range("Stack init error");
    size_m = size_;
    // data = new int[size_m]();
    data = (int *)malloc(sizeof(int) * size_m);
  }

  // 析构函数，释放内存空间
  ~Stack() {
    //delete[] data;
    free(data);
  }

  // 入栈
  void push(int d) {
    if (tail >= size_m) throw std::out_of_range("Stack full");
    data[tail++] = d;
  }

  // 取栈元素
  int &operator[](int pos) {
    if (pos >= tail) throw std::out_of_range("Stack out of range");
    return data[pos];
  }

  // 取栈顶元素
  int &top() {
    if (0 == tail) throw std::out_of_range("Stack empty");
    return data[tail - 1];
  }

  // 出栈
  void pop() { --tail; }

  // 判断是否为空
  bool empty() const { return tail == 0; }

  // 得到栈的长度
  int &size() { return tail; }
};

class AdjacencyTable {
  Stack *data = nullptr;
  int size_m = 0;

public:
  // 构造函数：初始化内存空间
  explicit AdjacencyTable(int size_) {
    if (size_ == 0) throw std::out_of_range("AdjacencyTable init error");
    size_m = size_;
    data = new Stack[size_m * 2]();
    for (int i = 0; i < size_m * 2; i++) data[i] = Stack(size_m);
  }

  // 析构函数：清理内存
  ~AdjacencyTable() { delete[] data; }

  // 获得：最大节点数量
  int &size() { return size_m; }

  // 添加边（单向）
  void add_directed_edge(int from, int to) {
    if (from >= size_m || to >= size_m)
      throw std::out_of_range("AdjacencyTable out of range");
    data[from].push(to);
  }

  // 添加边（双向）
  void add_edges(int from, int to) {
    add_directed_edge(from, to);
    add_directed_edge(to, from);
  }

  // 取某一点出发的边栈
  Stack &operator[](int pos) {
    if (pos >= size_m) throw std::out_of_range("AdjacencyTable out of range");
    return data[pos];
  }
};

void bfs(AdjacencyTable &table, int src) {
  std::queue<int> q;
  int max_node = table.size();
  bool *visited = new bool[max_node];
  for (int i = 0; i < max_node; i++) visited[i] = false;
  visited[src] = true;
  q.push(src);
  while (!q.empty()) {
    int from = q.front();
    q.pop();
    visited[from] = true;
    printf("visited: %d\n", from);
    for (int t = 0; t < table[from].size(); t++) {
      int &to = table[from][t];
      if (!visited[to]) {
        q.push(to);
      }
    }
  }
}

// 深度优先遍历
void dfs(AdjacencyTable &table, int src) {
  int max_node = table.size();
  // 初始化路径栈，并且定义最大路径栈大小
  Stack path(128);
  // 是否已经访问到的表
  bool *visited = new bool[max_node];
  for (int i = 0; i < max_node; i++) visited[i] = false;
  // 向路径中加入初始节点
  path.push(src);
  while (!path.empty()) {
    // 取出栈顶作为当前处理节点
    int from = path.top();
    printf("visit: %d\n", from);
    // 标记节点已经访问
    visited[from] = true;
    // 该节点对应的边
    Stack &edges = table[from];
    bool found_next = false;
    for (int t = 0; t < edges.size(); t++) {
      int &to = edges[t];
      // 访问那些还没访问过的节点
      if (!visited[to]) {
        path.push(to);
        // 标记这些点也访问过了
        visited[to] = true;
        found_next = true;
        break;
      }
    }
    if (!found_next) path.pop();
  }
  // 清理标记的内存
  delete[] visited;
}

void ddd(AdjacencyTable &table, int u, int v, bool *visited, Stack &path) {
  //  printf("visited: %d\n", u);
  //  if (u == v) {
  //    printf("path: ");
  //    for (int i = 0; i < path.size(); i++) printf("%d ", path[i]);
  //    puts("");
  //    return;
  //  }
  for (int t = 0; t < table[u].size(); t++) {
    int &to = table[u][t];
    if (!visited[to]) {
      visited[to] = true;
      path.push(to);
      printf("path: ");
      for (int i = 0; i < path.size(); i++) printf("%d ", path[i]);
      puts("");
      ddd(table, to, v, visited, path);
      printf("path: ");
      for (int i = 0; i < path.size(); i++) printf("%d ", path[i]);
      puts("");
      path.pop();
      visited[to] = false;
    }
  }
}

int main_() {
  // 建立一个最大节点数量为8的邻接表
  AdjacencyTable table(8);
  // 建立图
  table.add_edges(0, 1);
  table.add_edges(0, 2);
  table.add_edges(1, 2);
  table.add_edges(1, 3);
  table.add_edges(3, 7);
  table.add_edges(1, 4);
  table.add_edges(2, 4);
  table.add_edges(2, 5);
  table.add_edges(5, 6);
  table.add_edges(4, 6);
  table.add_edges(6, 7);
  //  bfs(table, 0);
  //  puts("=============================");
  dfs(table, 0);
  return 0;
}

bool dfs_path(AdjacencyTable &table, int u, int v) {
  int max_node = table.size();
  // 初始化路径栈，并且定义最大路径栈大小
  Stack path(512);
  // 是否已经访问到的表
  int *visited = new int[max_node]();
  for (int i = 0; i < max_node; i++) visited[i] = false;
  // 向路径中加入初始节点
  path.push(u);
  visited[u] = true;
  bool found = false;
  while (!path.empty()) {
    int from = path.top();
    printf("path: ");
    for (int i = 0; i < path.size(); i++) printf("%d ", path[i]);
    puts("");
    bool will_pop = true;
    for (int t = 0; t < table[from].size(); t++) {
      int &to = table[from][t];
      if (!visited[to]) {
        will_pop = false;
        path.push(to);
        visited[to] = true;
        break;
      }
    }
    if (will_pop) {
      path.pop();
    }
  }
  delete[] visited;
  return found;
}

int main() {
  int *data = new int[512]();
  bool *visited2 = new bool[8]();
  // 建立一个最大节点数量为8的邻接表
  AdjacencyTable table(8);
  // 建立图
  //  table.add_directed_edge(0, 1);
  //  table.add_directed_edge(0, 2);
  //  table.add_directed_edge(1, 2);
  //  table.add_directed_edge(1, 3);
  //  table.add_directed_edge(3, 7);
  //  table.add_directed_edge(1, 4);
  //  table.add_directed_edge(2, 4);
  //  table.add_directed_edge(2, 5);
  //  table.add_directed_edge(5, 6);
  //  table.add_directed_edge(4, 6);
  //  table.add_directed_edge(6, 7);
  table.add_edges(0, 1);
  table.add_edges(0, 2);
  table.add_edges(1, 2);
  table.add_edges(1, 3);
  table.add_edges(3, 7);
  table.add_edges(1, 4);
  table.add_edges(2, 4);
  table.add_edges(2, 5);
  table.add_edges(5, 6);
  table.add_edges(4, 6);
  table.add_edges(6, 7);
  // dfs(table, 0);
  puts("=============================");
  if (!dfs_path(table, 0, 7)) {
    puts("Path not found!");
  }
  puts("=============================");
  bool *visited = new bool[8];
  Stack path(512);
  for (int i = 0; i < 8; i++) visited[i] = false;
  visited[0] = true;
  path.push(0);
  ddd(table, 0, 7, visited, path);
  delete[] visited;
  return 0;
}