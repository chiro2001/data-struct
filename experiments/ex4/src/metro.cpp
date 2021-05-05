#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "chilib/chistring.hpp"
#include "chilib/chivector.hpp"
#include "chilib/queue.hpp"
// 链式前向星放这里了
#include "chilib/linked_edges.hpp"

// Constants
// 地铁站名称最长7个中文字
const int METRO_NAME_MAX = 15;
// 最大地铁长度
const int METRO_LEN_MAX = INT32_MAX;
// 最多地铁边数
const int METRO_EDGES_MAX = 5000;

class node_order {
public:
  int dis = METRO_LEN_MAX;
  int pos = 0;

  node_order() = default;

  node_order(int p, int d) : dis(d), pos(p) {}

  bool operator<(const node_order &a) const { return a.dis < this->dis; }

  bool operator==(const node_order &a) const { return a.pos == this->pos; }
};

// 读取地铁线路信息
void data_read_metro(const char *filename, chilib::linked_edges &edges) {
  FILE *file;
  int error = fopen_s(&file, filename, "r");
  if (error) throw std::runtime_error((chilib::string("Can not open file ") + filename).c_str());
  // 节点总数
  int total_stations, total_lines;
  fscanf(file, "%d%d", &total_stations, &total_lines);
  for (int line_id = 0; line_id < total_lines; line_id++) {
    int stations;
    fscanf(file, "%d", &stations);
    int from = -1;
    for (int station = 0; station < stations; station++) {
      int id, time;
      fscanf(file, "%d%d", &id, &time);
      if (from < 0) {
        from = id + 1;
        continue;
      }
      edges.add_edge(from, id + 1, time, line_id);
      edges.add_edge(id + 1, from, time, line_id);
      from = id + 1;
    }
  }
  edges.get_node_size() = total_stations;
}

// 读取地铁站点信息
int data_read_metro_names(const char *filename, chilib::vector<chilib::string> &names) {
  FILE *file;
  int error = fopen_s(&file, filename, "r");
  if (error) throw std::runtime_error((chilib::string("Can not open file ") + filename).c_str());
  // 节点总数
  int total_stations;
  fscanf(file, "%d", &total_stations);
  // 重新初始化
  names = chilib::vector<chilib::string>(total_stations);
  for (int i = 0; i < total_stations; i++) {
    char buf[METRO_NAME_MAX];
    int id;
    fscanf(file, "%d%s", &id, buf);
    names[id] = buf;
  }
  return total_stations;
}

// DFS 深度优先遍历
void dfs(int from, chilib::vector<bool> &visited, chilib::linked_edges &edges) {
  printf("%3d", from);
  bool ended = true;
  for (const auto &edge : edges.set_from(from)) {
    int to = edge.to;
    ended = false;
    if (!visited[to]) {
      visited[to] = true;
      printf(" --- ");
      dfs(to, visited, edges);
    }
  }
  if (ended) {
    printf("\n\n");
    printf("%3d ", from);
  }
}

// 判断图是否联通
bool is_map_connected(chilib::linked_edges &edges) {
  chilib::vector<bool> visited(edges.get_node_size() + 1);
  int head_first = edges.get_head_first();
  // BFS
  chilib::vector<int> queue;
  queue.emplace_back(head_first);
  visited[head_first] = true;
  while (!queue.empty()) {
    int from = queue.pop_front();
    try {
      for (const auto &edge : edges.set_from(from)) {
        int to = edge.to;
        if (!visited[to]) {
          visited[to] = true;
          queue.emplace_back(to);
        }
      }
    } catch (std::out_of_range &e) {
      std::cerr << e.what() << " when from = " << from << std::endl;
      throw e;
    }
  }
  for (size_t i = 1; i <= edges.get_node_size(); i++)
    if (!visited[i]) {
//      printf("ID %d not visited!\n", i);
      return false;
    }
  return true;
}

// Graphviz绘制地铁图像
void draw(chilib::linked_edges &edges, chilib::vector<chilib::string> &names) {
  int error;
  int total_stations = edges.get_node_size();
  FILE *fp;
  error = freopen_s(&fp, "out.dot", "w", stdout);
  if (error != 0) throw std::runtime_error("Can not create .dot file!");
  fprintf(fp, "digraph G {\nnode [fontname=\"SimHei\"];\n");
  for (int i = 1; i <= total_stations; i++) {
    fprintf(fp, "N%d [shape=circle, label=\"%s\"];\n", i - 1, names[i - 1].c_str());
  }
  for (int from = 1; from <= total_stations; from++) {
    for (const auto &edge : edges.set_from(from)) {
      fprintf(fp, "N%d->N%d;\n", from - 1, edge.to - 1);
    }
  }
  fprintf(fp, "}\n");
  fflush(fp);
  fclose(fp);
  // GBK 转为 UTF8...请确认已经安装 Python 3
  system("python ../utf8.py out.dot");
  system("dot -Tpng out.dot -o out.png && start out.png");
}

