# stackoverflow [Understanding the warning: binding r-value to l-value reference](https://stackoverflow.com/questions/34240794/understanding-the-warning-binding-r-value-to-l-value-reference)

## [A](https://stackoverflow.com/a/34240886)

You are taking a reference to a **temporary object**. The only legal way to do this is either :

`const object&` (const l-value reference), or

`object&&` (mutable r-value reference)

This is a (deliberate) language limitation.

further discussion:

Assigning a temporary to a reference extends the lifetime of the temporary so that it matches the lifetime of the reference. Therefore, surprisingly to many beginners, this is legal:

```cpp
{
  const string& s = foo();
  cout << s << endl;         // the temporary to which s refers is still alive
}
// but now it's destroyed
```

However, it would normally be a logic error to take a mutable reference to a temporary so this is disallowed in the language:

```cpp
{
  string s& = foo();  // this is not possible
  s += "bar";         // therefore neither is this
  // the implication is that since you modified s, you probably want to
  // preserve it
}
// ... but now it's destroyed and you did nothing with it.
```

here's a more realistic reason why it's probably a logic error, given:

```cpp
string foo();         // function returning a string
void bar(string& s);  // this function is asserting that it intends to *modify*
                      // the string you sent it

// therefore:

bar(foo());           // makes no sense. bar is modifying a string that will be discarded.
                      // therefore assumed to be a logic error
```

you would have to replace the above with:

```cpp
  string s = foo();
  s += "bar";
  // do something here with s
```

Note that there is no overhead whatsoever for capturing the temporary in a named variable (l-value).

r-value references are designed to be the subject of a **move-constructor** or **move-assignment**. Therefore it makes sense that they are mutable. Their very nature implies that the object is transient(短暂的).

thus, this is legal:

```cpp
string&& s = foo();    // extends lifetime as before
s += "bar";
baz(std::move(s));     // move the temporary into the baz function.
```

It might help you to remember that specifying `&&` is you asserting that you *know* that the variable is a mutable temporary.

But the real reason it's allowed is so that this will work:

```cpp
string foo();   // function that returns a string
void bar(string&& s);  // function that takes ownership of s

bar(foo());  // get a string from foo and move it into bar

// or more verbosely:

string s = foo();
bar(move(s));
```

prior to c++11, bar would have to have been written one of these ways:

```cpp
void bar(string s);   // copy a string

// resulting in:

const string& s = foo();
bar(s);  // extra redundant copy made here

void bar(const string& s); // const l-value reference - we *may* copy it
// resulting in:

const string& s = foo();
bar(s);  // maybe an extra redundant copy made here, it's up to bar().
```

> NOTE: 看了上面的介绍，我想到了一个新的问题：`const type &`和 `&&`之间，有哪些异同。

### Example one

```cpp
#include <iostream>
#include <algorithm>
class AClass{
    public:
    AClass(){
        std::cout<<"constructor"<<std::endl;
        member = "hello";
    }
    AClass(const AClass& other){
        std::cout<<"copy constructor"<<std::endl;
        member=other.member;
    }
    AClass(AClass&& other){
        std::cout<<"move constructor"<<std::endl;
        std::swap(member, other.member);
    }
    public:
    std::string member;
};

AClass foo() {
    AClass x;
    return x;
}

int main(){
    std::cout<<"using const reference"<<std::endl;
    const AClass& a = foo();
    std::cout << a.member << std::endl;
    std::cout<<"do not using const reference"<<std::endl;
    AClass b = foo();
    std::cout << b.member << std::endl;
}
```

输出如下:

```
using const reference
constructor
hello
do not using const reference
constructor
hello

```



```c++
#include <iostream>
#include <algorithm>
class AClass{
    public:
    AClass(){
        std::cout<<"constructor"<<std::endl;
        member = "hello";
    }
    AClass(const AClass& other){
        std::cout<<"copy constructor"<<std::endl;
        member=other.member;
    }
    AClass(AClass&& other){
        std::cout<<"move constructor"<<std::endl;
        std::swap(member, other.member);
    }
    public:
    std::string member;
};

AClass foo() {
    AClass x;
    return x;
}

int main(){
    std::cout<<"using const reference"<<std::endl;
    const AClass& a = foo();
    //a.member += " world"; // 会导致编译报错，因为a是const类型
    std::cout << a.member << std::endl;
    std::cout<<"do not using const reference"<<std::endl;
    AClass b = foo();
    b.member += " world";
    std::cout << b.member << std::endl;
    //AClass& c = foo(); //会导致编译报错,报错信息:用类型为‘AClass’的右值初始化类型为‘AClass&’的非常量引用无效
}
```

输出如下：

```C++
using const reference
constructor
hello
do not using const reference
constructor
hello world
```

 编译器的RVO非常厉害。





### Example two



