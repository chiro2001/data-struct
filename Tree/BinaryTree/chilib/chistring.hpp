// Last modified at 2021-04-07 by Chiro

#ifndef CHILIB_CHISTRING_HPP
#define CHILIB_CHISTRING_HPP

#include <cstring>
#include <sstream>
#include <memory>

namespace chilib {

//typedef char char;

class string {
private:
  char *mdata = nullptr;
  size_t msize = 0;

  // 构造重用部分
  inline void create_from_str(const char *str) {
    if (str == nullptr) {
      // 默认需要加上内存分配，一个\\0
      this->mdata = new char[1];
      this->mdata[0] = '\0';
    } else {
      this->msize = strlen(str);
      this->mdata = new char[this->msize + 1];
      strcpy(this->mdata, str);
    }
  }

public:

  // 默认构造函数
  explicit string(const char *str = nullptr) {
    this->create_from_str(str);
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

  string operator+(string &s) const {
    // 构造新的字符串
    string n(s.length() + this->length());
    memcpy(n.mdata, this->mdata, this->size());
    memcpy(n.mdata + this->msize, s.mdata, s.size());
    return n;
  }
};
}

#endif // CHILIB_CHISTRING_HPP