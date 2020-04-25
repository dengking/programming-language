# Use macro to define function

对于template class，如果将function definition放到单独的文件，则需要使用如下语法

```c++
template<typename T1, typename T2...>
void C<T1, T2...>::Function()
{

}
```

如果有多个template parameter，则写起来就非常不便利，那么如何来缓解这个问题呢？使用c++ macro来进行实现：

```c++
/**
 * 模板声明的宏
 */
#define TEMPLATE_DECLARATION template <typename T1,	typename T2, typename T3, ...>

/**
 * 模板参数声明
 */
#define TEMPLATE_PARAMETER_DECLARATION T1, T2, T3 ...

/**
 * 定义成员方法的宏
 */
#define DEFINE_METHOD(RETURN, METHOD) \
	TEMPLATE_DECLARATION              \
	RETURN ClassName<TEMPLATE_PARAMETER_DECLARATION>::METHOD
```



下面是一个简单的例子：

```c++
#define DEFINE_METHOD(METHOD) template<typename T>\
C<T>::METHOD \

template<typename T>
class C
{
public:
C();
T m_t;
};
DEFINE_METHOD(C())
{
m_t = 0;
}

int main()
{


}

```

