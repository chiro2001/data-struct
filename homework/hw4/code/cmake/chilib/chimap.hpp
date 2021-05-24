//
// Created by Chiro on 2021/5/24.
//

#ifndef CHILIB_CHIMAP_HPP
#define CHILIB_CHIMAP_HPP

#include "tree.hpp"
#include "algo.hpp"

namespace chilib {

class map {
public:
  using T1 = int;
  using T2 = int;
  using Cmp = less<T1>;

  class node {
  public:
    T1 key;
    std::shared_ptr<T2> value;

    bool operator<(const node &n) const {
      return this->key < n.key;
    }
  };

  binary_tree<node> data;
  chilib::string def = "123";

  // T2& find_inside(T1 &key, const shared_ptr<binary_tree<node>> &tree) {
  //
  //   if (tree == nullptr) return nullptr;
  //   if (Cmp()(tree->get_data().key, key)) return find_inside(key, tree->get_left());
  //   else if (Cmp()(key, tree->get_data().key)) return find_inside(key, tree->get_right());
  //   else return tree->get_data().value;
  // }


};

}

#endif //EX5_CHIMAP_HPP
