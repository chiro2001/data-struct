#include <iostream>
#include <memory>
#include <vector>
#include <pattern_matching.hpp>

using namespace chilib;

int main() {
  std::string s = "1234567890", t = "78";
  std::cout << s.substr(pattern_matching_raw(s, t)) << std::endl;
  KMP kmp(t);
  auto search = kmp.search(s);
  std::cout << "get search = " << search << std::endl;
  if (search < 0) {
    std::cout << "Can not find sub string!" << std::endl;
    return 1;
  }
  std::cout << s.substr(search) << std::endl;
  return 0;
}