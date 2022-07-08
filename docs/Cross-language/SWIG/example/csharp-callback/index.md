# C# callback

一、从C++OOP到C accessor function:

1、accessor function的入参类型是void pointer



## swg文件

5.1.1 Input format

> More often than not, this is a special SWIG interface file which is usually denoted with a special `.i` or `.swg` suffix. In certain cases,

5.4.7.3 Limiting global renaming rules

> Many other checks are possible and this documentation is not exhaustive, see the "%rename predicates" section in `swig.swg` for the full
> list of supported match expressions.

13.3.3 Default typemap matching rules

> The best way to explore the default typemaps is to look at the ones already defined for a particular language module. Typemap definitions are usually found in the SWIG library in a file such as java.swg , csharp.swg etc. However, for many of the target languages the typemaps are hidden behind complicated macros, so the best way to view the default typemaps, or any typemaps for that matter, is to look at the preprocessed output by running swig -E on any interface file. Finally the best way to view the typemap matching rules in action is via the debugging typemap pattern matching options covered later on.



## director的实现

```C++
class SwigDirector_Callback : public Callback, public Swig::Director {

public:
    SwigDirector_Callback();
    virtual ~SwigDirector_Callback();
    virtual void run();

    typedef void (SWIGSTDCALL* SWIG_Callback0_t)();
    void swig_connect_director(SWIG_Callback0_t callbackrun);

private:
    SWIG_Callback0_t swig_callbackrun;
    void swig_init_callbacks();
};
```

其中 base class `Callback` 是由有如下指定的:

```C++
/* turn on director wrapping Callback */
%feature("director") Callback;
```



它的run方法override如下:

```C++
void SwigDirector_Callback::run() {
  if (!swig_callbackrun) {
    Callback::run();
    return;
  } else {
    swig_callbackrun();
  }
}
```



```C++
SWIGEXPORT void SWIGSTDCALL CSharp_Callback_director_connect(void *objarg, SwigDirector_Callback::SWIG_Callback0_t callback0) {
  Callback *obj = (Callback *)objarg;
  SwigDirector_Callback *director = static_cast<SwigDirector_Callback *>(obj);
  director->swig_connect_director(callback0);
}
```

应该是由C#调用的。



总结：从上述内容可以看出，通过从C#端传入一个pointer to function来实现跨语言多态。



