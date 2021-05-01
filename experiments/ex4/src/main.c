#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_dis 100000
// 最大顶点数
#define MAX_SIZE 300
#define MAX_PATH_LEN 300
// 宏: 调试模式
// #define DEBUG
#ifndef dep
#ifdef DEBUG
#define dep(...)         \
  do {                   \
    printf(__VA_ARGS__); \
  } while (0)
#else
#define dep(...)
#endif
#endif

typedef char vextype[20];
typedef struct {
  int N, E;         // N是顶点数，E是边数
  int **matrix;     //储存邻接矩阵
  vextype *vertex;  //存储节点的名字
} Graph;

Graph createGraph(int n);
int isConnected(Graph g);
void nodeDegree(Graph g, int *node_degree);
double clusteringCoefficient(Graph g);
void computeEcc(Graph g, int *diameter, int *radius);
int dijkstra(Graph g, int start, int end, int *path);
void printPath(int d, int *diameter_path, Graph g);

/**
 * 创建一个节点数为n的图
 * @param n 节点个数
 * @return 返回这个图
 */
Graph createGraph(int n) {
  int i, j;
  Graph g;
  g.N = n;
  g.matrix = (int **)malloc(sizeof(int *) * g.N);
  for (i = 0; i < n; i++) {
    g.matrix[i] = (int *)malloc(sizeof(int) * g.N);
  }
  for (i = 0; i < g.N; i++) {
    for (j = 0; j < g.N; j++) {
      g.matrix[i][j] = max_dis;
    }
  }
  for (i = 0; i < g.N; i++) {
    g.matrix[i][i] = 0;
  }
  g.vertex = (vextype *)malloc(sizeof(vextype) * g.N);
  return g;
}

/**
 * 根据距离d和路径数组path输出路径，这样就不需要路径的节点数也能正确输出路径
 * @param d 路径长度
 * @param path 储存路径的数组
 * @param g 图
 */
void printPath(int d, int *path, Graph g) {
  int k = 0;
  int path_length = 0;
  printf("Path: ");
  do {
    printf("%s ", g.vertex[path[k]]);
    path_length += g.matrix[path[k]][path[k + 1]];
    k++;
  } while (path_length < d);
  printf("%s\n", g.vertex[path[k]]);
}

/**
 * 判断图是否连通
 * @param g 图
 * @return 连通返回1，否则返回0
 */
int isConnected(Graph g) {
  // BFS能够遍历到所有的节点就是连通的
  int queue[MAX_SIZE * 3], vis[MAX_SIZE];
  for (int i = 0; i < g.N; i++) vis[i] = 0;
  int head = 0, tail = 0;
  queue[tail++] = 0;
  while (tail - head != 0) {
    int top = queue[head++];
    dep("get top: %d\n", top);
    for (int to = 0; to < g.N; to++)
      if (g.matrix[top][to] < max_dis) {
        if (!vis[to]) {
          queue[tail++] = to;
          vis[to] = 1;
        }
      }
  }
  for (int i = 0; i < g.N; i++)
    if (!vis[i]) return 0;
  return 1;
}

/**
 * 计算每个点的度
 * @param g 图
 * @param node_degree 将每个点的度写到这个数组中
 */
void nodeDegree(Graph g, int *node_degree) {
  for (int i = 0; i < g.N; i++) node_degree[i] = 0;
  for (int i = 0; i < g.N; i++) {
    for (int j = 0; j < g.N; j++) {
      if (i != j && g.matrix[i][j] < max_dis) {
        node_degree[i]++;
      }
    }
  }
}

/**
 * 计算图的聚类系数
 * 顶点的聚类系数：一个顶点，它有K个邻居顶点，
 * 这k个邻居顶点之间实际存在的边的个数比上这k个邻居顶点最多可能存在的边的个数（也即C^2_k
 * = (k(k-1))/2）， 这个比值就是这个顶点的聚类系数。
 * @param g 图
 * @return 返回聚类系数
 */
double clusteringCoefficient(Graph g) {
  double ave = 0;
  for (int i = 0; i < g.N; i++) {
    int degree = 0;
    int neighbor[MAX_SIZE];
    // 选出所有的邻居节点
    for (int j = 0; j < g.N; j++) {
      if (i != j && g.matrix[i][j] < max_dis) {
        neighbor[degree++] = j;
      }
    }
    if (degree == 0) continue;
    dep("node now: %d\nneighbors: ", i);
    for (int j = 0; j < degree; j++) dep("%d ", neighbor[j]);
    dep("\n");
    int edges = 0;
    // 找这些邻居节点之间存在的边数
    for (int j = 0; j < degree; j++) {
      for (int k = 0; k < degree; k++) {
        if (neighbor[j] == neighbor[k]) continue;
        if (g.matrix[neighbor[j]][neighbor[k]] < max_dis) edges++;
      }
    }
    // 每条边都多数了一次
    edges /= 2;
    dep("degree: %d, edges: %d\n", degree, edges);
    if (degree > 1) ave += (double)(edges) / ((degree * (degree - 1) / 2));
  }
  ave /= g.N;
  return ave;
}

