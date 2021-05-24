//
// Created by Chiro on 2021/5/24.
//

#include <map>
#include "chilib/chimap.hpp"

int main() {
  chilib::map m;
  std::map<std::string, chilib::map> mm{
          {"Yes", chilib::map{.def="456"}}
  };
  m = mm["No"];
  // printf("%d\n", *mm["No"].data.get_data().value);
  printf("%s\n", m.def.c_str());
  m = mm["Yes"];
  // printf("%d\n", *mm["No"].data.get_data().value);
  printf("%s\n", m.def.c_str());
  return 0;
}