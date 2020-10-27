# ODR

compiler需要从多个层次来保证ODR：

1) 单个translation unit内，是由compiler来保证的

2) 整个target file，是由linker来保证的

在这种情况下，linker的作用对象是**symbol with external linkage**，对于**symbol with internal linkage**，由于它们是**translation local**的，linker是不会对它们进行检查的。

准确的把我了ODR的含义，就能够理解multiple definition的情况了: 在C++、C中，是允许同时存在多个translation中存在着translation local的definition的。

## cppreference [Definitions and ODR (One Definition Rule)#One Definition Rule](https://en.cppreference.com/w/cpp/language/definition)



## wikipedia [One Definition Rule](https://en.wikipedia.org/wiki/One_Definition_Rule)

