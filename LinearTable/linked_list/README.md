## 说明

请在c++11以上标准编译，最好c++14或者c++17。

## 编译器情况

CMake编译test_shared_ptr.cpp

**MingW64**

```
201402
Constructor of B(0)
P1B | 16843650438575001256
Constructor of B(1)
St10shared_ptrI1BE | 9907074239572524838
Calling get_B()
Constructor of B(2)
2
Done.
Destructor of B(0)
Destructor of B(2)
Destructor of B(1)
```

**MSVC19**

```
199711
Constructor of B(0)
class B * __ptr64 | 9847873382690236939
Constructor of B(1)
class std::shared_ptr<class B> | 2361398036921738067
Calling get_B()
Constructor of B(2)
2
Done.
Destructor of B(0)
Destructor of B(2)
Destructor of B(1)
```
