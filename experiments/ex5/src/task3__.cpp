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

int main() {
  int n, m;
  // 读取数据
  freopen("5_3_input.in", "r", stdin);
  while (scanf("%d%d", &n, &m) > 0) {
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
    printf("origin data: ");
    while (!tmp.empty()) {
      auto top = tmp.top();
      printf("Period{%d, %d} ", top.start, top.end);
      tmp.pop();
    }
    puts("");
    int started = -1;
    while (!periods_queue.empty()) {
      periods.emplace_back(periods_queue.top());
      if (started > 0) {
        periods_data.emplace_back(Period{started, periods_queue.top()});
        points_count.emplace_back(0);
        start2index[started] = periods_data.size() - 1;
      }
      started = periods_queue.top();
      periods_queue.pop();
    }
    start2index[started] = periods_data.size();
    points_count.emplace_back(0);
    printf("periods: ");
    for (const auto &d : periods) printf("%d ", d);
    puts("");
    printf("periods_data: ");
    for (const auto &d : periods_data) printf("Period(%d, %d) ", d.start, d.end);
    puts("");

    chilib::vector<bool> points_same(points_count.size());
    chilib::priority_queue<int, chilib::greater<int>> index_ends;
    int count = 0;
    for (int index = 0; index < points_count.size(); index++) {
      bool changed = false;
      printf("index now: %d\n", index);
      // 遇到新的开始阶段就计数加一
      while (!q.empty() && start2index[q.top().start] == index) {
        const auto &top = q.top();
        index_ends.push(start2index[top.end]);
        printf("end push %d(%d)\n", start2index[top.end], top.end);
        count++;
        changed = true;
        q.pop();
      }
      points_count[index] = count;
      printf("periods_count[%d] = %d\n", index, count);
      // 遇到阶段结束就减一
      while (!index_ends.empty() && index_ends.top() == index) {
        int top = index_ends.top();
        printf("end pop %d\n", top);
        index_ends.pop();
        count--;
        changed = true;
      }
      if (!changed) points_same[index] = true;
    }
    int period_index = -1;
    int period_max_count = 0;
    chilib::priority_queue<std::pair<Period, int>> period_max;
    for (int i = 0; i < points_count.size(); i++) {
      printf("points_count[%d] = %d, (%2d - %2d), %s\n", i, points_count[i],
             i < periods_data.size() ? periods_data[i].start : -1,
             i < periods_data.size() ? periods_data[i].end : -1,
             points_same[i] ? "true" : "false");
      if (points_count[i] >= period_max_count) {
        period_max_count = points_count[i], period_index = i;
        if (i < periods_data.size()) period_max.push({periods_data[i], period_max_count});
      }
    }
    if (period_index < 0) {
      printf("Error.\n");
      return 1;
    }
    chilib::priority_queue<Period, chilib::greater<Period>> result;
    while (!period_max.empty() && period_max.top().second == period_max_count) {
      printf("max Period(%d, %d), val = %d\n", period_max.top().first.start, period_max.top().first.end,
             period_max.top().second);
      result.push(period_max.top().first);
      period_max.pop();
    }
    while (!result.empty()) {
      printf("%d %d%s", result.top().start, result.top().end - 1, result.size() == 1 ? "\n" : ", ");
      result.pop();
    }
    puts("###############\n");
  }
  return 0;
}