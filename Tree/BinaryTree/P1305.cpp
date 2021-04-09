//
// Created by Chiro on 2021/4/6.
//

#ifndef CHILIB_BTREE_HPP
#define CHILIB_BTREE_HPP

#include <memory>

using std::shared_ptr;

namespace chilib {

class btree {
public:
  using T = char;

  explicit btree(T val) : data(val) {}

  explicit btree(const T *val) : data(*val) {}

  btree() = default;

  static shared_ptr<btree> make(T &d) {
    auto p = new btree(d);
    return std::shared_ptr<btree>(p);
  }

  T data{};
  // 是否线索化
  bool is_in_threading = false;
  // 是否有左右孩子，在线索化的时候使用
  bool tag_left = false, tag_right = false;
  shared_ptr<btree> child_left = nullptr, child_right = nullptr;

  T &get_data() { return data; }

  shared_ptr<btree> &get_left() { return child_left; }

  shared_ptr<btree> &get_right() { return child_right; }

  // 前序遍历 + 中序遍历建树

  // 中序遍历本树，遍历到 callback 返回 false 为止
  template<typename F>
  void traversal_inorder(F const &callback) {
    if (!is_in_threading) {
      if (child_left != nullptr)
        child_left->traversal_inorder(callback);
      if (!callback(*this)) return;
      if (child_right != nullptr)
        child_right->traversal_inorder(callback);
    }
  }

  // 前序遍历本树
  template<typename F>
  void traversal_preorder(F const &callback) {
    if (!is_in_threading) {
      if (!callback(*this)) return;
      if (child_left != nullptr)
        child_left->traversal_preorder(callback);
      if (child_right != nullptr)
        child_right->traversal_preorder(callback);
    }
  }
};

}

#endif //CHILIB_BTREE_HPP

// Last modified at 2021-04-09 by Chiro

#ifndef CHILIB_CHISTRING_HPP
#define CHILIB_CHISTRING_HPP

#include <cstring>
#include <sstream>
#include <memory>
//
// Created by Chiro on 2021/4/6.
//

#ifndef CHILIB_BASE_ITERATOR_HPP
#define CHILIB_BASE_ITERATOR_HPP

#include <memory>

namespace chilib {

// 基础可迭代对象

class iterator_base {
public:
  template<class T>
  class iterator_ptr {
  public:
    using ref = T &;
    using ptr = T *;
    using self = iterator_ptr<T>;
    T *node;

    explicit iterator_ptr(T &d) : node(&d) {}

    ref operator*() { return *node; }

    ptr operator->() { return &(operator*()); }

    // ++it
    virtual self &operator++() = 0;

    bool operator==(const self &s) {
      return node == s.node;
    }

    bool operator!=(const self &s) {
      return node != s.node;
    }
  };

  template<class T>
  class iterator_shared_ptr {
  public:
    using ref = T &;
    using ptr = T *;
    using self = iterator_shared_ptr<T>;
    std::shared_ptr<T> node;

    explicit iterator_shared_ptr(std::shared_ptr<T> &d) : node(d) {}

    ref operator*() { return *node; }

    ptr operator->() { return &(operator*()); }

    // ++it
    virtual self &operator++() = 0;

    bool operator==(const self &s) {
      return node == s.node;
    }

    bool operator!=(const self &s) {
      return node != s.node;
    }
  };
};


}

#endif //CHILIB_BASE_ITERATOR_HPP


namespace chilib {

class string : public iterator_base {
private:
  char *mdata = nullptr;
  size_t msize = 0;
  static const size_t buf_size = 512;

  // 构造重用部分
  inline void create_from_str(const char *str, size_t size = 0) {
    if (size == 0) {
      if (str == nullptr) {
        // 默认需要加上内存分配，一个\\0
        this->mdata = new char[1];
        this->mdata[0] = '\0';
      } else {
        this->mdata = new char[this->msize + 1];
        this->msize = strlen(str);
        strcpy(this->mdata, str);
      }
    } else {
      if (str == nullptr) {
        this->mdata = new char[size];
        this->mdata[0] = '\0';
      } else {
        auto len = strlen(str) > size ? strlen(str) : size;
        this->mdata = new char[len];
        this->msize = len;
      }
    }

  }

public:

