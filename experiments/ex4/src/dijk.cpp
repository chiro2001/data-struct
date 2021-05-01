#include <cstring>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>
using namespace std;

#define MY_INF 0x3f3f3f3f
#define MAX_N 10001
#define MAX_M 500001

// 链式前向星
// next表示同起点的下一条边的下标
class Edge {
 public:
  int to = 0;
  int w = MY_INF;
  int next = 0;
  Edge() {}
  Edge(int _to, int weight) : to(_to), w(weight) {}
  Edge(int _to, int weight, int _next) : to(_to), w(weight), next(_next) {}
};

class Node {
 public:
  int dis = MY_INF;
  int pos = 0;
  Node(int p, int d) : dis(d), pos(p) {}
  bool operator<(const Node& a) const { return a.dis < this->dis; }
};

// 表示点的个数、有向边的个数、出发点的编号
int n, m, s;
// 起点节点到各个节点目前的最短距离
int dis[MAX_N];
// priority_queue<int> dis;
// 是否已经访问到，即是否已经是最优解
bool vis[MAX_N];
// 存边
Edge edges[MAX_M];
// 记录每条边对应起始点连接的不同一组边最末端的下标
// heads[i]表示第i个点最近加入的边的下标，0表示没有出边
int heads[MAX_N] = {0};
// edge_cnt表示edges最近添加的边的下标
int edge_cnt = 0;

void add_edge(int from, int to, int weight) {
  edge_cnt++;
  // 最后会访问到0
  // if (heads[from] == 0) heads[from] = edge_cnt;
  edges[edge_cnt] = Edge(to, weight);
  edges[edge_cnt].next = heads[from];
  heads[from] = edge_cnt;
}

// Edge对象转换成字符串输出
string Edge2String(Edge e, int from = 0) {
  ostringstream out;
  if (from) out << from << "=>";
  out << e.to << "(" << e.w << ")";
  return out.str();
}

// void Dijkstra() {
//   dis[s] = 0;
//   // 注意这里不能把s加入已经知道最短路径的节点集合
//   // vis[s] = true;
//   // 初始化dis
//   for (int i=heads[s]; i; i = edges[i].next) {
//     dis[edges[i].to] = edges[i].w;
//   }
//   int min_k, min_w;
//   for (int i=1; i<=n; i++) {
//     // 找到dis中最小的对应节点
//     min_w = MY_INF;
//     for (int k=1; k<=n; k++) {
//       if (!vis[k] && min_w >= dis[k]) {
//         min_w = dis[k], min_k = k;
//       }
//     }
//     if (vis[min_k]) {
//       for (int j=1; j<=n; j++)
//         if (!vis[j]) {
//           min_k = j;
//           break;
//         }
//     }
//     // 读边
//     int from = min_k;
//     vis[from] = true;
//     for (int i=heads[from]; i; i = edges[i].next) {
//       int to = edges[i].to;
//       int w = edges[i].w;
//       if (!vis[to] && dis[to] > dis[from] + w) {
//         dis[to] = dis[from] + w;
//         // 更新了边不能证明这里就是最优解
//         // vis[to] = true;
//       }
//     }
//   }
// }

void Dijkstra() {
  priority_queue<Node> q;
  // 不需要初始化
  // // 初始化即将考虑的边的集合
  // for (int i = heads[s]; i; i = edges[i].next) {
  //   dis[edges[i].to] = edges[i].w;
  // }
  dis[s] = 0;
  // 加入开头
  q.push(Node(s, 0));

  while (!q.empty()) {
    Node top = q.top();
    q.pop();
    if (vis[top.pos]) continue;
    // top对应最小dis下标
    int from = top.pos;
    vis[from] = true;

    for (int i = heads[from]; i; i = edges[i].next) {
      int to = edges[i].to;
      int w = edges[i].w;
      // cout << Edge2String(edges[i], from) << endl;
      // cout << "vis[" << to << "] = " << vis[to] << endl;
      // cout << "Judge: dis[to] > dis[from] + w => " << dis[to] << " < "
      //      << dis[from] + w << endl;

      if (dis[to] > dis[from] + w) {
        // cout << "Update: dis[" << to << "](" << dis[to] << ") => "
        //      << dis[from] + w << endl;
        dis[to] = dis[from] + w;
        if (!vis[to]) {
          q.push(Node(to, dis[to]));
          // cout << "Push: Node (to=" << to << ", dis=" << dis[to] << ")" << endl;
        }
        // 更新了边不能证明这里就是最优解
        // vis[to] = true;
      }
    }
  }
}

int main() {
  cin >> n >> m >> s;
  // 表示一条 u→v 的，长度为 w 的边
  int u, v, w;
  for (int i = 0; i <= n; i++) dis[i] = MY_INF, vis[i] = false;
  for (int i = 0; i < m; i++) {
    cin >> u >> v >> w;
    if (u == v) continue;
    add_edge(u, v, w);
  }
  Dijkstra();
  for (int i = 1; i <= n; i++)
    cout << (dis[i] == MY_INF ? 2147483647 : dis[i]) << " ";
  cout << endl;
}