// 找经过最多线路的站点
int find_most_exchanged(chilib::linked_edges &edges, int &max_degree) {
  int id = -1;
  for (size_t i = 1; i <= edges.get_node_size(); i++) {
    int degree = 0;
    auto it = edges.set_from(i).begin();
    while (it != edges.end()) {
      ++it;
      degree++;
    }
    if (degree > max_degree) {
      max_degree = degree;
      id = i;
    }
  }
  return id;
}

void dijkstra(int start, chilib::linked_edges &edges,
              chilib::vector<int> &distance,
              chilib::vector<int> *path) {
  // 初始化 visited, path 和 distance 数组
  chilib::vector<bool> visited(edges.get_node_size() + 1);
  distance = chilib::vector<int>(edges.get_node_size() + 1);
  if (path) *path = chilib::vector<int>(edges.get_node_size() + 1);
  // 距离初始化为最大值
  for (auto &d : distance) d = METRO_LEN_MAX;
  // 虽然这两个优先队列用法一致，但是自己写的这个得开 -O2 性能才赶得上STL的...
  // std::priority_queue<node_order> q;
  chilib::priority_queue<node_order> q;
  // 记录起点距离
  distance[start] = 0;
  // 添加起点
  q.push(node_order(start, 0));
  while (!q.empty()) {
    // 取出当前可访问到的边的最小的边
    node_order top = q.top();
    q.pop();
    if (visited[top.pos]) continue;
    int from = top.pos;
    visited[from] = true;

    // 利用链式前向星容器遍历边
    for (const auto &edge : edges.set_from(from)) {
      int to = edge.to, weight = edge.weight;
      // 更新最短边数据
      if (distance[to] > distance[from] + weight) {
        distance[to] = distance[from] + weight;
        // 添加路径
        if (path) (*path)[to] = from;
        if (!visited[to]) {
          q.push(node_order(to, distance[to]));
        }
      }
    }
  }
}


// 计算ecc、直径、半径
void compute_ecc(chilib::linked_edges &edges, int &d, int &r) {
  d = 0, r = METRO_LEN_MAX;
  for (size_t i = 1; i <= edges.get_node_size(); i++) {
    int ecc = 0;
    chilib::vector<int> distance;
    dijkstra(i, edges, distance, nullptr);
    // ecc: 单源距离最大值
    for (size_t j = 1; j <= edges.get_node_size(); j++) {
      ecc = ecc > distance[j] ? ecc : distance[j];
    }
    // d: ecc 的最大值, r: ecc 的最小值
    d = d < ecc ? ecc : d;
    r = r > ecc ? ecc : r;
  }
}

int main() {
  // 配置
  // 打开这个开关，会使用Graphviz绘制整个地铁站的图像
  const bool draw_image = false;
  // 起点和目的地名称
  chilib::string station1_name = "大学城",
          station2_name = "机场";
//          station2_name = "深圳湾公园";
  // 文件名
  const char *metro_data = "metro.txt", *metro_name_data = "no2metro.txt";
  chilib::linked_edges edges(METRO_EDGES_MAX);
  chilib::vector<chilib::string> names;
  data_read_metro(metro_data, edges);
  data_read_metro_names(metro_name_data, names);
  printf("这个图%s联通的.\n", is_map_connected(edges) ? "是" : "否");
  int max_degree = 0;
  int most_exchanged = find_most_exchanged(edges, max_degree);
  printf("线路图中换乘线路最多的站点是%s, 共有 %d 条线路通过.\n", names[most_exchanged - 1].c_str(), max_degree);
  chilib::vector<int> distance;
  dijkstra(most_exchanged, edges, distance, nullptr);
// 打印最短长度信息
//  printf("distance: ");
//  for (const auto d : distance) {
//    printf("%d ", d == METRO_LEN_MAX ? -1 : d);
//  }
//  puts("");
  int d, r;
  compute_ecc(edges, d, r);
  printf("该线路图的直径是 %d, 半径是 %d.\n", d, r);
  int station1 = -1, station2 = -1;
  for (size_t n = 1; n < names.size(); n++) {
    if (names[n] == station1_name) station1 = n;
    if (names[n] == station2_name) station2 = n;
  }
  if (station1 < 0 || station2 < 0) {
    printf("找不到站点: %s 或者 %s!\n", station1_name.c_str(), station2_name.c_str());
    return 1;
  }
  // printf("station1: %d, %s; station2: %d, %s\n", station1, station1_name.c_str(), station2, station2_name.c_str());
  station1++, station2++;
  chilib::vector<int> path;
  dijkstra(station1, edges, distance, &path);
  printf("从%s站到%s站最少需要 %d 分钟, 路径: \n", station1_name.c_str(), station2_name.c_str(), distance[station2]);
  // 用栈把路径反过来
  chilib::vector<int> stack;
  int p = station2;
  while (p > 0) {
    stack.emplace_front(p - 1);
    if (p == station1) break;
    p = path[p];
  }
  // 打印路径
  for (const auto st : stack) {
    printf("%s", names[st].c_str());
    if (st != station2 - 1) printf(" --> ");
  }
  puts("");
  if (draw_image) draw(edges, names);
  return 0;
}
