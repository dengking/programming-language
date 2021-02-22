# Type Erasure

## Summary

本节对各种type erasure technique进行总结。

### OOP interface + template implementation

1、C++的实现方式是: 

声明一个abstract base class，在其中定义[pure]virtual method(consistent interface)，然后让各个[sub|implementation]class[template]来implement(通过inheritance)这个abstract base class

2、从而实现了consistent type、consistent interface:

consistent interface: behavior

consistent type: type、data

#### 我的实践

对于像UST table这样的class template，我创建了多个specialization，同时我想要创建table name 到 object 的映射关系。显然，实现"table name 到 object 的映射关系"的做好的做法是: 将它们存到一个map中，但是由于各个specialization的type是不一致的，因此无法定义这样的map，那有没有解决方案呢？我的做法是: 声明一个abstract base class，在其中定义[pure]virtual method(consistent interface)，然后让各个class template来implement(通过inheritance)这个abstract base class。

> NOTE: 其实我的上述实现思路，即使用map来进行映射，是属于典型的custom virtual table-generic function pattern，这种实现方式是不推荐的。

### Function pointer for behavior and void pointer for data

本节标题是源自: stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) # [A](https://stackoverflow.com/a/6044720) 的。

Function pointer的典型的代表就是:

1、`std::function`

2、`boost::function`





### `shared_ptr<void>`

这是在  stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) # [A](https://stackoverflow.com/a/6044720)  中提出的。



## C++ standard library support for type erasure



https://en.cppreference.com/w/cpp/utility/any



https://en.cppreference.com/w/cpp/utility/variant



http://en.wikipedia.org/wiki/Type_erasure



`void*`

dynamic_cast `void*`



## TODO wizmann [C++类型擦除与`std::function`性能探索](https://wizmann.tk/cpp-type-erasure-and-std-function.html)



## TODO

1、modernescpp [C++ Core Guidelines: Type Erasure](https://www.modernescpp.com/index.php/c-core-guidelines-type-erasure)

2、quuxplusone [ What is Type Erasure?](https://quuxplusone.github.io/blog/2019/03/18/what-is-type-erasure/)

3、cplusplus [C++ type erasure](http://www.cplusplus.com/articles/oz18T05o/)

4、davekilian [C++ 'Type Erasure' Explained](https://davekilian.com/cpp-type-erasure.html)

5、lucadavidian [Type erasure in C++](http://www.lucadavidian.com/2019/08/26/type-erasure-in-c/)



