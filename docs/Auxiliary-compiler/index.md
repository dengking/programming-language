# Auxiliary compiler



## 两种模式

mode 1、DSL，编写专门compiler DSL的auxiliary compiler来实现code generator 

"tag-auxiliary compiler DSL code generator"

mode 2、source transformer，使用 auxiliary compiler直接编译source code，一般需要在source code中进行特殊的标记，来实现code generator

"tag-auxiliary compiler source transformer code generator"

## 价值

在 cppcon2015 [Qt: modern user interfaces for C++](https://cppcon2015.sched.com/#) 中，进行了很好的总结:

> - improve productivity
> - simplify code maintenance
> - overcome C++ limitations



## Example

一、[Apache Thrift](https://thrift.apache.org/)

使用Auxiliary compiler来编译DSL生成stub code，参见 `Apache-Thrift` 章节；

二、[googletest](https://github.com/google/googletest)

使用Cornichon Gherkin DSL parser stub code(桩代码) for GoogleTest，参见 `Googletest` 章节；

三、doc.qt [The Meta-Object System](https://doc.qt.io/qt-6/metaobjects.html)

使用`moc`来编译source code生成code，参见:

1、 `QT-code-generator` 章节；

2、doc.qt [The Meta-Object System](https://doc.qt.io/qt-6/metaobjects.html)

四、wikipedia [Reflection](https://en.wikipedia.org/wiki/Reflection_(computer_programming))

五、`struct-RTTI`

六、AspectC++

七、Google protobuf

使用Auxiliary compiler来编译DSL生成stub code
