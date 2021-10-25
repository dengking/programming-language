# Dart OOP



## Destructor

dart是不支持destructor的，参见:

1、stackoverflow [How to do something before object destruction in Dart?](https://stackoverflow.com/questions/31645751/how-to-do-something-before-object-destruction-in-dart) # [A](https://stackoverflow.com/a/31645761/10173843)

> This is not supported. There is nothing like a destructor in Dart.
> JS garbage collector doesn't provide a way to implement this. 

2、stackoverflow [How do we delete an object in Dart?](https://stackoverflow.com/questions/20482596/how-do-we-delete-an-object-in-dart)

3、github [Allow cleanup of an object through a destructor or dispose method #3691](https://github.com/dart-lang/sdk/issues/3691)



## extension 

medium [Dart Extension Methods Fundamentals](https://medium.com/dartlang/extension-methods-2d466cd8b308)

mobikul [Flutter: Use of Dart extension Methods.](https://mobikul.com/flutter-use-of-dart-extension-methods/)



## getter、setter

w3adda [Dart Getters and Setters](https://www.w3adda.com/dart-tutorial/dart-getters-setters)





## OOP static method

正如[这里](https://stackoverflow.com/a/20741850/10173843)所总结的: 

> Dart doesn't inherit static methods to derived classes

github [Access of static class method by static generic method #34131](https://github.com/dart-lang/sdk/issues/34131)

通过其中的内容可以发现: 暂时并没有比较好的方式来实现调用基类的static method。

