# Adaptor pattern

1、Adaptor pattern相对比较简单，结合具体例子是非常容易理解的，在"Example"章节中总结了一些例子



## wikipedia [Adapter pattern](https://en.wikipedia.org/wiki/Adapter_pattern)





## Example

基于基础功能扩展出新的，所以adaptor pattern中，一定有一个underlying。

### [The Boost Graph Library (BGL)](https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/)

> custom-made (or even legacy) graph structures can be used as-is with the generic graph algorithms of the BGL, using *external adaptation* (see Section [How to Convert Existing Graphs to the BGL](https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/leda_conversion.html)). External adaptation wraps a new interface around a data-structure without copying and without placing the data inside adaptor objects. The BGL interface was carefully designed to make this adaptation easy.



### C++ [Input/output library](Input/output library)

> These abstract devices allow the same code to handle input/output to files, memory streams, or custom adaptor devices that perform arbitrary operations (e.g. compression) on the fly.

### C++ STL stack

正如在[design patterns used in STL(standard template library)](https://stackoverflow.com/questions/2698474/design-patterns-used-in-stlstandard-template-library)的[回答](https://stackoverflow.com/a/2698775)中所说：

> The STL stack is a container adapter.



### `C++\Pattern\Adaptor-pattern`

其中给出了例子。