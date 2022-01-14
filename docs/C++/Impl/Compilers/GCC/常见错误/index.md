# 常见错误



## `-l` 要放到最后

第一次写:

```
// g++ --std=c++11 -fpic -shared -L. -lexample -I. -I/usr/include/python3.8/ example_wrap.cxx -o _example.so  
```

结果通过 `ldd -r _example.so`  来检查，发现在 `libexample.so` 中定义的一个函数竟然是 undefined symbol，百思不得其解，后来在stackoverflow [C++ shared library undefined reference to `FooClass::SayHello()'](https://stackoverflow.com/questions/12748837/c-shared-library-undefined-reference-to-fooclasssayhello) # [A](https://stackoverflow.com/a/12748882/10173843) 中看到了它的写法是:

```C++
g++ -I. -L. main.cpp -o main -lfoo
```

显然它将依赖的shared library放到了最后，我就尝试修改为:

```C++
g++ --std=c++11 -fpic -shared -L. -I. -I/usr/include/python3.8/ example_wrap.cxx -o _example.so -lexample 
```

此时能够正确的链接上。

