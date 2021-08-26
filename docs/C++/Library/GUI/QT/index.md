# [QT](https://www.qt.io/)





# Qt Documentation # [Qt 6.1](https://doc.qt.io/qt-6/index.html) 

## [Qt Core](https://doc.qt.io/qt-6/qtcore-index.html)

The Qt Core module adds these features to C++:

> NOTE: 
>
> QT的这些特性是实现一个大型的C++工程所必须的，我们的工程也参考了QT

1、a very powerful mechanism for seamless object communication called signals and slots

> NOTE: 
>
> 参见 wikipedia [Signals and slots](https://en.wikipedia.org/wiki/Signals_and_slots)

2、queryable and designable object properties

> NOTE:
>
> dynamic reflection

3、hierarchical and queryable object trees that organize

> dynamic reflection

4、object ownership in a natural way with guarded pointers ([QPointer](https://doc.qt.io/qt-6/qpointer.html))

> NOTE: 
>
> resource safety、memory safety

5、a dynamic cast that works across library boundaries

The following pages provide more information about Qt's core features:

1、[The Meta-Object System](https://doc.qt.io/qt-6/metaobjects.html)

2、[The Property System](https://doc.qt.io/qt-6/properties.html)

3、[Object Model](https://doc.qt.io/qt-6/object.html)

4、[Object Trees & Ownership](https://doc.qt.io/qt-6/objecttrees.html)

5、[Signals & Slots](https://doc.qt.io/qt-6/signalsandslots.html)

## [The Meta-Object System](https://doc.qt.io/qt-6/metaobjects.html)

Qt's meta-object system provides the signals and slots mechanism for inter-object communication, run-time type information, and the dynamic property system.

The meta-object system is based on three things:

1、The [QObject](https://doc.qt.io/qt-6/qobject.html) class provides a base class for objects that can take advantage of the meta-object system.

2、The [Q_OBJECT](https://doc.qt.io/qt-6/qobject.html#Q_OBJECT) macro inside the private section of the class declaration is used to enable meta-object features, such as dynamic properties, signals, and slots.

3、The [Meta-Object Compiler](https://doc.qt.io/qt-6/moc.html) (`moc`) supplies each [QObject](https://doc.qt.io/qt-6/qobject.html) subclass with the necessary code to implement meta-object features.



## [Object Model](https://doc.qt.io/qt-6/object.html)

> NOTE: 
>
> 对standard C++ object model进行扩展



## [Signals & Slots](https://doc.qt.io/qt-6/signalsandslots.html)

Signals and slots are used for communication between objects. The signals and slots mechanism is a central feature of Qt and probably the part that differs most from the features provided by other frameworks. Signals and slots are made possible by Qt's [meta-object system](https://doc.qt.io/qt-6/metaobjects.html).



### Introduction

In GUI programming, when we change one widget, we often want another widget to be notified. More generally, we want objects of any kind to be able to communicate with one another. For example, if a user clicks a **Close** button, we probably want the window's [close()](https://doc.qt.io/qt-6/qwidget.html#close) function to be called.

> NOTE: 
>
> inter-object communication

Other toolkits achieve this kind of communication using **callbacks**. A callback is a pointer to a function, so if you want a **processing function** to notify you about some event,  you pass a pointer to another function (the callback) to the **processing function**. The **processing function** then calls the callback when appropriate. While successful frameworks using this method do exist, callbacks can be unintuitive(违反直觉的) and may suffer from problems in ensuring the type-correctness of callback arguments.



### Signals and Slots

In Qt, we have an alternative to the callback technique: We use signals and slots. A **signal** is emitted when a particular event occurs. Qt's widgets have many predefined signals, but we can always subclass widgets to add our own signals to them. A **slot** is a function that is called in response to a particular signal. Qt's widgets have many pre-defined slots, but it is common practice to subclass widgets and add your own slots so that you can handle the signals that you are interested in.

![img](https://doc.qt.io/qt-6/images/abstract-connections.png)



The signals and slots mechanism is type safe: The signature of a signal must match the signature of the receiving slot. (In fact a slot may have a shorter signature than the signal it receives because it can ignore extra arguments.) Since the signatures are compatible, the compiler can help us detect type mismatches when using the function pointer-based syntax. The string-based SIGNAL and SLOT syntax will detect type mismatches at runtime. Signals and slots are loosely coupled: A class which emits a signal neither knows nor cares which slots receive the signal. Qt's signals and slots mechanism ensures that if you connect a signal to a slot, the slot will be called with the signal's parameters at the right time. Signals and slots can take any number of arguments of any type. They are completely type safe.

# QT source code



## GitHub [QT](https://github.com/qt)



## [Offline Qt Downloads](https://www.qt.io/offline-installers)



## [Building Qt 5 from Git # Getting_the_source_code](https://wiki.qt.io/Building_Qt_5_from_Git#Getting_the_source_code)

