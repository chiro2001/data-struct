#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <sstream>

#include "linked_list.hpp"

using namespace chilib;
using std::ios;
using std::string;

class StudentLinkedListNode {
public:
  string stuId;
  int grade{};

  StudentLinkedListNode(string stuId_, int grade_) : stuId(std::move(stuId_)), grade(grade_) {};

  // 重载一下 << 让这个结构能直接输出
  friend std::ostream &operator<<(std::ostream &out, const StudentLinkedListNode &stu) {
    // 格式：{ID:student10, Grade:100}
    out << "{ID:" << stu.stuId << ", Grade:" << stu.grade << "}";
    return out;
  };
};

// 指针节点简写为 StudentList
using StudentList = linked_list_p<StudentLinkedListNode>;
using CrossLink = std::pair<int, int>;
// 异常
namespace ex {
class Base : public std::exception {
public:
  string message;

  Base() = default;

  explicit Base(string message_) : message(std::move(message_)) {}

  const char *what() {
    std::cerr << "Error: " << typeid(this).name() << std::endl;
    if (message.length()) std::cerr << " with message: " << this->message << std::endl;
    return typeid(this).name();
  }
};

class ReadFileData : public Base {
public:
  ReadFileData() = default;

  explicit ReadFileData(string message_) : Base(std::move(message_)) {}
};
}

// @prog    : 使用头插法读取文件数据到一个链表
// @rets    : 通过引用返回两个链表头指针和交叉节点，直接返回读取文件的offset
// @notice  : 读取完需要的数据或者EOF就表示文件结束
long readFileData(const string &filename, StudentList &class1, StudentList &class2, CrossLink &cross,
                  size_t offset = 0) {
  int length1 = 0, length2 = 0;
  // 尝试打开文件
  std::ifstream fileData(filename);
  if (!fileData) throw ex::ReadFileData();
  fileData.seekg(offset, std::ios::beg);
  // 读取数据
  string line;
  static std::stringstream ss;
  class1 = class2 = nullptr;
  // 读取第一行
  std::getline(fileData, line);
  ss.clear();
  if (line.length() == 0) return -1;
  ss << line;
  ss >> length1 >> length2;
  // 解析一行数据
  auto parseLine = [](const string &line) -> StudentList {
//    std::cout << line.length() << std::endl;
    // 读取到空行
    if (line.length() == 0)
      return nullptr;
    // 用空格分隔
    size_t pos = line.find(' ');
    if (!(0 < pos && pos < line.length())) throw ex::ReadFileData();
    // 用 stringstream 转到 int
    string stuId = line.substr(0, pos);
    ss.clear();
    ss << line.substr(pos + 1, line.length());
    int grade = 0;
    ss >> grade;
    StudentLinkedListNode data(stuId, grade);
    return linked_list_make<StudentLinkedListNode>(data);
  };
  // 读取数据到classx
  auto readClassData = [parseLine](std::ifstream &file, StudentList &head, int length) {
    string line;
    for (int i = 0; i < length; i++) {
      std::getline(file, line);
      auto p = parseLine(line);
      if (head == nullptr) head = p;
      else {
        p->link(head);
        head = p;
      }
    }
  };
  readClassData(fileData, class1, length1);
  readClassData(fileData, class2, length2);
  // 读取最后一行
  ss.clear();
  std::getline(fileData, line);
  if (line.length() == 0) return -1;
  ss << line;
  ss >> cross.first >> cross.second;
  auto size = fileData.tellg();
  fileData.close();
  return size;
}

// @prog    : 创建交叉链表
// @rets    : 通过参数引用修改 head1, head2
// @notice  : 相交方法是将b中的前一结点指向a中的首个相交结点
//            使用了智能指针使得未被引用的数据自动删除
void createCrossLink(StudentList &head1, StudentList &head2, CrossLink &cross) {
//  auto p1 = head1->step(cross.first - 2), p2 = head2->step(cross.second - 2);
  auto p1 = head1, p2 = head2;
  int a = cross.first - 1, b = cross.second - 1;
  while (a > 0 && p1 != nullptr) {
    p1 = p1->next;
    a--;
  }
  while (b > 0 && p2 != nullptr) {
    p2 = p2->next;
    b--;
  }
  // shared_ptr不能指向自己，特殊处理0
  if (cross.first == 0) {
    p2->next = p1;
//    std::cout << "###### Create node at " << p1->data << std::endl;
    return;
  }
  if (p1 == nullptr || p2 == nullptr || p1->next == nullptr || p2->next == nullptr) return;
//  std::cout << "###### Create node at " << p1->next->data << std::endl;
  p2->next = p1->next;
  // 在这之后未被引用的数据自动删除
}

// @prog    : 找到相交的第一个结点
// @rets    : 返回找到的交点
StudentList findCrossBeginNode(StudentList &head1, StudentList &head2) {
  auto p1 = head1, p2 = head2;
  while (p1 != nullptr) {
    p2 = head2;
    while (p2 != nullptr) {
      if (p1 == p2) {
        return p1;
      }
      p2 = p2->next;
    }
    p1 = p1->next;
  }
  return nullptr;
}

// @prog    : 输出该表的成绩情况
void outputStudentLinkedList(StudentList &head) {
  auto p = head;
  while (p != nullptr) {
    std::cout << p->data << (p->next == nullptr ? "\n" : "->");
    p = p->next;
  }
}

// @prog    : 翻转链表
// @rets    : 通过引用修改链表
void reverseLinkedList(StudentList &head) {
  // 再反插一次吧...之前的方法会出现循环引用
  // 由于会自动释放内存，所以内存占用还是一样的
  auto newHead = linked_list_make(head->data);
  head = head->next;
  while (head != nullptr) {
    auto newP = linked_list_make(head->data);
    newP->link(newHead);
    newHead = newP;
    head = head->next;
  }
  head = newHead;
}

// @prog    : 主函数
int main() {
  string filename = "./gradeImport.in";
  StudentList class1 = nullptr, class2 = nullptr;
  CrossLink cross;
  long offset = 0;
  while ((offset = readFileData(filename, class1, class2, cross, offset)) > 0) {
    // 调试信息
//    puts("class 1:");
//    linked_list_print(class1);
//    puts("class 2:");
//    linked_list_print(class2);
//    std::cout << "offset: " << offset << ", cross: <" << cross.first << ", " << cross.second << ">" << std::endl;

    std::cout << "* part1:" << std::endl << "class1:" << std::endl;
    outputStudentLinkedList(class1);
    std::cout << "class2:" << std::endl;
    outputStudentLinkedList(class2);
    reverseLinkedList(class1);
    reverseLinkedList(class2);
    std::cout << "* part2:" << std::endl << "class1:" << std::endl;
    outputStudentLinkedList(class1);
    std::cout << "class2:" << std::endl;
    outputStudentLinkedList(class1);
    createCrossLink(class1, class2, cross);
    auto node = findCrossBeginNode(class1, class2);
    // 有可能没有交点
    if (node != nullptr)
      std::cout << "* part3:" << std::endl << node->data << std::endl;
    offset++;
    // 销毁数据
    // 空间因为智能指针的计数被降到0而自动释放
    class1 = class2 = nullptr;
  }
  return 0;
}