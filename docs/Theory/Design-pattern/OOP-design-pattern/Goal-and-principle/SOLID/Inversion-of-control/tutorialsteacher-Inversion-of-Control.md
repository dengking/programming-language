

# tutorialsteacher [Inversion of Control](https://www.tutorialsteacher.com/ioc/inversion-of-control)

![](D:/github/dengking/programming-language-theory/programming-language/docs/Theory/Design-pattern/OOP-design-pattern/Goal-and-principle/SOLID/Inversion-of-control/ioc-step1.png)

> NOTE: 上面描述了实现IoC的多种方式，它们是递进关系。



Inversion of Control (IoC) is a design principle (although, some people refer to it as a pattern). As the name suggests, it is used to invert different kinds of controls in object-oriented design to achieve loose coupling. Here, controls refer to any additional responsibilities a class has, other than its main responsibility. This include control over the flow of an application, and control over the flow of an object creation or dependent object creation and binding.

> NOTE: 有了martinfowler [Inversion of Control Containers and the Dependency Injection pattern](https://martinfowler.com/articles/injection.html)中的解释，再来理解上面这段话，就非常任意理解了。

Let's understand how IoC inverts the different kinds of control.

## Control Over the Flow of a Program

> NOTE: 这一段中所列举的例子在martinfowler [Inversion of Control Containers and the Dependency Injection pattern](https://martinfowler.com/articles/injection.html)的Inversion of Control段中已经说明了，此处省略。

## Control Over the Dependent Object Creation

> NOTE: 这一段中所说明的其实就是dependency injection，这在martinfowler [Inversion of Control Containers and the Dependency Injection pattern](https://martinfowler.com/articles/injection.html)中已经说明了。

IoC can also be applied when we create objects of a dependent class. First of all, let's understand what we mean by dependency here.

```java
public class A
{
    B b;

    public A()
    {
        b = new B();
    }

    public void Task1() {
        // do something here..
        b.SomeMethod();
        // do something here..
    }

}

public class B {

    public void SomeMethod() { 
        //doing something..
    }
}
```

In the above example, `class A` calls `b.SomeMethod()` to complete its `task1`. `Class A` cannot complete its task without `class B` and so you can say that "Class A is dependent on class B" or "class B is a dependency of class A".

In the object-oriented design approach, classes need to interact with each other in order to complete one or more functionalities of an application, such as in the above example - classes `A` and `B`. Class `A` creates and manages the life time of an object of class `B`. Essentially, it controls the creation and life time of objects of the dependency class.

The IoC principle suggests to **invert the control.** This means to delegate the control to another class. In other words, invert the dependency creation control from class `A` to another class, as shown below.

> NOTE: 由另外的一个class来实现control，这其实是引入了一层indirection，最最典型的例子就是factory class、framework、container 。

```java
public class A
{
    B b;

    public A()
    {
        b = Factory.GetObjectOfB ();
    }

    public void Task1() {
        // do something here..
        b.SomeMethod();
        // do something here..
    }
}

public class Factory
{
    public static B GetObjectOfB() 
    {
        return new B();
    }
}
```

As you can see above, `class A` uses **Factory class** to get an object of `class B`. Thus, we have inverted the dependent object creation from `class A` to Factory. `Class A` no longer creates an object of `class B`, instead it uses the factory class to get the object of `class B`.

Let's understand this using a more practical example.

In an object-oriented design, classes should be designed in a loosely coupled way. Loosely coupled means changes in one class should not force other classes to change, so the whole application can become **maintainable** and **extensible**. Let's understand this by using typical n-tier architecture as depicted by the following figure:

![](D:/github/dengking/programming-language-theory/programming-language/docs/Theory/Design-pattern/OOP-design-pattern/Goal-and-principle/SOLID/Inversion-of-control/demo-architecture.png)

In the typical n-tier architecture, the User Interface (UI) uses **Service layer** to retrieve or save data. The **Service layer** uses the `BusinessLogic` class to apply business rules on the data. The `BusinessLogic` class depends on the `DataAccess` class which retrieves or saves the data to the underlying database. This is simple n-tier architecture design. Let's focus on the `BusinessLogic` and `DataAccess` classes to understand IoC.

The following is an example of `BusinessLogic` and `DataAccess` classes for a customer.

```java
public class CustomerBusinessLogic
{
    DataAccess _dataAccess;

    public CustomerBusinessLogic()
    {
        _dataAccess = new DataAccess();
    }

    public string GetCustomerName(int id)
    {
        return _dataAccess.GetCustomerName(id);
    }
}

public class DataAccess
{
    public DataAccess()
    {
    }

    public string GetCustomerName(int id) {
        return "Dummy Customer Name"; // get it from DB in real app
    }
}
```

As you can see in the above example, the `CustomerBusinessLogic` class depends on the `DataAccess` class. It creates an object of the `DataAccess` class to get the customer data.

Now, let's understand what's wrong with the above classes.

In the above example, `CustomerBusinessLogic` and `DataAccess` are **tightly coupled** classes because the `CustomerBusinessLogic` class includes the reference of the concrete `DataAccess` class. It also creates an object of `DataAccess` class and manages the lifetime of the object.

Problems in the above example classes:

1. `CustomerBusinessLogic` and `DataAccess` classes are tightly coupled classes. So, changes in the `DataAccess` class will lead to changes in the `CustomerBusinessLogic` class. For example, if we add, remove or rename any method in the `DataAccess` class then we need to change the `CustomerBusinessLogic` class accordingly.
2. Suppose the customer data comes from different databases or web services and, in the future, we may need to create different classes, so this will lead to changes in the `CustomerBusinessLogic` class.
3. The `CustomerBusinessLogic` class creates an object of the `DataAccess` class using the **new** keyword. There may be multiple classes which use the `DataAccess` class and create its objects. So, if you change the name of the class, then you need to find all the places in your source code where you created objects of `DataAccess` and make the changes throughout the code. This is repetitive code for creating objects of the same class and maintaining their dependencies.
4. Because the `CustomerBusinessLogic` class creates an object of the concrete `DataAccess` class, it cannot be tested independently (TDD). The `DataAccess` class cannot be replaced with a mock class.

> NOTE: 上述总结的问题非常值得学习

To solve all of the above problems and get a loosely coupled design, we can use the IoC and DIP principles together. Remember, IoC is a principle, not a pattern. It just gives high-level design guidelines but does not give implementation details. You are free to implement the IoC principle the way you want.

The following pattern (but not limited) implements the IoC principle.

![](D:/github/dengking/programming-language-theory/programming-language/docs/Theory/Design-pattern/OOP-design-pattern/Goal-and-principle/SOLID/Inversion-of-control/ioc-patterns.png)



Let's use the *Factory* pattern to implement IoC in the above example, as the first step towards attaining loosely coupled classes.

```c++
public class DataAccessFactory
{
    public static DataAccess GetDataAccessObj() 
    {
        return new DataAccess();
    }
}
```





## [Dependency Inversion Principle](https://www.tutorialsteacher.com/ioc/dependency-inversion-principle)

> NOTE: 本节对应的是第一节中给出的流程图中的“Implement DIP by creating abstraction”

First, let's understand what is Dependency Inversion Principle (DIP)?

DIP is one of the SOLID object-oriented principle invented by [Robert Martin](https://en.wikipedia.org/wiki/Robert_Cecil_Martin) (a.k.a. Uncle Bob)

### DIP Definition

> NOTE: 在维基百科[Dependency inversion principle](https://en.wikipedia.org/wiki/Dependency_inversion_principle)中也给出了相同的定义



### What is an Abstraction?



## [Dependency Injection](https://www.tutorialsteacher.com/ioc/dependency-injection)

> NOTE: 本节对应的是第一节中给出的流程图中的“Implement DI”