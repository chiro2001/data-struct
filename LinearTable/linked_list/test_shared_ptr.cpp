#include <iostream>
#include <memory>

class B {
public:
  int a;

  explicit B(int _a) : a(_a) {
    std::cout << "Constructor of B(" << this->a << ")" << std::endl;
  }

  ~B() {
    std::cout << "Destructor of B(" << this->a << ")" << std::endl;
  }
};

auto get_B(int a) {
  return std::make_shared<B>(a);
}

int main() {
  std::cout << __cplusplus << std::endl;
  auto pb0 = new B(0);
  const std::type_info &nInfo = typeid(pb0);
  std::cout << nInfo.name() << " | " << nInfo.hash_code() << std::endl;
  std::shared_ptr<B> pb2(new B(1));
  const std::type_info &nInfo2 = typeid(pb2);
  std::cout << nInfo2.name() << " | " << nInfo2.hash_code() << std::endl;
  std::cout << "Calling get_B()" << std::endl;
  auto pb = get_B(2);
  std::cout << pb->a << std::endl;
  std::cout << "Done." << std::endl;
  delete pb0;
  return 0;
}