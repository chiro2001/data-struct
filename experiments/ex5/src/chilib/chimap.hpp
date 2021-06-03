//
// Created by Chiro on 2021/5/24.
//

#ifndef CHILIB_CHIMAP_HPP
#define CHILIB_CHIMAP_HPP

#include "tree.hpp"
#include "algo.hpp"
#include <iostream>
#include <utility>

namespace chilib {

template<typename T1, typename T2>
class pair {
public:
  T1 first;
  T2 second;
};

template<typename T1, typename T2>
class map {
public:
  using Cmp = less<T1>;

  class node {
  public:
    T1 key;
    std::shared_ptr<T2> value = nullptr;

    explicit node(T1 &key_, std::shared_ptr<T2> value_ = nullptr) : key(key_), value(std::move(value_)) {}

    node(T1 &&key_, T2 &&value_) : key(key_), value(std::make_shared<T2>(value_)) {}

    node(T1 &key_, T2 &value_) : node(std::forward<T1>(key_), std::forward<T2>(value_)) {}

    explicit node(pair<T1, T2> &p) : node(p.first, p.second) {}

    bool operator<(const node &n) const {
      return this->key < n.key;
    }
  };

  using bitree = binary_tree<node>;

  shared_ptr<bitree> data = nullptr;

  shared_ptr<T2> find_inside(T1 &key, const shared_ptr<bitree> &tree) {
    if (tree == nullptr) return nullptr;
    // printf("find_inside(%d, %6p), %d\n", key, tree.get(), tree->get_data().key);
    // std::cout << "Cmp()(key, tree->get_data().key) = " << Cmp()(key, tree->get_data().key)
    //           << ", Cmp()(tree->get_data().key, key) = " << Cmp()(tree->get_data().key, key) << std::endl;
    if (Cmp()(key, tree->get_data().key)) return find_inside(key, tree->get_left());
    else if (Cmp()(tree->get_data().key, key)) return find_inside(key, tree->get_right());
    else return tree->get_data().value;
  }

  shared_ptr<bitree> find_node(T1 &key, const shared_ptr<bitree> &tree) {
    if (tree == nullptr) return nullptr;
    if (Cmp()(key, tree->get_data().key)) {
      auto res = find_node(key, tree->get_left());
      return res == nullptr ? tree : res;
    } else if (Cmp()(tree->get_data().key, key)) {
      auto res = find_node(key, tree->get_right());
      return res == nullptr ? tree : res;
    } else return tree;
  }

  void insert_inside(T1 &key, T2 &value) {
    auto n = find_node(key, data);
    if (n == nullptr) return;
    // printf("head(%6p), right(%6p), node(%6p)\n", data.get(), data->get_right().get(), n.get());
    if (n->get_data().key == key) {
      n->get_data().value = std::make_shared<T2>(value);
    } else if (Cmp()(key, n->get_data().key)) {
      n->get_left() = bitree::make(node(key, value));
    } else {
      n->get_right() = bitree::make(node(key, value));
    }
  }

  bool has(T1 &key) {
    return has(std::forward<T1>(key));
  }

  bool has(T1 &&key) {
    shared_ptr<T2> res = find_inside(key, data);
    return res != nullptr;
  }

  void insert(pair<T1, T2> &&p) {
    if (data == nullptr) {
      data = bitree::make(node(p));
      return;
    }
    insert_inside(p.first, p.second);
  }

  void insert(pair<T1, T2> &p) {
    insert(std::forward<pair<T1, T2>>(p));
  }

  T2 &operator[](T1 &key) {
    return this->operator[](std::forward<T1>(key));
  }

  T2 &operator[](T1 &&key) {
    shared_ptr<T2> res = find_inside(key, data);
    if (res == nullptr) {
      // std::cerr << "Key not found!" << std::endl;
      // throw std::runtime_error("Key not found!");
      res = std::make_shared<T2>(T2());
      insert(pair<T1, T2>{key, *res});
    }
    return *res;
  }
};

}

#endif //EX5_CHIMAP_HPP
