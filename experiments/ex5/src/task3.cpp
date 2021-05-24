#include <cstdio>
#include <map>
#include "chilib/algo.hpp"

class Period {
public:
  int start{};
  int end{};

  bool operator<(const Period &a) const {
    return this->start < a.start;
  }

  bool operator>(const Period &a) const {
    return this->start > a.start;
  }
};

using StuData = Period;

#define PINT(x) do {            \
printf(#x " = %d\n", x);        \
fflush(stdout);                 \
} while(0)

// #define DEBUG

#ifdef DEBUG
#define dep(...) printf(__VA_ARGS__)
#else
#define dep(...)
#endif

int main() {
  int n, m;
  // 读取数据
  freopen("5_3_input.in", "r", stdin);
  int case_count = 1;
  while (scanf("%d%d", &n, &m) > 0) {
    printf("==== Case %d ====\n", case_count++);
    // 用优先队列
    chilib::priority_queue<StuData, chilib::greater<StuData>> q;
    chilib::priority_queue<StuData, chilib::greater<StuData>> tmp;
    // 用来记录有多少阶段
    chilib::priority_queue<int, chilib::greater<int>> periods_queue;
    std::map<int, bool> periods_map;
    chilib::vector<int> periods;
    chilib::vector<Period> periods_data;
    chilib::vector<int> points_count;
    // 从start转换到index
    std::map<int, int> start2index;
    for (int i = 0; i < m; i++) {
      StuData d;
      scanf("%d%d", &d.start, &d.end);
      q.push(d);
      tmp.push(d);
      if (periods_map.find(d.start) == periods_map.end()) {
        periods_queue.push(d.start);
        periods_map[d.start] = true;
      }
      if (periods_map.find(d.end) == periods_map.end()) {
        periods_queue.push(d.end);
        periods_map[d.end] = true;
      }
    }
    dep("origin data: ");
    while (!tmp.empty()) {
      auto top = tmp.top();
      dep("Period{%d, %d} ", top.start, top.end);
      tmp.pop();
    }
    dep("\n");
    int started = -1;
    while (!periods_queue.empty()) {
      periods.emplace_back(periods_queue.top());
      if (started > 0) {
        periods_data.emplace_back(Period{started, periods_queue.top()});
        start2index[started] = periods_data.size() - 1;
      }
      points_count.emplace_back(0);
      started = periods_queue.top();
      periods_queue.pop();
    }
    start2index[started] = periods_data.size();
    // points_count.emplace_back(0);
    dep("periods: ");
    for (const auto &d : periods) dep("%d ", d);
    dep("\n");
    dep("periods_data: ");
    for (const auto &d : periods_data) dep("Period(%d, %d) ", d.start, d.end);
    dep("\n");

    chilib::vector<bool> points_same(points_count.size());
    chilib::priority_queue<int, chilib::greater<int>> index_ends;
    int count = 0;
    for (int index = 0; index < points_count.size(); index++) {
      bool changed = false;
      dep("index now: %d\n", index);
      // 遇到新的开始阶段就计数加一
      while (!q.empty() && start2index[q.top().start] == index) {
        const auto &top = q.top();
        index_ends.push(start2index[top.end]);
        dep("end push %d(%d)\n", start2index[top.end], top.end);
        count++;
        changed = true;
        q.pop();
      }
      points_count[index] = count;
      dep("periods_count[%d] = %d\n", index, count);
      // 遇到阶段结束就减一
      while (!index_ends.empty() && index_ends.top() == index) {
        int top = index_ends.top();
        dep("end pop %d\n", top);
        index_ends.pop();
        count--;
        changed = true;
      }
      if (!changed) points_same[index] = true;
    }
    for (int i = 0; i < points_count.size(); i++) {
      dep("points_count[%d] = %d, (%2d - %2d), %s\n", i, points_count[i],
          i < periods_data.size() ? periods_data[i].start : -1,
          i < periods_data.size() ? periods_data[i].end : -1,
          points_same[i] ? "true" : "false");
    }
    chilib::vector<std::pair<int, int>> result;
    int period_max_count = 0;
    for (int i : points_count)
      if (i > period_max_count) period_max_count = i;
    for (int i = 0; i < points_count.size(); i++) {
      if (period_max_count == points_count[i]) {
        int period_index_max_start = i;
        int origin_i = i;
        while (i != points_count.size() - 1 && points_count[i + 1] == period_max_count && points_same[i + 1]) i++;
        if (i != origin_i) {
          int period_index_max_end = ++i;
          result.emplace_back({periods[period_index_max_start - 1], periods[period_index_max_end - 1]});
        } else {
          result.emplace_back({i + 1, i + 1});
        }
      }
    }
    for (int i = 0; i < result.size(); i++)
      printf("%d %d%s", result[i].first, result[i].second,
             i == result.size() - 1 ? "\n" : ", ");
  }
  return 0;
}