/**
 * 使用dijkstra算法计算单源最短路径
 * @param g 图
 * @param start 起点
 * @param end 终点
 * @param path 从start到end的路径, [start,...,end]
 * @return 路径长度
 */
int dijkstra(Graph g, int start, int end, int *path) {
  int dis[MAX_SIZE], vis[MAX_SIZE];
  int dis_path[MAX_SIZE][MAX_PATH_LEN];
  int dis_path_tail[MAX_SIZE];
  // 初始化数据
  for (int i = 0; i < g.N; i++)
    dis[i] = max_dis, vis[i] = 0, dis_path_tail[i] = 1, dis_path[i][0] = start;

  dis[start] = 0;
  vis[start] = 1;
  for (int i = 0; i < g.N; i++)
    for (int visited = 0; visited < g.N; visited++) {
      // 找到在集合 vis 中的所有的点 visited
      if (!vis[visited]) continue;
      // 在 visited 的所有的点的邻居当中找到
      // 1. 距离源点最近的 2. 在未访问过的
      int target = -1;
      int target_min = max_dis;
      for (int to = 0; to < g.N; to++) {
        if (vis[to]) continue;
        if (target_min > dis[visited] + g.matrix[visited][to]) {
          target_min = dis[visited] + g.matrix[visited][to];
          target = to;
        }
      }
      if (target < 0) continue;
      vis[target] = 1;
      dis[target] = target_min;
      dis_path[target][dis_path_tail[target]++] = visited;
    }
  // for (int i = 0; i < g.N; i++) dis_path[i][dis_path_tail[i]++] = i;
  dep("dis: ");
  for (int i = 0; i < g.N; i++) dep("%d ", dis[i]);
  dep("\n");
  dep("path: ");
  for (int i = 0; i < dis_path_tail[end]; i++) {
    dep("%d ", dis_path[end][i]);
    path[i] = dis_path[end][i];
  }
  path[dis_path_tail[end]] = end;
  dep("%d\n", end);
  return dis[end];
}

/**
 * 计算图的直径和半径，提示: Floyd算法
 * @param g 图
 * @param diameter 指向直径变量的指针
 * @param radius 指向半径变量的指针
 * @return void
 */
void computeEcc(Graph g, int *diameter, int *radius) {
  int dis[MAX_SIZE][MAX_SIZE];
  // int path[MAX_SIZE][MAX_SIZE];
  for (int i = 0; i < g.N; i++) {
    for (int j = 0; j < g.N; j++) {
      dis[i][j] = i == j ? 0 : g.matrix[i][j];
      // path[i][j] = -1;
    }
  }
  for (int k = 0; k < g.N; k++) {
    for (int i = 0; i < g.N; i++) {
      for (int j = 0; j < g.N; j++) {
        if (dis[i][j] > dis[i][k] + dis[k][j]) {
          dis[i][j] = dis[i][k] + dis[k][j];
          // path[i][j] = k;
        }
      }
    }
  }
  int d = 0, r = max_dis;
  for (int i = 0; i < g.N; i++) {
    // 当前: i，查看当前Eccentricity
    int ecc = 0;
    for (int j = 0; j < g.N; j++) {
      ecc = ecc < dis[i][j] ? dis[i][j] : ecc;
    }
    d = d < ecc ? ecc : d;
    r = r > ecc ? ecc : r;
  }
  *diameter = d, *radius = r;
}

int main() {
  int node_num;
  int edge_num;
  int ca = 1;
  if (freopen("stu.in", "r", stdin) == NULL) {
    printf("There is an error in reading file stu.in");
  }
  while (scanf("%d %d\n", &node_num, &edge_num) != EOF) {
    printf("\ncase %d:\n", ca++);
    int start_idx, end_idx, weight;
    Graph g = createGraph(node_num);
    for (int i = 0; i < node_num; i++) {
      sprintf(g.vertex[i], "%d", i);
    }
    for (int i = 0; i < edge_num; i++) {
      scanf("%d %d %d\n", &start_idx, &end_idx, &weight);
      g.matrix[start_idx][end_idx] = weight;
      g.matrix[end_idx][start_idx] = weight;
    }

    printf("connected: %d\n", isConnected(g));

    int *degree = (int *)malloc(sizeof(int) * g.N);
    nodeDegree(g, degree);
    printf("degree distribution:\n");
    for (int i = 0; i < g.N; i++) {
      printf("node%s:%d,", g.vertex[i], degree[i]);
    }
    printf("\n");
    free(degree);

    double c = clusteringCoefficient(g);
    printf("clustering coefficient:%f\n", c);

    if (isConnected(g)) {
      int *short_path = (int *)malloc(sizeof(int) * g.N);
      int dis = dijkstra(g, 1, 3, short_path);
      printf("the shortest path between 1 and 3: %d\n", dis);
      printPath(dis, short_path, g);
      free(short_path);

      int diameter, radius;
      computeEcc(g, &diameter, &radius);
      printf("diameter:%d\n", diameter);
      printf("radius:%d\n", radius);
    }
  }
  return 0;
}
