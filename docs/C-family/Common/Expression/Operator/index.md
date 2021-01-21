# 关于本章

本章对C family language中的operator相关的内容进行介绍。



## pow operator

需要注意的是，C++、C都没有pow operator，只能怪通过调用标准库函数来进行实现。

geeksforgeeks [Power Function in C/C++](https://www.geeksforgeeks.org/power-function-cc/)



```C++
// CPP program to illustrate 
// power function 
#include <bits/stdc++.h> 
using namespace std; 
  
int main() 
{ 
    double x = 6.1, y = 4.8; 
  
    // Storing the answer in result. 
    double result = pow(x, y); 
  
    // printing the result upto 2 
    // decimal place 
    cout << fixed << setprecision(2) << result << endl; 
  
    return 0; 
} 
// g++ --std=c++11 test.cpp
```

