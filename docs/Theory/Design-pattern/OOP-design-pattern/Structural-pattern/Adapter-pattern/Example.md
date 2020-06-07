# Example

基于基础功能扩展出新的，所以adaptor pattern中，一定有一个underlying。

## [The Boost Graph Library (BGL)](https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/)

> custom-made (or even legacy) graph structures can be used as-is with the generic graph algorithms of the BGL, using *external adaptation* (see Section [How to Convert Existing Graphs to the BGL](https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/leda_conversion.html)). External adaptation wraps a new interface around a data-structure without copying and without placing the data inside adaptor objects. The BGL interface was carefully designed to make this adaptation easy.



## c++ [Input/output library](Input/output library)

> These abstract devices allow the same code to handle input/output to files, memory streams, or custom adaptor devices that perform arbitrary operations (e.g. compression) on the fly.