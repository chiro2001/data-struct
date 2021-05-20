#include <cstdio>
#include <type_traits>
#include <iostream>
#include <array>

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
constexpr T fib = fib < T, i
- 1 > +
fib<T, i - 2>;

template<>
constexpr uint64_t fib<uint64_t, 1> = 1;


template<>
constexpr uint64_t fib<uint64_t, 0> = 1;


//获取最大的整数
template<size_t arg, size_t... rest>
struct IntegerMax;

template<size_t arg>
struct IntegerMax<arg> : std::integral_constant<size_t, arg> {
};

template<size_t arg1, size_t arg2, size_t... rest>
struct IntegerMax<arg1, arg2, rest...> : std::integral_constant<size_t,
        arg1 >= arg2 ? IntegerMax<arg1, rest...>::value :
        IntegerMax<arg2, rest...>::value> {
};

#include <iostream>
#include <array>

// vector
template<int ...data>
struct mvector;

template<int first, int ...data>
struct mvector<first, data...> {
  static constexpr int size = sizeof...(data) + 1;
  static constexpr int value = first;
  typedef mvector<data...> next_type;
  constexpr static std::array<int, sizeof...(data) + 1> array = {first, data...};
};

template<int first>
struct mvector<first> {
  static constexpr int size = 1;
  static constexpr int value = first;
  typedef mvector<> next_type;
  constexpr static int array[] = {first};
};

template<>
struct mvector<> {
  static constexpr int size = 0;
  static constexpr int value = -1;
  typedef mvector<> next_type;
  constexpr static int array[] = {};
};


// 分割向量
template<int index, typename T, typename S>
struct SplitVector;

template<int index, int ...LeftData, int ...RightData>
struct SplitVector<index, mvector<LeftData...>, mvector<RightData...>> {
  typedef SplitVector<index -
                      1, mvector<LeftData..., mvector<RightData...>::value>, typename mvector<RightData...>::next_type> next_split;
  typedef typename next_split::LeftVector LeftVector;
  typedef typename next_split::RightVector RightVector;
};

template<int ...LeftData, int ...RightData>
struct SplitVector<0, mvector<LeftData...>, mvector<RightData...>> {
  typedef mvector<LeftData...> LeftVector;
  typedef typename mvector<RightData...>::next_type RightVector;
};


// 合并向量
template<typename T, typename S>
struct MergeVector;

template<int ...dataa, int ...datab>
struct MergeVector<mvector<dataa...>, mvector<datab...>> {
  typedef mvector<dataa..., datab...> result_type;
};


// 寻找最大值
template<int now_index, typename U, typename V>
struct FindMax;

template<int now_index, int ...Looped, int ...unLooped>
struct FindMax<now_index, mvector<Looped...>, mvector<unLooped...>> {
  typedef FindMax<now_index +
                  1, mvector<Looped..., mvector<unLooped...>::value>, typename mvector<unLooped...>::next_type> next_max;
  constexpr static int max = mvector<unLooped...>::value > next_max::max ? mvector<unLooped...>::value : next_max::max;
  constexpr static int max_index = mvector<unLooped...>::value > next_max::max ? now_index : next_max::max_index;
};

template<int now_index, int ...Looped>
struct FindMax<now_index, mvector<Looped...>, mvector<>> {
  typedef FindMax<now_index, mvector<Looped...>, mvector<>> next_max;
  constexpr static int max = -1;
  constexpr static int max_index = now_index;
};

//
template<typename T, typename S>
struct SelectSortWork;

template<int ...unSorted, int ...Sorted>
struct SelectSortWork<mvector<unSorted...>, mvector<Sorted...>> {
  typedef FindMax<0, mvector<>, mvector<unSorted...>> max_find_type;
  constexpr static int max = max_find_type::max;
  constexpr static int max_index = max_find_type::max_index;
  typedef SplitVector<max_index, mvector<>, mvector<unSorted...>> split_type;
  typedef SelectSortWork<typename MergeVector<typename split_type::LeftVector, typename split_type::RightVector>::result_type, mvector<max, Sorted...>> next_select_sort_work_type;
  typedef typename next_select_sort_work_type::sorted_type sorted_type;
};

template<int ...Sorted>
struct SelectSortWork<mvector<>, mvector<Sorted...>> {
  typedef mvector<Sorted...> sorted_type;
};

template<int ...data>
struct SelectSort {
  typedef SelectSortWork<mvector<data...>, mvector<>> select_sort_type;
  static constexpr std::array<int, sizeof...(data)> value = select_sort_type::sorted_type::array;
};

#define PINT(x) printf(#x " = %d\n", x)

int main() {
//  auto p1 = foo<2>();
//  auto p2 = foo<10>();
//  PINT(p1);
//  PINT(p2);
//  auto p3 = call_fun<double, true>(10.1);
//  auto p4 = call_fun<double, false>(10.1);
//  cout << p3 << ", " << p4 << endl;
//  auto p5 = fib<uint64_t, 100>;
//  cout << p5 << endl;
  PINT((IntegerMax<2, 5, 1, 7, 3>::value));
  constexpr static std::array<int, 8> result = SelectSort<1, 2, 43, 21, 5324, 22, 9, 0>::value;
  for (int loop_i = 0; loop_i < 8; ++loop_i) {
    PINT(result[loop_i]);
  }

  return 0;
}