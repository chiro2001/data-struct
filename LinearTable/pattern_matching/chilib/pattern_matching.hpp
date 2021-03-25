#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <cstddef>

namespace chilib {

auto pattern_matching_raw(const std::string &s, const std::string &t) {
  auto a = s.begin();
  auto p = a;
  auto q = t.begin();
  if (q == t.end()) return 0ll;
  if (s.size() < t.size()) return 0ll;
  while (a != s.end()) {
    p = a;
    q = t.begin();
    while (*p == *q && p != s.end() && q != t.end()) {
      p++;
      q++;
    }
    if (q == t.end()) {
      return a - s.begin();
    }
    a++;
  }
  return s.end() - s.begin();
}

//auto pattern_matching(const std::string &s, const std::string &t) {
//  if (s.empty()) return 0ll;
//  // get next vectors
//  std::vector<long long> next(s.size());
//  next[1] = 0;
//  auto i = s.begin() + 1;
//  auto j = t.begin();
//
//  auto a = s.begin();
//  auto p = a;
//  auto q = t.begin();
//  if (q == t.end()) return 0ll;
//  if (s.size() < t.size()) return 0ll;
//  while (a != s.end()) {
//    p = a;
//    q = t.begin();
//    while (*p == *q && p != s.end() && q != t.end()) {
//      p++;
//      q++;
//    }
//    if (q == t.end()) {
//      return a - s.begin();
//    }
//    a++;
//  }
//  return s.end() - s.begin();
//}

class KMP {
  std::vector<long long[256]> dp;
public:
  std::string pat;

  explicit KMP(std::string t) : pat(std::move(t)) {
    // 构建dp数组
    long long m = this->pat.length();
    this->dp = std::vector<long long[256]>(m);
    for (auto p : this->dp)
      for (int c = 0; c <= 255; c++)
        p[c] = 0;
    // 初始化状态初始值：从第 0 个状态只有读取到 pat[0] 才能转移到下一个状态
    this->dp[0][this->pat.at(0)] = 1;
    // 初始化“影子状态”x，即遇到不匹配的时候的后退位置
    long long x = 0;
    // 当前状态 j 从 1 开始
    for (long long j = 1; j < m; j++) {
      for (int i = 0; i <= 255; i++) {
        // 如果在当前状态能匹配，就转移到下一个状态
        if (this->pat.at(j) == i) this->dp[j][i] = j + 1;
        // 不能匹配就回退到“影子状态”
        else this->dp[j][i] = this->dp[x][i];
      }
    }
    std::cout << "KMP(" << this->pat << ") inited." << std::endl;
  }

  // 搜索文本，找不到返回 -1
  long long search(const std::string &txt) {
    /*
    dp[j][c] = next
    0 <= j < M，代表当前的状态
    0 <= c < 256，代表遇到的字符（ASCII 码）
    0 <= next <= M，代表下一个状态

    dp[4]['A'] = 3 表示：
    当前是状态 4，如果遇到字符 A，
    pat 应该转移到状态 3

    dp[1]['B'] = 2 表示：
    当前是状态 1，如果遇到字符 B，
    pat 应该转移到状态 2
     * */
    long long n = txt.length(), m = this->pat.length();
    // j 表示 pat 当前状态
    long long j = 0;
    for (long long i = 0; i < n - m; i++) {
      j = this->dp[j][txt.at(i)];
      if (j == m) return i - m + 1;
    }
    return -1;
  }
};

}