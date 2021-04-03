#include <cstring>

namespace chilib {

//typedef char char;

class chistring {
private:
  char *mdata = nullptr;
  size_t msize = 0;
public:
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

  // 默认构造函数
  explicit chistring(const char *str = nullptr) {
    this->create_from_str(str);
  }

  // 拷贝构造函数
  chistring(const chistring &t) {
    this->create_from_str(t.mdata);
  }

  // 析构函数
  ~chistring() { delete[] this->mdata; }

  // 字符串赋值
  chistring &operator=(const chistring &str) {
#if 0
    if (this == &str) return *this;
    delete[] this->mdata;
    this->create_from_str(str.mdata);
    return *this;
#endif
    if (this != &str) {
      chistring temp(str);
      // 交换内存管理
      char *t = temp.mdata;
      temp.mdata = this->mdata;
      this->mdata = t;
    }
    return *this;
  }

  // 重载输出
  friend std::ostream &operator<<(std::ostream &out, const chistring &str) {
    out << str.mdata;
    return out;
  }

  // 求长度
  inline size_t length() const {
    return this->msize;
  }

  // 返回 C 风格字符串指针
  inline const char *c_str() const {
    return (const char *) this->mdata;
  }
};
}