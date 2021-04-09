// Last modified at 2021-04-09 by Chiro

#ifndef CHILIB_CHISTRING_HPP
#define CHILIB_CHISTRING_HPP

#include <cstring>
#include <sstream>
#include <memory>
#include "iterator_base.hpp"

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