```c++
#include <iostream>
#include <algorithm>
class AClass{
    public:
    AClass(){
        std::cout<<"constructor"<<std::endl;
        member = "hello";
    }
    AClass(const AClass& other){
        std::cout<<"copy constructor"<<std::endl;
        member=other.member;
    }
    AClass(AClass&& other){
        std::cout<<"move constructor"<<std::endl;
        
    }
    public:
    std::string member;
};

AClass foo() {
    AClass x;
    return x;
}
void bar(AClass& s){
    s.member += "world";
    std::cout<<s.member<<std::endl;
}
int main(){
    bar(foo()); 
}
```

上述代码会导致编译报错，错误信息如下：

```
test.cpp: 在函数‘int main()’中:
test.cpp:26:14: 错误：用类型为‘AClass’的右值初始化类型为‘AClass&’的非常量引用无效
     bar(foo()); 
              ^
test.cpp:21:6: 错误：在传递‘void bar(AClass&)’的第 1 个实参时
 void bar(AClass& s){

```

> NOTE:  : 上述代码有如下改法：

- `bar`的签名改成这种方式:`void bar(const AClass& s)`

```c++
#include <iostream>
#include <algorithm>
class AClass{
    public:
    AClass(){
        std::cout<<"constructor"<<std::endl;
        member = "hello";
    }
    AClass(const AClass& other){
        std::cout<<"copy constructor"<<std::endl;
        member=other.member;
    }
    AClass(AClass&& other){
        std::cout<<"move constructor"<<std::endl;
        
    }
    public:
    std::string member;
};

AClass foo() {
    AClass x;
    return x;
}
void bar(AClass& s){
    s.member += "world";
    std::cout<<s.member<<std::endl;
}
int main(){
    AClass&& s = foo();    // extends lifetime as before
    s.member += "bar";
    bar(std::move(s)); 
}
```

编译后会报如下错误:

```
test.cpp: 在函数‘int main()’中:
test.cpp:32:21: 错误：用类型为‘std::remove_reference<AClass&>::type {aka AClass}’的右值初始化类型为‘AClass&’的非常量引用无效
     bar(std::move(s)); 
                     ^
test.cpp:25:6: 错误：在传递‘void bar(AClass&)’的第 1 个实参时
 void bar(AClass& s){

```



将上述代码改写为如下，可以成功：

```c++
#include <iostream>
#include <algorithm>
class AClass{
    public:
    AClass(){
        std::cout<<"constructor"<<std::endl;
        member = "hello";
    }
    AClass(const AClass& other){
        std::cout<<"copy constructor"<<std::endl;
        member=other.member;
    }
    AClass(AClass&& other){
        std::cout<<"move constructor"<<std::endl;
        
    }
    public:
    std::string member;
};

AClass foo() {
    AClass x;
    return x;
}
void bar(AClass&& s){
    s.member += "world";
    std::cout<<s.member<<std::endl;
}
int main(){
    AClass&& s = foo();    // extends lifetime as before
    s.member += "bar";
    bar(std::move(s)); 
}
```



去掉`std::move`

```c++
#include <iostream>
#include <algorithm>
class AClass{
    public:
    AClass(){
        std::cout<<"constructor"<<std::endl;
        member = "hello";
    }
    AClass(const AClass& other){
        std::cout<<"copy constructor"<<std::endl;
        member=other.member;
    }
    AClass(AClass&& other){
        std::cout<<"move constructor"<<std::endl;
        
    }
    public:
    std::string member;
};

AClass foo() {
    AClass x;
    return x;
}
void bar(AClass&& s){
    s.member += "world";
    std::cout<<s.member<<std::endl;
}
int main(){
    AClass&& s = foo();    // extends lifetime as before
    s.member += "bar";
    bar(s); 
}
```

编译后会报这样的错误:

```
test.cpp: 在函数‘int main()’中:
test.cpp:32:10: 错误：无法将左值‘AClass’绑定到‘AClass&&’
     bar(s); 
          ^
test.cpp:25:6: 错误：以初始化‘void bar(AClass&&)’的实参 1
 void bar(AClass&& s){

```

这种类型的错误，Google "cannot bind rvalue reference to lvalue"后，如下回答是非常具有参考价值的：

- [Lvalue to rvalue reference binding](https://stackoverflow.com/questions/20583531/lvalue-to-rvalue-reference-binding)



这种写法也是可行的：

```c++
#include <iostream>
#include <algorithm>
class AClass{
    public:
    AClass(){
        std::cout<<"constructor"<<std::endl;
        member = "hello";
    }
    AClass(const AClass& other){
        std::cout<<"copy constructor"<<std::endl;
        member=other.member;
    }
    AClass(AClass&& other){
        std::cout<<"move constructor"<<std::endl;

    }
    public:
    std::string member;
};

AClass foo() {
    AClass x;
    return x;
}
void bar(AClass&& s){
    s.member += "world";
    std::cout<<s.member<<std::endl;
}
int main(){
    bar(foo());
}
```

在这种写法中，`foo()`的返回值就是rvalue，所以函数`foo`的入参类型`AClass&& `能够bind到它。



> NOTE:  : 总共有两种类型的错误：

- 将rvalue 绑定到lvalue reference
- 将lvalue 绑定到rvalue reference