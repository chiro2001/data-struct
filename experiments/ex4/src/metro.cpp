#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "chilib/chistring.hpp"
#include "chilib/chivector.hpp"
#include "chilib/queue.hpp"
#include <queue>

// Constants
// 地铁站名称最长7个中文字
const int METRO_NAME_MAX = 15;
// 最大地铁长度
const int METRO_LEN_MAX = 0x3f3f3f3f;
// 最多地铁边数
const int METRO_EDGES_MAX = 5000;

class linked_edges : chilib::iterator_base {
public:
  class edge_node {
  public:
    int to = 0;
    int weight = METRO_LEN_MAX;
    int next = 0;
    int line{};

    edge_node() = default;

    explicit edge_node(int to_, int weight_, int next_, int line_) : to(to_), weight(weight_), next(next_),
                                                                     line(line_) {}

    // 重载输出
    friend std::ostream &operator<<(std::ostream &out, const edge_node &node) {
      out << "node(line=" << node.line << ", to=" << node.to << ", weight=" << node.weight << ", next=" << node.next
          << ")";
      return out;
    }
  };

  size_t n;
  size_t node_size = 0;
  std::shared_ptr<chilib::vector<int>> distance = nullptr;
  std::shared_ptr<chilib::vector<bool>> visit = nullptr;
  std::shared_ptr<chilib::vector<edge_node>> edges = nullptr;
  std::shared_ptr<chilib::vector<int>> heads = nullptr;
  int cnt = 0;
  int m_from{};

  linked_edges &set_from(int from) {
    // 并不线程安全呢
    m_from = from;
    return *(this);
  }

  void data_init() {
    distance = std::make_shared<chilib::vector<int>>(n);
    visit = std::make_shared<chilib::vector<bool>>(n);
    heads = std::make_shared<chilib::vector<int>>(n);
    edges = std::make_shared<chilib::vector<edge_node>>(n);
  }

  explicit linked_edges(size_t size) : n(size) { data_init(); }

  void add_edge(int from, int to, int weight, int line) {
    cnt++;
    (*edges)[cnt] = edge_node(to, weight, (*heads)[from], line);
//    std::cout << "add_edge: from=" << from << ",\t" << edges[cnt] << std::endl;
    (*heads)[from] = cnt;
  }

  /*!
   * 迭代器类
   */
  class iterator : public iterator_instance<size_t> {
  private:
    chilib::vector<edge_node> &edges;
  public:
    explicit iterator(chilib::vector<edge_node> &edges_, size_t index) : iterator_instance(index), edges(edges_) {}

    iterator &operator++() {
      this->node = edges[this->node].next;
      return *this;
    }

    edge_node &operator*() {
      return edges[this->node];
    }
  };

  /*!
   * 获取起始迭代器
   * @return 起始迭代器
   */
  iterator begin() {
    return iterator(*edges, (*heads)[m_from]);
  }

  /*!
   * 获取末尾迭代器
   * @return 末尾迭代器
   */
  iterator end() {
    return iterator(*edges, 0);
  }
};

class node_order {
public:
  int dis = METRO_LEN_MAX;
  int pos = 0;

  node_order() = default;

  node_order(int p, int d) : dis(d), pos(p) {}

  bool operator<(const node_order &a) const { return a.dis < this->dis; }

  bool operator==(const node_order &a) const { return a.pos == this->pos; }
};

void data_read_metro(const char *filename, linked_edges &edges) {
  FILE *file;
  int error = 0;
  error = fopen_s(&file, filename, "r");
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
  edges.node_size = total_stations;
}

