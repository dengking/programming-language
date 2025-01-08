# Java OOP



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

