#include <cstdio>
#include <type_traits>
#include <iostream>

using namespace std;

template<int size>
constexpr std::enable_if_t<size < 5, int> foo() {
  return -size;
}

template<int size>
constexpr std::enable_if_t<size >= 5, int> foo() {
  return size;
}

template<typename T, bool return_value>
auto call_fun(T &&val) {
  if constexpr (return_value) return (int) val;
  else return (double) val;
}

//template<int i>
//struct fib {
//  using value = fib<i - 1>::value + fib<i - 2>::value;
//};
template<typename T, T i>
[[maybe_unused]] constexpr T fib = fib <T,  i - 1 > +fib<T, i - 2>;

template<>
constexpr uint64_t fib<uint64_t, 1> = 1;


template<>
constexpr uint64_t fib<uint64_t, 0> = 1;


#define PINT(x) printf(#x " = %d\n", x)

int main() {
  auto p1 = foo<2>();
  auto p2 = foo<10>();
  PINT(p1);
  PINT(p2);
  auto p3 = call_fun<double, true>(10.1);
  auto p4 = call_fun<double, false>(10.1);
  cout << p3 << ", " << p4 << endl;
  auto p5 = fib<uint64_t, 100>;
  cout << p5 << endl;
  return 0;
}