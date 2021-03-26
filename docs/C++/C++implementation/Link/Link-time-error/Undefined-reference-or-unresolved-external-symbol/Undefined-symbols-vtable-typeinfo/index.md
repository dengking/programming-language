# Undefined symbols “vtable for …” and “typeinfo for…”

## 我的经历

编译生成了so，通过`ldd -r`进行检查，发现了undefined symbol: 

```C++
undefined symbol: _ZTIN13UstDerivative17CHSAMUSTTraderApiE	(../../../../../lib/libHSAMUSTTraderApi.so)
```

使用`c++filt`进行检查:

```C++
c++filt _ZTIN13UstDerivative17CHSAMUSTTraderApiE
typeinfo for UstDerivative::CHSAMUSTTraderApi
```

于是Google: "c++ undefined symbol typeinfo"，在 stackoverflow [Undefined symbols “vtable for …” and “typeinfo for…”?](https://stackoverflow.com/questions/1693634/undefined-symbols-vtable-for-and-typeinfo-for) # [A](https://stackoverflow.com/a/1693787) 中，找到了问题症结所在: 我没有将abstract base class的virtual function声明为pure virtual method。



## stackoverflow [Undefined symbols “vtable for …” and “typeinfo for…”?](https://stackoverflow.com/questions/1693634/undefined-symbols-vtable-for-and-typeinfo-for) 

### [A](https://stackoverflow.com/a/1693787)

If Obstacle is an abstract base class, then make sure you declare all its virtual methods "pure virtual":

```cpp
virtual void Method() = 0;
```

The `= 0` tells the compiler that this method must be overridden by a derived class, and might not have its own implementation.

If the class contains any non-pure virtual functions, then the compiler will assume that they have an implementation somewhere, and its internal structures (vtable and typeinfo) might be generated in the same object file as one of those; if those functions are not implemented, then the internal structures will be missing and you will get these errors.

## stackoverflow [Undefined reference to `typeinfo for class' and undefined reference to `vtable for class' [duplicate]](https://stackoverflow.com/questions/16460522/undefined-reference-to-typeinfo-for-class-and-undefined-reference-to-vtable-f)