#include "iterator_base.hpp"
#include "linked_list.hpp"
#include "chistring.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include "chivector.hpp"

using namespace chilib;

//int main() {
//  int d = 0;
//  linked_list_p<int> head = linked_list<int>::make_head();
//  auto p = head->emplace_back(1);
//  head->emplace_back(2);
//  head->emplace_back(20);
//  std::cout << "Insert done." << std::endl;
//  for (auto it = head->begin(); it != head->end(); ++it) {
//    std::cout << "Trying to output..." << std::endl;
//    std::cout << (*it).data << std::endl;
//  }
//  for (const auto &m : *head) {
//    std::cout << "Trying to output..." << std::endl;
//    std::cout << m.data << std::endl;
//  }
//  linked_list<int> h;
//  h.emplace_back(10);
//  h.emplace_back(20);
//  h.emplace_back(30);
//  h.emplace_back(40);
//  for (const auto &m : h) {
//    std::cout << "h: " << m.data << std::endl;
//  }
//  for (auto &m : h) {
//    m.data *= 2;
//  }
//  for (const auto &m : h) {
//    std::cout << "h: " << m.data << std::endl;
//  }
//  std::cout << "length: " << h.length() << std::endl;
//  for (size_t i = 0; i < h.length(); i++) {
//    std::cout << "h: " << h.at(i) << std::endl;
//  }
//  try {
//    h.at(100) = 1;
//  } catch (std::out_of_range &e) {
//    std::cerr << e.what() << std::endl;
//  }
//  chilib::string s = "This is a string.";
//  for (auto c : s) {
//    std::cout << c << "_";
//  }
//  std::cout << std::endl;
//  return 0;
//}


int main() {
  vector<int> v;
  std::cout << "v.length() = " << v.length() << std::endl;
  v.emplace_back(1);
  v.emplace_back(3);
  v.emplace_back(6);
  for (const auto p : v) {
    std::cout << p << std::endl;
  }
  for (auto &p : v) {
    p *= 2;
  }
  std::cout << "v.length() = " << v.length() << std::endl;
  for (int i = 0; i < v.length(); i++) {
    std::cout << v.at(i) << std::endl;
    v.at(i) *= 3;
  }
  auto it = v.begin();
  while (it != v.end()) {
    *it -= 10;
    ++it;
  }
  for (const auto p : v) {
    std::cout << p << std::endl;
  }
  vector<int> vs(10);
  std::cout << "vs.length() = " << vs.length() << std::endl;
  vector<int> vv = {1, 2, 3};
  std::cout << "vv.length() = " << vv.length() << std::endl;
  vv.emplace_back(4);
  std::cout << "vv.length() = " << vv.length() << std::endl;
  for (const auto &pp : vv) {
    std::cout << pp << std::endl;
  }
  std::cout << "sizeof(vs) = " << sizeof(vs) << std::endl;
  std::cout << "sizeof(vv) = " << sizeof(vv) << std::endl;

  linked_list_p<int> li = linked_list<int>::make({1, 2, 3});
  for (auto &p : *li) {
    std::cout << "li: " << p.get_data() << std::endl;
  }
  return 0;
}