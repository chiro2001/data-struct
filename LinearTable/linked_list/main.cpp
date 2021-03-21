#include <iostream>
#include <memory>
#include <vector>
#include "linked_list.hpp"

using namespace chilib;

int main() {
  std::vector<int> data = {
          0, 1, 2, 3, 4, 5, 6, 7
  };
  linked_list_p<int> head = linked_list_make<int>(data.at(0));
  auto tail = head;
  for (auto d : data)
    if (d != head->data)
      tail = tail->link(d);

  auto p = head->step(4);

  std::vector<int> data_new = {
          100, 200, 300, 400
  };
  auto head_new = linked_list_make<int>(data_new.at(0));
  auto p_new = head_new;
  for (auto d : data_new)
    if (d != head_new->data)
      p_new = p_new->link(d);

  tail.reset();
  p_new.reset();
  std::cout << "Linking..." << std::endl;
  p->link(head_new);
  p->append(head_new);
  std::cout << "Done." << std::endl;
  std::cout << "Printing..." << std::endl;

  std::cout << "Finding circulation..." << std::endl;
  if (linked_list_check_circulation<int>(head))
    std::cout << "Have circulation!" << std::endl;
  else std::cout << "Have no circulation." << std::endl;

  if (!linked_list_check_circulation<int>(head)) {
    p = head;
    while (p != nullptr) {
      std::cout << p->data;
      if (p->next == nullptr) std::cout << std::endl;
      else std::cout << ", ";
      p = p->next;
    }
  }
  return 0;
}