int data_read_metro_names(const char *filename, chilib::vector<chilib::string> &names) {
  FILE *file;
  int error = 0;
  error = fopen_s(&file, filename, "r");
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

void dfs(int from, chilib::vector<bool> &visited, linked_edges &edges) {
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

bool is_map_connected(linked_edges &edges) {
  chilib::vector<bool> visited(edges.n + 1);
  int head_first = 0;
  while ((*edges.heads)[head_first] == 0) head_first++;
//  printf("head_first = %d\n", head_first);
  // BFS
  chilib::vector<int> queue;
  queue.emplace_back(head_first);
  visited[head_first] = true;
  while (!queue.empty()) {
    int from = queue.pop_front();
    try {
      for (const auto &edge : edges.set_from(from)) {
//        std::cout << "from=" << from << ", " << edge << std::endl;
        int to = edge.to;
        if (!visited[to]) {
          visited[to] = true;
//          printf("from => to: %d => %d\n", from, to);
//          printf("######## push: %d\n", to);
          queue.emplace_back(to);
//          queue.push(to);
        }
      }
    } catch (std::out_of_range &e) {
      std::cerr << e.what() << " when from = " << from << std::endl;
      throw e;
    }
  }
  for (int i = 1; i <= edges.node_size; i++)
    if (!visited[i]) {
//      printf("ID %d not visited!\n", i);
      return false;
    }
  return true;
}

void draw(linked_edges &edges, chilib::vector<chilib::string> &names) {
  int error;
  int total_stations = edges.node_size;
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
  system("python ../utf8.py out.dot");
  system("dot -Tpng out.dot -o out.png && start out.png");
}

int find_most_exchanged(linked_edges &edges, int &max_degree) {
  int id = -1;
  for (int i = 1; i <= edges.node_size; i++) {
    int degree = 0;
    for (const auto &edge : edges.set_from(i)) {
      degree++;
    }
    if (degree > max_degree) {
      max_degree = degree;
      id = i;
    }
  }
//  printf("most: id=%d, degree=%d\n", id, max_degree);
  return id;
}

void dijkstra(int start, linked_edges &edges, chilib::vector<int> &distance) {
  chilib::vector<bool> visited(edges.node_size + 1);
  distance = chilib::vector<int>(edges.node_size + 1);
  for (auto &d : distance) d = METRO_LEN_MAX;
  std::priority_queue<node_order> q;
//  chilib::priority_queue<node_order> q;
  distance[start] = 0;
  q.push(node_order(start, 0));
  while (!q.empty()) {
    node_order top = q.top();
    q.pop();
    if (visited[top.pos]) continue;
    int from = top.pos;
//    printf("####### from: %d, dis: %d\n", from, top.dis);
    visited[from] = true;

    for (const auto &edge : edges.set_from(from)) {
      int to = edge.to, weight = edge.weight;
//      printf("from=%d\tto=%d\tweight=%d\n", from, to, weight);
      if (distance[to] > distance[from] + weight) {
        distance[to] = distance[from] + weight;
        if (!visited[to]) q.push(node_order(to, distance[to]));
      }
    }
  }
}

void compute_ecc(linked_edges &edges, int &d, int &r) {
  d = 0, r = METRO_LEN_MAX;
  for (int i = 1; i <= edges.node_size; i++) {
    int ecc = 0;
    chilib::vector<int> distance;
    dijkstra(i, edges, distance);
    for (int j = 1; j <= edges.node_size; j++) {
      ecc = ecc > distance[j] ? ecc : distance[j];
    }
    d = d < ecc ? ecc : d;
    r = r > ecc ? ecc : r;
  }
}

void dijkstra_path(int start, linked_edges &edges,
                   chilib::vector<int> &distance,
                   chilib::vector<int> &path) {
  chilib::vector<bool> visited(edges.node_size + 1);
  distance = chilib::vector<int>(edges.node_size + 1);
  path = chilib::vector<int>(edges.node_size + 1);
  for (auto &d : distance) d = METRO_LEN_MAX;
  std::priority_queue<node_order> q;
//  chilib::priority_queue<node_order> q;
  distance[start] = 0;
  q.push(node_order(start, 0));
  while (!q.empty()) {
    node_order top = q.top();
    q.pop();
    if (visited[top.pos]) continue;
    int from = top.pos;
//    printf("####### from: %d, dis: %d\n", from, top.dis);
    visited[from] = true;

    for (const auto &edge : edges.set_from(from)) {
      int to = edge.to, weight = edge.weight;
//      printf("from=%d\tto=%d\tweight=%d\n", from, to, weight);
      if (distance[to] > distance[from] + weight) {
        distance[to] = distance[from] + weight;
        path[to] = from;
//        if (from == 83)
//          printf("path[%d] = %d\n", to, from);
        if (!visited[to]) {
          q.push(node_order(to, distance[to]));
        }
      }
    }
  }
}

int main() {
//  priority_queue<int, greater<int>> q;
//  for (int i = 0; i < 100; i++) q.push(100 - i);
//  int count = 0;
//  while (!q.empty()) {
//    printf("%d\n", q.top());
//    count++;
//    q.pop();
//  }
//  printf("count: %d\n", count);
//  return 0;
  const char *metro_data = "metro.txt", *metro_name_data = "no2metro.txt";
  linked_edges edges(METRO_EDGES_MAX);
  chilib::vector<chilib::string> names;
  data_read_metro(metro_data, edges);
  data_read_metro_names(metro_name_data, names);
//  for (int i = 0; i < names.size(); i++) {
//    std::cout << "id=" << i << ", " << names[i] << std::endl;
//  }

//  int from_test = 53;
//  for (const auto &edge : edges.set_from(from_test)) {
//    std::cout << "from=" << from_test << ", " << edge << std::endl;
//  }
//  for (int from = 1; from <= total_stations; from++) {
//    for (const auto &edge : edges.set_from(from)) {
//      if (edge.to == from_test) {
//        printf("found %d\n", from_test);
//        std::cout << "from=" << from << ", " << edge << std::endl;
//      }
//    }
//  }
//  return 0;

//  chilib::vector<bool> visited(edges.n + 1);
//  dfs(1, visited, edges);
  printf("这个图%s联通的.\n", is_map_connected(edges) ? "是" : "否");
  int max_degree = 0;
  int most_exchanged = find_most_exchanged(edges, max_degree);
  printf("线路图中换乘线路最多的站点是%s, 共有 %d 条线路通过.\n", names[most_exchanged - 1].c_str(), max_degree);
//  draw(total_stations, edges);
  chilib::vector<int> distance;
  dijkstra(most_exchanged, edges, distance);
//  printf("distance: ");
//  for (const auto d : distance) {
//    printf("%d ", d == METRO_LEN_MAX ? -1 : d);
//  }
//  puts("");
  int d, r;
  compute_ecc(edges, d, r);
  printf("该线路图的直径是 %d, 半径是 %d.\n", d, r);
  int station1 = -1, station2 = -1;
  chilib::string station1_name = "大学城",
//          station2_name = "机场";
          station2_name = "深圳湾公园";
  for (int n = 1; n < names.size(); n++) {
    if (names[n] == station1_name) station1 = n;
    if (names[n] == station2_name) station2 = n;
  }
  if (station1 < 0 || station2 < 0) {
    printf("找不到站点: %s 或者 %s!\n", station1_name.c_str(), station2_name.c_str());
    return 1;
  }
//  printf("station1: %d, %s; station2: %d, %s\n", station1, station1_name.c_str(), station2, station2_name.c_str());
  station1++, station2++;
  chilib::vector<int> path;
  dijkstra_path(station1, edges, distance, path);
  printf("从%s站到%s站最少需要 %d 分钟, 路径: \n", station1_name.c_str(), station2_name.c_str(), distance[station2]);
  chilib::vector<int> stack;
  int p = station2;
  while (p > 0) {
    stack.emplace_front(p - 1);
    if (p == station1) break;
    p = path[p];
  }
  for (const auto st : stack) {
    printf("%s", names[st].c_str());
    if (st != station2 - 1) printf(" --> ");
  }
  puts("");
//  draw(edges, names);
  return 0;
}