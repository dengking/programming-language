# QT code generator

## cppcon2015 [Qt: modern user interfaces for C++](https://cppcon2015.sched.com/#)



> QT CODE GENERATORS
>
> - moc
> - uic
> - rcc



> - improve productivity
> - simplify code maintenance
> - overcome C++ limitations



## doc.qt [The Meta-Object System](https://doc.qt.io/qt-6/metaobjects.html)

Qt's meta-object system provides the signals and slots mechanism for inter-object communication, run-time type information, and the dynamic property system.

> NOTE: 
>
> "run-time type information"即"RTTI"

The meta-object system is based on three things:

1、The [QObject](https://doc.qt.io/qt-6/qobject.html) class provides a base class for objects that can take advantage of the meta-object system.

2、The [Q_OBJECT](https://doc.qt.io/qt-6/qobject.html#Q_OBJECT) macro inside the private section of the class declaration is used to enable meta-object features, such as dynamic properties, signals, and slots.

3、The [Meta-Object Compiler](https://doc.qt.io/qt-6/moc.html) (`moc`) supplies each [QObject](https://doc.qt.io/qt-6/qobject.html) subclass with the necessary code to implement meta-object features.

The `moc` tool reads a C++ source file. If it finds one or more class declarations that contain the [Q_OBJECT](https://doc.qt.io/qt-6/qobject.html#Q_OBJECT) macro, it produces another C++ source file which contains the meta-object code for each of those classes. This generated source file is either `#include`'d into the class's source file or, more usually, compiled and linked with the class's implementation.

> NOTE: 
>
> "tag-auxiliary compiler source transformer code generator-QT-C++with macro annotation注解"