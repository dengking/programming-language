# final specifier (since C++11)



## cppreference [final specifier (since C++11)](https://en.cppreference.com/w/cpp/language/final)

Specifies that a [virtual function](https://en.cppreference.com/w/cpp/language/virtual) cannot be overridden in a derived class or that a class cannot be [inherited from](https://en.cppreference.com/w/cpp/language/derived_class).

> NOTE:
>
> 终止override或者inheritance
>
> 



## [LeetBook C++ 面试突击](https://leetcode-cn.com/leetbook/detail/cpp-interview-highlights/) # [如何让类不能被继承？](https://leetcode-cn.com/leetbook/read/cpp-interview-highlights/oxbax2/)

解决方法一：借助 `final` 关键字，用该关键字修饰的类不能被继承。

```C++
#include <iostream>

using namespace std;

class Base final
{
};

class Derive: public Base{ // error: cannot derive from 'final' base 'Base' in derived type 'Derive'

};

int main()
{
    Derive ex;
    return 0;
}

```

解决方法二：借助友元、虚继承和私有构造函数来实现

```C++
#include <iostream>
using namespace std;

template <typename T>
class Base{
    friend T;
private:
    Base(){
        cout << "base" << endl;
    }
    ~Base(){}
};

class B:virtual public Base<B>{   //一定注意 必须是虚继承
public:
    B(){
        cout << "B" << endl;
    }
};

class C:public B{
public:
    C(){}     // error: 'Base<T>::Base() [with T = B]' is private within this context
};


int main(){
    B b;  
    return 0;
}


```

说明：在上述代码中 B 类是不能被继承的类。

具体原因：

1、虽然 Base 类构造函数和析构函数被声明为私有 private，在 B 类中，由于 B 是 Base 的友元，因此可以访问 Base 类构造函数，从而正常创建 B 类的对象；

2、B 类继承 Base 类采用虚继承的方式，创建 C 类的对象时，C 类的构造函数要负责 Base 类的构造，但是 Base 类的构造函数私有化了，C 类没有权限访问。因此，无法创建 C 类的对象， B 类是不能被继承的类。

注意：在继承体系中，友元关系不能被继承，虽然 C 类继承了 B 类，B 类是 Base 类的友元，但是 C 类和 Base 类没有友元关系。

