

```C++
template<typename ServiceTrait, typename ...Args>
auto GenericPackerFactory(int UserRequestID, Args &&...args) //-> decltype(CGenericPackerFactory<ServiceTrait>::New(std::forward<Args>(args)...))
{
}
```

如果不将 `-> decltype(CGenericPackerFactory<ServiceTrait>::New(std::forward<Args>(args)...))` 即 trailing return type注释掉，则上述code是会报编译错误的，注释掉后，上述code只会报warning。