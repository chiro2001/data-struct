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
    chilib::vector<int> periods_count;
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
        periods_count.emplace_back(0);
        start2index[started] = periods_data.size() - 1;
      }
      started = periods_queue.top();
      periods_queue.pop();
    }
    printf("periods: ");
    for (const auto &d : periods) printf("%d ", d);
    puts("");
    printf("periods_data: ");
    for (const auto &d : periods_data) printf("Period(%d, %d) ", d.start, d.end);
    puts("");

#if 0
    // 记录已经取出的阶段的结尾，总是取最小值
    chilib::priority_queue<int, chilib::greater<int>> ends;
    int index_end = 0, index_start = 0;
    while (!q.empty()) {
      printf("top StuData{%d, %d}\n", q.top().start, q.top().end);
      PINT(ends.size());
      if (!ends.empty())
        PINT(ends.top());
      auto &top = q.top();
      // 增加计数
      index_start = start2index[top.start];
      periods_count[index_start]++;
      printf("add: ");
      PINT(periods_count[index_start]);
      ends.push(top.end);
      printf("ends pushed %d\n", top.end);
      // 检查是否有比top小的end，有则这个阶段存在则计数减一
      while (!ends.empty() && ends.top() <= top.start) {
        periods_count[index_end]--;
        printf("dec: ");
        PINT(periods_count[index_end]);
        index_end++;
        printf("dec: ");
        PINT(index_end);
        ends.pop();
      }
      q.pop();
    }
#endif
#if 0
    chilib::priority_queue<int, chilib::greater<int>> ends;
    int index_now = 0;
    int index_start;
    while (!q.empty()) {
      printf("top StuData{%d, %d}\n", q.top().start, q.top().end);
      auto &top = q.top();
      index_start = start2index[top.start];
      PINT(index_start);
      // 从index_now到当前得到的index，赋值相同的值
      printf("index_now => index_start: %d => %d\n", index_now, index_start);
      while (index_now < index_start) {
        periods_count[index_now + 1] = periods_count[index_now];
        index_now++;
      }
      periods_count[index_start]++;
      while (!ends.empty() && ends.top() < top.start) {
        auto &ends_top = ends.top();
        int index_end = start2index[ends_top] - 1;
        // if (index_end < 0) {}
        PINT(index_end);
        periods_count[index_end]--;
        printf("dec: ");
        PINT(periods_count[index_end]);
        ends.pop();
      }
      ends.push(top.end);
      q.pop();
    }
    // 从index_start到结尾，赋值相同的值
    printf("index_now => index_start: %d => %d\n", index_now, index_start);
//    return 1;
#endif
    chilib::priority_queue<int, chilib::greater<int>> index_ends;
    int count = 0;
    for (int index = 0; index < periods_count.size(); index++) {
      printf("index now: %d\n", index);
      // 遇到新的开始阶段就计数加一
      while (!q.empty() && start2index[q.top().start] == index) {
        const auto &top = q.top();
        index_ends.push(start2index[top.end]);
        printf("end push %d\n", top.end);
        count++;
        if (index > 0) {
          printf("setting periods_count[%d] = %d\n", index - 1, periods_count[index - 1] + 1);
          periods_count[index - 1]++;
        }
        q.pop();
      }
      periods_count[index] = count;
      printf("periods_count[%d] = %d\n", index, count);
      // 遇到阶段结束就减一
      while (!index_ends.empty() && index_ends.top() == index) {
        const auto &top = index_ends.top();
        printf("end pop %d\n", top);
        index_ends.pop();
        count--;
      }
    }
    int period_index = -1;
    int period_max_count = 0;
    Period period_max;
    for (int i = 0; i < periods_data.size(); i++) {
      PINT(periods_count[i]);
      if (periods_count[i] > period_max_count) {
        period_max_count = periods_count[i], period_index = i, period_max = periods_data[i];
      }
    }
    if (period_index < 0) {
      printf("Error.\n");
      return 1;
    }
    printf("max Period(%d, %d), val = %d\n", period_max.start, period_max.end, period_max_count);
    puts("###############\n");
  }
  return 0;
}