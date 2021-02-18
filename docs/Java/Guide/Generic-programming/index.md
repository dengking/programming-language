# Java generic programming

在`Theory\Programming-paradigm\Generic-programming`章节中阐述了generic programming的核心思想，其中非常重要的一点是实现concept，并且基于concept来设计algorithm。后来无意中，阅读了Java的"interface as method parameter"相关的文章，对Java generic programming有了一个新的认知，下面首先描述这两篇文章，然后对Java generic programming进行总结。



## Interface as method parameter

### stackoverflow [interface as a method parameter in Java](https://stackoverflow.com/questions/2575429/interface-as-a-method-parameter-in-java)

I had an interview days ago and was thrown a question like this.

Q: Reverse a linked list. Following code is given:

```java
public class ReverseList { 
    interface NodeList {
        int getItem();
        NodeList nextNode();
    }
    void reverse(NodeList node) {

    }
    public static void main(String[] args) {

    }
}
```

> NOTE: `NodeList`是一个interface，`void reverse(NodeList node)`方法接收一个interface parameter。

I was confused because I did not know an interface object could be used as a method parameter. The interviewer explained a little bit but I am still not sure about this. Could somebody enlighten(启发) me?

[A](https://stackoverflow.com/a/2575454)

> NOTE: 
>
> 1、interface is a contact

This is in fact one of the most common and useful ways to use an interface. The interface defines a contract, and your code can work with any class that implements the interface, without having to know the concrete class - it can even work with classes that didn't exist yet when the code was written.

There are many examples in the Java standard API, especially in the collections framework. For example, [Collections.sort()](http://java.sun.com/javase/6/docs/api/java/util/Collections.html#sort(java.util.List)) can sort anything that implements the `List` interface (not just `ArrayList` or `LinkedList`, though implementing your own `List` is uncommon) and whose contents implement the `Comparable` interface (not just `String` or the numerical wrapper classes - and having your own class implement `Comparable` for that purpose is *quite* common).

[A](https://stackoverflow.com/a/2575450)

> NOTE: 这是容易让人混淆的

It's not the interface "object" being passed to the method, still just a regular object. It's just a way of saying "this parameter will accept any object that supports this interface". It's equivalent to accepting some object of a base class type, even if you're passing in a subclass.

### stackoverflow [Java Interface as argument to a method](https://stackoverflow.com/questions/40207720/java-interface-as-argument-to-a-method)



[A](https://stackoverflow.com/a/40207776)

Yes - if an interface type is required as an argument, you can pass any object whose class implements this interface.

Example:

```java
// the method
void myMethod(MyInterface object) { ... }

// the interface
interface MyInterface {

    void interfaceMethod();

}

// class implementing the interface
class MyImplementation implements MyInterface {

    void interfaceMethod() {
        // ...
    }

}
```

You can now do this

```java
MyInterface object = new MyImplementation();
myMethod(object);
```

Hope this helps!



## Java generic programming 

### Java interface

我是通过C++ template来学习generic programming的，在阅读了"Interface as method parameter"章节的example后，我的想法是: 

1、如果使用C++ template来实现上述"Interface as method parameter"章节的example，我们使用类比思维: 我觉得上述example也是符合generic programming的: 

a、Java interface特性让programmer能够explicit、formal描述interface，Java通过interface来进行generic programming，java是interface是完全behavior-based的；Java interface能够让programmer来实现concept。

b、相比于C++ template behavior based的implicit，java interface是explicit。

关于Java interface，参见:

1、wikipedia [Interface (Java)](https://en.wikipedia.org/wiki/Interface_(Java))

### Java Generics

仅仅使用Java interface还不足以完全地实现generic programming，Java还有generics特性，借助这个特性，Java能够更加完整的实现generic programming。

关于Java generics，参见:

1、wikipedia [Generics in Java](https://en.wikipedia.org/wiki/Generics_in_Java)

2、tutorialspoint [Java - Generics](https://www.tutorialspoint.com/java/java_generics.htm)



## C++ VS Java: generic programming

1、Java的generic programming可以使用Java interface，Java interface是对behavior的formal description，Java interface是完全behavior-based的；C++ template也是behavior based的，但是它的interface是implicit，java interface是explicit。C++ concept相比之下，不似JAVA那般直观、容易理解