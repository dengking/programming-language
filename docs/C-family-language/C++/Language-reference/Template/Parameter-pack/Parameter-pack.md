# Parameter pack



一个类使用另外一个template class作为data member，如下面的，`Test2`使用模板类`Test`作为成员变量，显然此时需要将`Test`的模板参数传递到`Test2`中，如果使用逐个声明的方式，在`Test2`中声明和`Test`相同的模板参数，则维护起来就比较麻烦，尤其当`Test`的模板参数较多的时候，使用Parameter pack可以避免这种问题。

```c++
#include <iostream>
template<class T1>
class Test
{

};
template<class ...Args>
class Test2
{
public:
Test2(Test<Args...>  t):m_t(t){}
Test<Args...>  m_t;

};

int main()
{

Test<int> t;
Test2<int> t2(t);
std::cout<<&t2<<std::endl;

}
```



## parameter pack and `std::forward`

https://stackoverflow.com/questions/41847828/c-passing-parameter-pack-to-class



## [Limit the number of parameters in a variadic template parameter pack](https://stackoverflow.com/questions/39621158/limit-the-number-of-parameters-in-a-variadic-template-parameter-pack)

如何限制parameter pack中参数的个数



## Parameter pack for class

为class添加parameter pack。

https://en.cppreference.com/w/cpp/language/parameter_pack



## mix parameter pack and single template parameter

```c++
template<typename CHSInsRspQryOrderField,
		typename CHSInsRspQryTradeField,typename ...Args>
class C
{

};


int main
{

C<int, int, int> c;


}

```

需要注意的是：参数包‘Args’必须出现在模板形参表末尾。