#include <iostream>
#include <memory>
//#include <fstream>

#include "chilib/linked_list.hpp"
#include "chilib/chistring.hpp"

// 调试输出
#define PINT(x) printf("Line %d:\t"#x" = %d\n", __LINE__, x)
#define PCHAR(x) printf("%c", x)

using namespace chilib;
using std::ios;
// 声明：本程序没有使用 std::string
// NOT using std::string;

class StudentLinkedListNode {
public:
  chilib::string stuId;
  int grade{};

  StudentLinkedListNode() = default;

  StudentLinkedListNode(const chilib::string &stuId_, int grade_) : stuId(stuId_), grade(grade_) {};

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
  chilib::string message;

  Base() = default;

  explicit Base(const chilib::string &message_) : message(message_) {}

  const char *what() {
    std::cerr << "Error: " << typeid(this).name() << std::endl;
    if (message.length()) std::cerr << " with message: " << this->message << std::endl;
    return typeid(this).name();
  }
};

class ReadFileData : public Base {
public:
  ReadFileData() = default;

  explicit ReadFileData(const chilib::string &message_) : Base(message_) {}
};
}

size_t getFileLine(FILE *fp, char *dst, size_t offset) {
  if (!fp) throw ex::ReadFileData();
  fseek(fp, offset, SEEK_SET);
  char *p = dst;
  size_t retry = 2;
  int c;
  while (retry > 0 && (p == dst || *dst == '\n')) {
    while ((c = fgetc(fp)) != '\n' && c > 0) *(p++) = (char) c;
    *p = '\0';
    retry--;
  }
//  puts(dst);
  offset += (long) (p - dst) + 1;
  fseek(fp, offset, SEEK_SET);
//  PINT(ftell(fp));
//  PINT(offset);
  return offset;
}

