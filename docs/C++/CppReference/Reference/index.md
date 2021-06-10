# 关于本章

本章描述C++ reference。其中重点描述C++11的新特性：

- rvalue reference
- forward reference

本章，采用的是在`Theory\Programming-language\How-to-master-programming-language`的”What's new in the programming language“章节中描述的方法来学习这两个新特性：

1、rvalue reference用于move semantic；

2、forwarding reference用于perfect forwarding；

这两个特性，为C++语言带来了较大的改变。



本章先描述perfect forwarding，然后描述move semantic，因为在move semantic会涉及perfect forwarding中的内容，然后描述cppreference中，关于reference的描述；



## Reference and value category and const

在stackoverflow [What is move semantics?](https://stackoverflow.com/questions/3106110/what-is-move-semantics)的[回答](https://stackoverflow.com/a/11540204)中的总结：

```c++
            lvalue   const lvalue   rvalue   const rvalue
---------------------------------------------------------              
X&          yes
const X&    yes      yes            yes      yes
X&&                                 yes
const X&&                           yes      yes
```

> In practice, you can forget about `const X&&`. Being restricted to read from **rvalues** is not very useful.

上述表格总结得非常好，基本上涵盖了reference、value category and const的所有可能的组合，下面结合具体的例子来对它进行仔细的说明:

### 1) `const X&`  to lvalue

来源: csdn [【C++数据类型】const 引用的几点用法](https://blog.csdn.net/hyman_c/article/details/52700094)

这个例子中的`ra`就是典型的`const X&`  to lvalue，后续对`ra`的使用需要保持相同的CV，否则会编译报错，下面是一个典型的例子: 

```C++
#include<iostream>
using std::cout;
using std::endl;
void print(int &a)
{
	cout << a << endl;
}
int main()
{
	int a = 10;
	const int &ra = a;
	print(ra); //传入常左值引用
	return 0;
}
// g++ test.cpp

```

上述代码编译报错如下:

```C++
test.cpp: 在函数‘int main()’中:
test.cpp:12:10: 错误：将类型为‘int&’的引用初始化为类型为‘const int’的表达式无效
  print(ra); //传入常左值引用
          ^
test.cpp:4:6: 错误：在传递‘void print(int&)’的第 1 个实参时
 void print(int &a)

```

上述报错提示中的`const int`所指为`ra`，显然上述程序就违背了CV原则，修改版本如下:

```C++
#include<iostream>
using std::cout;
using std::endl;
void print(const int &a)
{
	cout << a << endl;
}
int main()
{
	int a = 10;
	const int &ra = a;
	print(ra); //传入常左值引用
	return 0;
}
// g++ test.cpp

```

### 2) `const X&` to const lvalue

关于此的一个典型的例子就是在const-qualified member function中，所有的member data都是const-qualified的，如果想要reference，必须要使用`const X&` ，在`C++\Language-reference\Classes\Members\Non-static-member\Function-member`中给出了典型的例子。

### 3) `const X&`  to rvalue

下面是一个小例子: 


如果一个函数的说明如下:

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





## Rvalue reference VS forwarding reference

[cppreference std::move#Notes](https://en.cppreference.com/w/cpp/utility/move#Notes):

> One exception is when the type of the function parameter is rvalue reference to type template parameter ("forwarding reference" or "universal reference"), in which case **std::forward** is used instead.

上面这段话给我的感觉是：forwarding reference是一种特殊的rvalue reference。

| reference类型        |                                                              |
| -------------------- | ------------------------------------------------------------ |
| rvalue reference     | [std::move](https://en.cppreference.com/w/cpp/utility/move)  |
| forwarding reference | [std::forward](https://en.cppreference.com/w/cpp/utility/forward) |

## TODO: isocpp faq [References](https://isocpp.org/wiki/faq/references) 

