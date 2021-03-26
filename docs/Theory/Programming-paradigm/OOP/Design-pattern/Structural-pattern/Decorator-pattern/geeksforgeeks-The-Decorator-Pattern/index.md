# geeksforgeeks Decorator Pattern 

这个系列文章是我见过的讲述decorator pattern的非常好的文章。

## geeksforgeeks [Decorator Pattern | Set 1 (Background)](https://www.geeksforgeeks.org/decorator-pattern/)

To understand decorator pattern let us consider a scenario inspired from the book “Head First Design Pattern”. Suppose we are building an application for a pizza store and we need to model their pizza classes. Assume they offer four types of pizzas namely Peppy Paneer, Farmhouse, Margherita and Chicken Fiesta. Initially we just use inheritance and abstract out the common functionality in a **Pizza** class.

[![pizza1](https://media.geeksforgeeks.org/wp-content/uploads/decorePattern-1.png)](https://media.geeksforgeeks.org/wp-content/uploads/decorePattern-1.png)

Each pizza has different cost. We have overridden the `getCost()` in the subclasses to find the appropriate cost. Now suppose a new requirement, in addition to a pizza, customer can also ask for several toppings(浇头) such as Fresh Tomato, Paneer, Jalapeno, Capsicum, Barbeque, etc. Let us think about for sometime that how do we accommodate changes in the above classes so that customer can choose pizza with toppings and we get the total cost of pizza and toppings the customer chooses.

> NOTE: 
>
> 1、第一想法肯定是combination: 显然pizza 和 topping 会产生非常多的combination，
>
> 2、如果为每个combination都实现一个class，那么显然会发生组合爆炸

Let us look at various options.

### Option 1

> NOTE: 
>
> 1、这种做法就是为每个组合都定义一个class

Create a new subclass for every topping with a pizza. The class diagram would look like:

[
![pizza2](https://media.geeksforgeeks.org/wp-content/uploads/decorePattern.png)](https://media.geeksforgeeks.org/wp-content/uploads/decorePattern.png)

This looks very complex. There are way too many classes and is a maintenance nightmare. Also if we want to add a new topping or pizza we have to add so many classes. This is obviously very bad design.

### Option 2:

Let’s add instance variables to pizza base class to represent whether or not each pizza has a topping. The class diagram would look like:![pizza3](https://media.geeksforgeeks.org/wp-content/uploads/decorePattern-2.png)

The `getCost()` of superclass calculates the costs for all the toppings(浇头) while the one in the subclass adds the cost of that specific pizza.



```java
// Sample getCost() in super class
public int getCost()
{
    int totalToppingsCost = 0;
    if (hasJalapeno() )
        totalToppingsCost += jalapenoCost;
    if (hasCapsicum() )
        totalToppingsCost += capsicumCost;

    // similarly for other toppings
    return totalToppingsCost;
}
```



```java
// Sample getCost() in subclass
public int getCost()
{
    // 100 for Margherita and super.getCost()
    // for toppings.
    return super.getCost() + 100;
}
```

This design looks good at first but lets take a look at the problems associated with it.

1、Price changes in toppings(浇头) will lead to alteration in the existing code.

2、New toppings will force us to add new methods and alter `getCost()` method in superclass.

3、For some pizzas, some toppings may not be appropriate yet the subclass inherits them.

4、What if customer wants double capsicum or double cheeseburst?

In short our design violates(违反了) one of the most popular design principle – [**The Open-Closed Principle**](https://en.wikipedia.org/wiki/Open/closed_principle) which states that classes should be open for extension and closed for modification.

## geeksforgeeks [The Decorator Pattern | Set 2 (Introduction and Design)](https://www.geeksforgeeks.org/the-decorator-pattern-set-2-introduction-and-design/)

As we saw our [previous designs](https://www.geeksforgeeks.org/decorator-pattern/) using inheritance didn’t work out that well. In this article, decorator pattern is discussed for the design problem in previous set.

So what we do now is take a pizza and “decorate” it with toppings(浇头) at runtime:

1、Take a pizza object.

![piz1](https://media.geeksforgeeks.org/wp-content/uploads/Decorator1.jpg)[
](https://media.geeksforgeeks.org/wp-content/cdn-uploads/piz3.png)

2、“Decorate” it with a Capsicum object.

![piz2](https://media.geeksforgeeks.org/wp-content/uploads/Decorator2.jpg)

3、“Decorate” it with a CheeseBurst object.

![piz3](https://media.geeksforgeeks.org/wp-content/uploads/Decorator3.jpg)

3、Call `getCost()` and use delegation instead of inheritance to calculate the toppings cost.
![decorator pattern](https://media.geeksforgeeks.org/wp-content/uploads/Decorator4.jpg)

What we get in the end is a pizza with cheeseburst and capsicum toppings. Visualize the “decorator” objects like wrappers. Here are some of the properties of decorators:

1、Decorators have the same super type as the object they decorate.

2、You can use multiple decorators to wrap an object.

3、Since decorators have same type as object, we can pass around decorated object instead of original.

4、We can decorate objects at runtime.

### Definition:

*The decorator pattern attaches additional responsibilities to an object dynamically. Decorators provide a flexible alternative to subclassing for extending functionality.*

### Class Diagram:

![piz5](https://media.geeksforgeeks.org/wp-content/uploads/uml.jpg)

Image src: [Wikipedia](https://upload.wikimedia.org/wikipedia/commons/thumb/e/e9/Decorator_UML_class_diagram.svg/600px-Decorator_UML_class_diagram.svg.png)

1、Each component can be used on its own or may be wrapped by a decorator.

2、Each decorator has an instance variable that holds the reference to component it decorates(HAS-A relationship).

3、The `ConcreteComponent` is the object we are going to dynamically decorate.

### Advantages:

1、The decorator pattern can be used to make it possible to extend (decorate) the functionality of a certain object at runtime**.**

2、The decorator pattern is an alternative to subclassing. Subclassing adds behavior at compile time, and the change affects all instances of the original class; decorating can provide new behavior at runtime for individual objects.

3、Decorator offers a pay-as-you-go approach to adding responsibilities. Instead of trying to support all foreseeable features in a complex, customizable class, you can define a simple class and add functionality incrementally with Decorator objects.

### Disadvantages:

1、Decorators can complicate the process of instantiating the component because you not only have to instantiate the component, but wrap it in a number of decorators.

2、It can be complicated to have decorators keep track of other decorators, because to look back into multiple layers of the decorator chain starts to push the decorator pattern beyond its true intent.

### References:

1、Head First Design Patterns(Book)

2、https://neillmorgan.wordpress.com/2010/02/07/decorator-pattern-pros-and-cons/

3、https://en.wikipedia.org/wiki/Decorator_pattern

4、http://stackoverflow.com/questions/4842978/decorator-pattern-versus-sub-classing