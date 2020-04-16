# Template and inheritance



## Specialize-template-method-of-base-class

https://stackoverflow.com/questions/39808971/specialize-the-bases-templated-member-function-in-derived-class
## Inherit-from-template-class
https://stackoverflow.com/questions/8810224/inheriting-from-a-template-class-in-c

https://www.cprogramming.com/tutorial/templates.html

https://www.tutorialspoint.com/cplusplus/cpp_templates.htm



## 模板类继承一个非模板类

模板类继承一个非模板类，如下模板类`C`继承非模板类`Base`，在模板类`C`中，有一个`C2`类型的成员变量`m_c2`，这个类型需要一个类型为`Base`的入参，那这就出现了一个问题：如何将模板类`C`的特化后的类对象作为入参传入呢？下面是一个简单的测试出现，结果是可以的，其实显而易见，特化后的模板类是一个基础自`Base`的普通类。

```c++
class Base
{
};

class C2
{
public:
C2(Base* b):m_b(b)
{
}
Base* m_b;

};

template<class T>
class C:public Base
{
using TT = T;
public:
C()
{
m_c2 = new C2(this);
}
C2 *m_c2;
};

int main()
{
C<int> c;
}
```