  // 默认构造函数
  string(const char *str, size_t size = 0) { // NOLINT(google-explicit-constructor)
    this->create_from_str(str, size);
  }

  string() {
    this->create_from_str(nullptr, 0);
  }

  // 分配内存
  explicit string(size_t s) {
    if (s == 0) {
      this->mdata = new char[1];
      this->mdata[0] = '\0';
    } else {
      this->mdata = new char[s];
      this->msize = s;
    }
  }

  // 拷贝构造函数
  string(const string &t) {
    this->create_from_str(t.mdata);
  }

  // 析构函数
  ~string() { delete[] this->mdata; }

  // 迭代器
  class iterator : public iterator_ptr<char> {
  public:
    explicit iterator(char &d) : iterator_ptr<char>(d) {}

    iterator &operator++() override {
      this->node++;
      return *this;
    }
  };

  iterator begin() { return iterator(*this->mdata); }

  iterator end() { return iterator(*(this->mdata + this->length())); }

  // 字符串赋值
  string &operator=(const string &str) {
#if 0
    if (this == &str) return *this;
    delete[] this->mdata;
    this->create_from_str(str.mdata);
    return *this;
#endif
    if (this != &str) {
      string temp(str);
      // 交换内存管理
      char *t = temp.mdata;
      temp.mdata = this->mdata;
      this->mdata = t;
    }
    return *this;
  }

  // 返回占用空间
  size_t size() const {
    return msize;
  }

  // 求长度
  size_t length() const {
    return this->msize;
  }

  // 返回 C 风格字符串指针
  const char *c_str() const {
    return (const char *) this->mdata;
  }

  // 重载输出
  friend std::ostream &operator<<(std::ostream &out, const string &str) {
    out << str.mdata;
    return out;
  }

  // 重载输入
  friend std::istream &operator>>(std::istream &in, string &s) {
    static char buf[string::buf_size];
    in >> buf;
    s.create_from_str(buf);
    return in;
  }

  string operator+(string &s) const {
    // 构造新的字符串
    string n(s.length() + this->length());
    memcpy(n.mdata, this->mdata, this->size());
    memcpy(n.mdata + this->msize, s.mdata, s.size());
    return n;
  }

  string &operator=(const char *c) {
    this->create_from_str(c);
    return *this;
  }

  void range_check(size_t pos) const {
    if (pos >= this->size()) {
      char buf[512];
      sprintf(buf, "vector::range_check: pos (which is %d) >= this->size() (which is %d)", pos, this->size());
      throw std::out_of_range(buf);
    }
  }

  // 取值
  char &at(size_t pos) {
    this->range_check(pos);
    return this->mdata[pos];
  }

  char &operator[](size_t pos) {
    return this->at(pos);
  }
};
}

#endif // CHILIB_CHISTRING_HPP
#include <iostream>
using namespace chilib;

int main() {
  int n;
  btree b('*');
  std::cin >> n;
  string s;
  while (n--) {
    std::cin >> s;
    if (s.length() != 3) continue;
    auto use = [](btree &bt, string &str) {
      if (str.length() != 3) return;
      bt.get_data() = str[0];
      bt.get_left() = btree::make(str[1]);
      bt.get_right() = btree::make(str[2]);
//      std::cout << "use: " << str[0] << " -> " << str[1] << " + " << str[2] << std::endl;
    };
    if (b.get_data() == '*') {
      use(b, s);
      continue;
    }
    // 不是对根节点操作就先遍历找出节点
    b.traversal_inorder([&s, use](btree &t) -> bool {
//      std::cout << "visit: " << t.get_data() << std::endl;
      if (t.get_data() == s[0]) {
        use(t, s);
        return false;
      }
      return true;
    });
  }
  // 前序遍历
  b.traversal_preorder([](btree &t) -> bool {
    if (t.get_data() != '*')
      std::cout << t.get_data();
    return true;
  });
  std::cout << std::endl;
  return 0;
}