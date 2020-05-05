# Cast away CV

阅读[static_cast conversion](https://en.cppreference.com/w/cpp/language/static_cast)、[dynamic_cast conversion](https://en.cppreference.com/w/cpp/language/dynamic_cast)，可以看到两者都有这样的描述：

> except when such conversions would cast away *constness* or *volatility*.



如果cast away CV，compiler是会报错的：

```c++
static_cast from 'const clone_inherit<concrete, cloneable>' to 'concrete &' casts away qualifiers
```





[reinterpret_cast casts away qualifiers](https://stackoverflow.com/questions/27995692/reinterpret-cast-casts-away-qualifiers)