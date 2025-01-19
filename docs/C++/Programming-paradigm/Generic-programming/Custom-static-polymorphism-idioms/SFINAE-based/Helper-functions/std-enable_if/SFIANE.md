# 引言


## SFINAE
SFINAE 是C++ 的一种语言属性，具体内容就是”从一组重载函数中删除模板实例化无效的函数”。
> Prune functions that do not yield valid template instantiations from a set of overload functions.

SFINAE 的的全称是 Substitution Failure Is Not An Error。

SFINAE 应用最为广泛的场景是C++中的 std::enable_if，这里有完整的[英文描述](https://en.wikibooks.org/wiki/More_C++_Idioms/SFINAE)：
> In the process of template argument deduction, a C++ compiler attempts to instantiate signatures of a number of candidate overloaded functions to make sure that exactly one overloaded function is available as a perfect match for a given function call.

从上面的描述中我们可以看到，在对一个函数调用进行模板推导时，编译器会尝试推导所有的候选函数（重载函数，模板，但是普通函数的优先级要高），以确保得到一个最完美的匹配。

> If an invalid argument or return type is formed during the instantiation of a function template, the instantiation is removed from the overload resolution set instead of causing a compilation error. 
As long as there is one and only one function to which the call can be dispatched, the compiler issues no errors.

也就是说在推导的过程中，如果出现了无效的模板参数，则会将该候选函数从重载决议集合中删除，只要最终得到了一个 perfect match ，编如下代码所示：译就不会报错。(这段话非常重要)

如下代码所示：

```
long multiply(int i, int j) { return i * j; }

template <class T>
typename T::multiplication_result multiply(T t1, T t2)
{
    return t1 * t2;
}

int main(void)
{
    multiply(4, 5);
}
```

main 函数调用 multiply 会使编译器会尽可能去匹配所有候选函数，虽然第一个 multiply 函数明显是较优的匹配，但是为了得到一个最精确的匹配，编译器依然会尝试去匹配剩下的候选函数，此时就会去推导 第二个multiply函数，中间在参数推导的过程中出现了一个无效的类型 int::multiplication_result ，但是因为 SFINAE 原则并不会报错。