// @prog    : 使用头插法读取文件数据到一个链表
// @rets    : 通过引用返回两个链表头指针和交叉节点，直接返回读取文件的offset
// @notice  : 读取完需要的数据或者EOF就表示文件结束
long readFileData(const chilib::string &filename, StudentList &class1, StudentList &class2, CrossLink &cross,
                  size_t offset = 0) {
#ifdef USE_STREAM
  int length1 = 0, length2 = 0;
  // 尝试打开文件
  std::ifstream fileData(filename.c_str());
  if (!fileData) throw ex::ReadFileData();
  fileData.seekg(offset, std::ios::beg);
  // 读取数据
  chilib::string line;
  static std::stringstream ss;
  class1 = class2 = nullptr;
  // 读取第一行
  std::getline(fileData, line);
  ss.clear();
  if (line.length() == 0) return -1;
  ss << line;
  ss >> length1 >> length2;
  // 解析一行数据
  auto parseLine = [](const chilib::string &line) -> StudentList {
//    std::cout << line.length() << std::endl;
    // 读取到空行
    if (line.length() == 0)
      return nullptr;
    // 用空格分隔
    size_t pos = line.find(' ');
    if (!(0 < pos && pos < line.length())) throw ex::ReadFileData();
    // 用 stringstream 转到 int
    chilib::string stuId = line.substr(0, pos);
    ss.clear();
    ss << line.substr(pos + 1, line.length());
    int grade = 0;
    ss >> grade;
    StudentLinkedListNode data(stuId, grade);
    return linked_list_make<StudentLinkedListNode>(data);
  };
  // 读取数据到classx
  auto readClassData = [parseLine](std::ifstream &file, StudentList &head, int length) {
    chilib::string line;
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
#endif
  int length1 = 0, length2 = 0;
  FILE *fp = fopen(filename.c_str(), "r");
  if (!fp) throw ex::ReadFileData();
  fseek(fp, offset, SEEK_SET);
//  PINT(offset);
  char buf[128];
  // 因为fscanf貌似有点问题，会让文件指针直接指向负值，所以使用了自己的读函数
  offset = getFileLine(fp, buf, offset);
  if (sscanf(buf, "%d%d", &length1, &length2) <= 0) return -1;
  class1 = class2 = nullptr;
  class1 = linked_list<StudentLinkedListNode>::make_head();
  class2 = linked_list<StudentLinkedListNode>::make_head();
  auto readClassData = [&offset](StudentList &head, int length, FILE *fp) {
    char buf[512], stuId[128];
    int grade;
    for (int i = 0; i < length; i++) {
      offset = getFileLine(fp, buf, offset);
      sscanf(buf, "%s%d", stuId, &grade);
      chilib::string stuIdString = chilib::string(stuId);
      auto node = StudentLinkedListNode(stuIdString, grade);
      head->insert(node);
    }
  };
  readClassData(class1, length1, fp);
  readClassData(class2, length2, fp);
  // 读取最后一行
  offset = getFileLine(fp, buf, offset);
  sscanf(buf, "%d%d", &cross.first, &cross.second);
  fclose(fp);
  return offset;
}

// @prog    : 创建交叉链表
// @rets    : 通过参数引用修改 head1, head2
// @notice  : 相交方法是将b中的前一结点指向a中的首个相交结点
//            使用了智能指针使得未被引用的数据自动删除
void createCrossLink(StudentList &head1, StudentList &head2, CrossLink &cross) {
  // shared_ptr不能指向自己，特殊处理0
  if (cross.first == 0) {
    auto pp2 = head2->step(cross.second - 1);
    pp2->link(head1->get_next());
//    std::cout << "###### Create node at " << head1->get_next()->get_data() << std::endl;
    return;
  }
  auto p1 = head1->step(cross.first - 1), p2 = head2->step(cross.second - 1);
  if (p1 == nullptr || p2 == nullptr || p1->get_next() == nullptr || p2->get_next() == nullptr) return;
//  std::cout << "###### Create node at " << p1->get_next()->get_data() << std::endl;
  p2->link(p1->get_next());
  // 在这之后未被引用的数据自动删除
}

// @prog    : 找到相交的第一个结点
// @rets    : 返回找到的交点
// @note    : 输入的链表不能有环
StudentList findCrossBeginNode(StudentList &head1, StudentList &head2) {
  // O(n^2)
//  auto p1 = head1->get_next(), p2 = head2->get_next();
//  while (p1 != nullptr) {
//    p2 = head2->get_next();
//    while (p2 != nullptr) {
//      if (p1 == p2) {
//        return p1;
//      }
//      p2 = p2->get_next();
//    }
//    p1 = p1->get_next();
//  }
//  return nullptr;
  // 新算法：O(n)
  size_t len1 = head1->length(), len2 = head2->length();
  auto p1 = head1, p2 = head2;
  if (len1 == 0 || len2 == 0) return nullptr;
  // 调整到短的那一个
  if (len1 < len2) {
    p2 = p2->step(len2 - len1 - 1);
  } else if (len1 > len2) {
    p1 = p1->step(len1 - len2 - 1);
  }
  while (p1 != p2) {
    if (p1 == nullptr || p2 == nullptr) return nullptr;
    p1 = p1->get_next();
    p2 = p2->get_next();
  }
  return p1;
}

// @prog    : 输出该表的成绩情况
void outputStudentLinkedList(StudentList &head) {
  for (auto d : *head) {
    std::cout << d.get_data() << ((d.get_next() == nullptr) ? "\n" : "->");
  }
}

// @prog    : 翻转链表
// @rets    : 通过引用修改链表
void reverseLinkedList(StudentList &head) {
  // 再反插一次吧...之前的方法会出现循环引用
  // 由于会自动释放内存，所以内存占用还是一样的
  auto newHead = linked_list<StudentLinkedListNode>::make_head();
  if (head->get_next() == nullptr) {
    return;
  }
  head = head->get_next();
  while (head != nullptr) {
    newHead->insert(head->get_data());
    head = head->get_next();
  }
  head = newHead;
}

// @prog    : 主函数
int main() {
  chilib::string filename("./gradeImport.in");
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
    outputStudentLinkedList(class2);
    createCrossLink(class1, class2, cross);
    auto node = findCrossBeginNode(class1, class2);
    // 有可能没有交点
    if (node != nullptr)
      std::cout << "* part3:" << std::endl << node->get_data() << std::endl;
    offset++;
    // 销毁数据
    // 空间因为智能指针的计数被降到0而自动释放
    class1 = class2 = nullptr;
    std::cout << std::endl;
  }
//  PINT(offset);
  return 0;
}
