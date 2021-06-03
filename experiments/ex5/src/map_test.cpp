//
// Created by Chiro on 2021/5/24.
//

#include <map>
#include <iostream>
#include "chilib/chimap.hpp"

using namespace chilib;

int main() {
  chilib::map<string, string> m;
  m.insert(pair<string, string>{"Yes", "Hahaha"});
  m["Yes"] = "123";
  m["No"] = "456";
  std::cout << m["Yes"] << std::endl;
  std::cout << m["No"] << std::endl;
  // bool a = string("1") < string("2");
  // if (a) puts("yes");
  return 0;
}