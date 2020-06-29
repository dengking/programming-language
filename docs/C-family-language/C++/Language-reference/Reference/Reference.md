# Reference 

## cppreference [Reference declaration](https://en.cppreference.com/w/cpp/language/reference)



## isocpp [References](https://isocpp.org/wiki/faq/references) 



## Thoughts on reference

c++的reference是alias。

- `&`是左值的alias

- `&&`是右值的alias


比如，如果一个函数的说明如下:
```c++
Sub(int ReSubTimes, std::chrono::seconds& ReSubInterval)
```

它的第二个入参的类型是左值引用

下面两种调用方式

```c++
#include<chrono>
void Sub(int ReSubTimes, std::chrono::seconds& ReSubInterval)
{


}

int main(){
std::chrono::seconds t(3);
Sub(3,t);
Sub(3,std::chrono::seconds(3));

}
```
上述`Sub(3,std::chrono::seconds(3));`会导致如下编译报错
```c++
test.cpp: 在函数‘int main()’中:
test.cpp:10:30: 错误：用类型为‘std::chrono::seconds {aka std::chrono::duration<long int>}’的右值初始化类型为‘std::chrono::seconds& {aka std::chrono::duration<long int>&}’的非常量引用无效
 Sub(3,std::chrono::seconds(3)); 
                              ^
test.cpp:2:6: 错误：在传递‘void Sub(int, std::chrono::seconds&)’的第 2 个实参时
 void Sub(int ReSubTimes, std::chrono::seconds& ReSubInterval){

```
这是因为`Sub(3,std::chrono::seconds(3));`的第二个入参是右值，无法通过左值引用进行传参。