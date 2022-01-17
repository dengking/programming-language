# SWIG director

一、`D:\Program Files\swigwin-4.0.2\Examples\python\callback`

二、

5.1 Running SWIG

```
-directors - Turn on director mode for all the classes, mainly for testing
-nodirprot - Do not wrap director protected members
```

5.4.9 Pointers to functions and callbacks

> The 'director' feature can be used to make callbacks from C/C++ into the target language, see Callbacks to the target language.

6.29 Callbacks to the target language

> C/C++ function pointers are often used for callbacks and this is discussed in the Pointers to functions and callbacks section. The callback techniques described therein provide a way to control callbacks to a C/C++ function but not callbacks into the target language. The techniques described below show how the director feature can be used to support callbacks from C/C++ to the target language.





三、Google: swig c++ virtual class、swig c++ pure virtual function

1、stackoverflow [Using SWIG to wrap C++ class with virtual methods and overriding them in python](https://stackoverflow.com/questions/6684253/using-swig-to-wrap-c-class-with-virtual-methods-and-overriding-them-in-python)

[A](https://stackoverflow.com/a/6737491/10173843)

> NOTE: 
>
> 其中涉及了:
>
> ```C++
> %typemap(directorin)
> ```



2、github [Python and C++ : Overloading virtual function #897](https://github.com/swig/swig/issues/897)

[A](https://github.com/swig/swig/issues/897#issuecomment-281733842)

I have not yet tried to call Python function from C++ code. However, this seems possible by using a [SWIG director class](http://www.swig.org/Doc3.0/Python.html#Python_nn34).

Looking for [SWIG + Python + director](https://encrypted.google.com/search?hl=en&q=SWIG %2B Python %2B director) on gogole may help you!

For example: http://rjp.io/2013/05/07/swigdirectors-subclassing-from-python/

四、github [Python and C++ : Overloading virtual function #897](https://github.com/swig/swig/issues/897) # [Disambiguate overloaded virtual calls based on function argument count](https://github.com/swig/swig/issues/897#issuecomment-314125854)	

非常好的内容。

五、stackoverflow [Swig directors and smart pointers in Python](https://stackoverflow.com/questions/48206146/swig-directors-and-smart-pointers-in-python)

六、stackoverflow [SWIG JAVA how to wrap C++ multiple inheritance with %interface and pure virtual methods](https://stackoverflow.com/questions/38358017/swig-java-how-to-wrap-c-multiple-inheritance-with-interface-and-pure-virtual)



## test

```C
%{
int binary_op(int a, int b, int (*op)(int, int)) {
return op(a, b);
}
%}
%feature("director") BinaryOp;
%inline %{
struct BinaryOp {
virtual int handle(int a, int b) = 0;
virtual ~BinaryOp() {}
};
%}
%{
static BinaryOp *handler_ptr = NULL;
static int handler_helper(int a, int b) {
// Make the call up to the target language when handler_ptr
// is an instance of a target language director class
return handler_ptr->handle(a, b);
}
// If desired, handler_ptr above could be changed to a thread-local variable in order to make thread-safe
%}
%inline %{
int binary_op_wrapper(int a, int b, BinaryOp *handler) {
handler_ptr = handler;
int result = binary_op(a, b, &handler_helper);
handler = NULL;
return result;
}
%}
```



```Python
class Test(BinaryOp):
    def __init__(self):
        super(Test, self).__init__()

    def handle(self, a, b):
        """

        """
        print(f"a={a}, b={b}")
        return a + b

```



## swig director exception

stackoverflow [Swig exception in Python director class](https://stackoverflow.com/questions/55489154/swig-exception-in-python-director-class)

## implementation

它是如何实现的呢？