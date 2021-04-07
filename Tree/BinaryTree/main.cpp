#include "iterator_base.hpp"
#include "linked_list.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace chilib;

int main() {
  int d = 0;
  linked_list_p<int> head = linked_list<int>::make_head();
  auto p = head->emplace_back(1);
  head->emplace_back(2);
  head->emplace_back(20);
  std::cout << "Insert done." << std::endl;
  for (auto it = head->begin(); it != head->end(); ++it) {
    std::cout << "Trying to output..." << std::endl;
    std::cout << (*it).data << std::endl;
  }
  for (const auto &m : *head) {
    std::cout << "Trying to output..." << std::endl;
    std::cout << m.data << std::endl;
  }
  linked_list<int> h;
  h.emplace_back(10);
  h.emplace_back(20);
  h.emplace_back(30);
  h.emplace_back(40);
  for (const auto &m : h) {
    std::cout << "h: " << m.data << std::endl;
  }
  for (auto &m : h) {
    m.data *= 2;
  }
  for (const auto &m : h) {
    std::cout << "h: " << m.data << std::endl;
  }
//  std::vector<int> v;
//  try {
//    v.at(100) = 100;
//  } catch (std::out_of_range &e) {
//    std::cerr << e.what() << std::endl;
//  }
  std::cout << "length: " << h.length() << std::endl;
  for (size_t i = 0; i < h.length(); i++) {
    std::cout << "h: " << h.at(i) << std::endl;
  }
  return 0;
}

//class M : public iterator_base {
//public:
//  explicit M(int size) : size(size) {
//    this->data = new int[this->size];
//  }
//
//  ~M() {
//    delete[] this->data;
//  }
//
//  template<typename F>
//  void apply(F const &fun) const {
//    for (int i = 0; i < size; i++)
//      if (!fun(i, this->data[i])) return;
//  }
//
//  int *data = nullptr;
//  size_t size = 0;
//
//  class iterator : public iterator_base::iterator_ptr<int> {
//  public:
//    explicit iterator(int &d) : iterator_base::iterator_ptr<int>(d) {}
//
//    // ++it
//    iterator &operator++() override {
//      node++;
//      return *this;
//    };
//
////    // it++
////    iterator operator++(int) {};
//  };
//
//  iterator begin() const {
//    return iterator(*this->data);
//  }
//
//  iterator end() const {
//    return iterator(*(this->data + this->size));
//  }
//};
//
//int main() {
//  M m(40);
//  m.apply([](int index, int &value) -> bool {
//    value = index;
//    return true;
//  });
////  auto b = m.begin(), e = m.end();
////  std::cout << *(b.node) << "\t" << *(*(b.node)) << std::endl;
//  for (auto i : m) {
//    std::cout << typeid(i).name() << "\t" << i << std::endl;
////    std::cout << typeid(*i).name() << "\t" << *i << std::endl;
//  }
//}