# Functional-interface&lambda-expression



## Java Functional Interfaces vs lambda expression

In Java, **functional interfaces** and **lambda expressions** are closely related concepts that are part of the Java 8 features aimed at supporting **functional programming**. Let's break down each concept and explain how they relate to each other.

### Functional Interfaces

A **functional interface** is an interface that contains exactly one **abstract method**. Functional interfaces can have multiple default or static methods, but they must have only one abstract method. They are used as the basis for lambda expressions and **method references**.

> NOTE: Java如何实现method reference?

#### Characteristics of Functional Interfaces:
1. **Single Abstract Method**: A functional interface must have exactly one **abstract method**. This is the method that will be implemented by the lambda expression.
2. **@FunctionalInterface Annotation**: Although not required, it is a good practice to use the `@FunctionalInterface` annotation to indicate that the interface is intended to be a functional interface. This annotation helps the compiler enforce the rule of having only one abstract method.

#### Example of a Functional Interface:
```java
@FunctionalInterface
public interface MyFunctionalInterface {
    void execute(); // Single abstract method
}
```

### Lambda Expressions

A **lambda expression** is a concise way to represent an instance of a functional interface. It allows you to implement the abstract method of a functional interface in a more readable and less verbose way than using an anonymous class.

#### Syntax of Lambda Expressions:
The syntax of a lambda expression consists of:
- A list of parameters (optional).
- The arrow token `->`.
- A body that can be a single expression or a block of code.

#### Example of a Lambda Expression:
Using the functional interface defined above, you can create a lambda expression as follows:
```java
MyFunctionalInterface myLambda = () -> System.out.println("Hello from Lambda!");
myLambda.execute(); // This will print: Hello from Lambda!
```

### Relationship Between Functional Interfaces and Lambda Expressions

1. **Implementation**: Lambda expressions are used to provide the implementation of the **abstract method** defined in a **functional interface**. When you create a lambda expression, you are essentially creating an instance of a functional interface.

2. **Type Inference**: The type of the lambda expression is inferred from the context in which it is used. For example, if you assign a lambda expression to a variable of a functional interface type, the compiler knows which functional interface to use.

3. **Use in Collections and Streams**: Functional interfaces are widely used in **Java's Collections Framework** and **Stream API**. For example, the `Predicate`, `Function`, and `Consumer` interfaces are all functional interfaces that can be implemented using lambda expressions.

### Example of Using Functional Interfaces with Lambda Expressions

Here’s a complete example that demonstrates the use of a functional interface with a lambda expression:

```java
import java.util.Arrays;
import java.util.List;
import java.util.function.Consumer;

@FunctionalInterface
interface MyFunctionalInterface {
    void execute(String message);
}

public class Main {
    public static void main(String[] args) {
        // Using a lambda expression to implement the functional interface
        MyFunctionalInterface myLambda = (message) -> System.out.println(message);
        myLambda.execute("Hello from MyFunctionalInterface!");

        // Using built-in functional interfaces
        List<String> names = Arrays.asList("Alice", "Bob", "Charlie");

        // Using a lambda expression with Consumer functional interface
        Consumer<String> printName = name -> System.out.println(name);
        names.forEach(printName);
    }
}
```

### Summary

- **Functional Interfaces**: Interfaces with a single abstract method, which can be implemented using lambda expressions. They enable functional programming in Java.

- **Lambda Expressions**: A concise way to implement the abstract method of a functional interface. They provide a more readable and less verbose syntax compared to **anonymous classes**.

  > NOTE: 下面是对比的example: 
  >
  > The compiler will allow us to use an **anonymous inner class** to instantiate a functional interface; however, this can lead to very verbose code. We should prefer to use lambda expressions:
  >
  > ```java
  > Foo foo = parameter -> parameter + " from Foo";
  > ```
  >
  > Over an anonymous inner class:
  >
  > ```java
  > Foo fooByIC = new Foo() {
  >     @Override
  >     public String method(String string) {
  >         return string + " from Foo";
  >     }
  > };
  > ```
  >
  > **The lambda expression approach can be used for any suitable interface from old libraries.** It is usable for interfaces like *Runnable*, *Comparator*, and so on; **however, this doesn't mean that we should review our whole older code base and change everything.**
  >
  > 

Together, functional interfaces and lambda expressions enhance Java's capabilities for functional programming, making it easier to write clean and maintainable code.



## docs.oracle 8 [Annotation Type FunctionalInterface](https://docs.oracle.com/javase/8/docs/api/java/lang/FunctionalInterface.html) 

> NOTE: functional interface的正式定义



## docs.oracle 8 [Package java.util.function](https://docs.oracle.com/javase/8/docs/api/java/util/function/package-summary.html)

### Functions

[Function](https://docs.oracle.com/javase/8/docs/api/java/util/function/Function.html)<T,R>

[BiFunction](https://docs.oracle.com/javase/8/docs/api/java/util/function/BiFunction.html)<T,U,R>



[DoubleFunction](https://docs.oracle.com/javase/8/docs/api/java/util/function/DoubleFunction.html)<R>

[DoubleToIntFunction](https://docs.oracle.com/javase/8/docs/api/java/util/function/DoubleToIntFunction.html) 

[DoubleToLongFunction](https://docs.oracle.com/javase/8/docs/api/java/util/function/DoubleToLongFunction.html) 



[IntFunction](https://docs.oracle.com/javase/8/docs/api/java/util/function/IntFunction.html)<R>

[IntToDoubleFunction](https://docs.oracle.com/javase/8/docs/api/java/util/function/IntToDoubleFunction.html) 

[IntToLongFunction](https://docs.oracle.com/javase/8/docs/api/java/util/function/IntToLongFunction.html) 



[LongFunction](https://docs.oracle.com/javase/8/docs/api/java/util/function/LongFunction.html)<R>

[LongToDoubleFunction](https://docs.oracle.com/javase/8/docs/api/java/util/function/LongToDoubleFunction.html) 

[LongToIntFunction](https://docs.oracle.com/javase/8/docs/api/java/util/function/LongToIntFunction.html)



#### Example

The most simple and general case of a lambda is a functional interface with a method that receives one value and returns another. This function of a single argument is represented by the *Function* interface, which is parameterized by the types of its argument and a return value:

```java
public interface Function<T, R> { … }
```

One of the usages of the *Function* type in the standard library is the *Map.computeIfAbsent* method. This method returns a value from a map by key, but calculates a value if a key is not already present in a map. To calculate a value, it uses the passed Function implementation:

```java
Map<String, Integer> nameMap = new HashMap<>();
Integer value = nameMap.computeIfAbsent("John", s -> s.length());
```

In this case, we will calculate a value by applying a function to a key, put inside a map, and also returned from a method call. **We may replace the lambda with a method reference that matches passed and returned value types**.

Remember that an object we invoke the method on is, in fact, the implicit first argument of a method. This allows us to cast an instance method *length* reference to a *Function* interface:

```java
Integer value = nameMap.computeIfAbsent("John", String::length);
```

The *Function* interface also has a default *compose* method that allows us to combine several functions into one and execute them sequentially:

```java
Function<Integer, String> intToString = Object::toString;
Function<String, String> quote = s -> "'" + s + "'";

Function<Integer, String> quoteIntToString = quote.compose(intToString);

assertEquals("'5'", quoteIntToString.apply(5));
```



To define lambdas with two arguments, we have to use additional interfaces that contain “*Bi”* keyword in their names: *BiFunction*, *ToDoubleBiFunction*, *ToIntBiFunction*, and *ToLongBiFunction*.



```java
Map<String, Integer> salaries = new HashMap<>();
salaries.put("John", 40000);
salaries.put("Freddy", 30000);
salaries.put("Samuel", 50000);

salaries.replaceAll((name, oldValue) -> 
  name.equals("Freddy") ? oldValue : oldValue + 10000);
```



### Consumers

[Consumer](https://docs.oracle.com/javase/8/docs/api/java/util/function/Consumer.html)<T> 

[BiConsumer](https://docs.oracle.com/javase/8/docs/api/java/util/function/BiConsumer.html)<T,U>  



#### Example

geeksforgeeks [Java 8 | Consumer Interface in Java with Examples](https://www.geeksforgeeks.org/java-8-consumer-interface-in-java-with-examples/)



```java
// Java Program to demonstrate
// Consumer's accept() method

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.function.Consumer;

public class Main {
	public static void main(String args[])
	{
		// Consumer to display a number
		Consumer<Integer> display = a -> System.out.println(a);

		// Implement display using accept()
		display.accept(10);

		// Consumer to multiply 2 to every integer of a list
		Consumer<List<Integer> > modify = list ->
		{
			for (int i = 0; i < list.size(); i++)
				list.set(i, 2 * list.get(i));
		};

		// Consumer to display a list of numbers
		Consumer<List<Integer> >
			dispList = list -> list.stream().forEach(a -> System.out.print(a + " "));

		List<Integer> list = new ArrayList<Integer>();
		list.add(2);
		list.add(1);
		list.add(3);

		// Implement modify using accept()
		modify.accept(list);

		// Implement dispList using accept()
		dispList.accept(list);
	}
}

```



```shell
javac Main.java
java Main
```



### Supplier

[Supplier](https://docs.oracle.com/javase/8/docs/api/java/util/function/Supplier.html)<T> 

[IntSupplier](https://docs.oracle.com/javase/8/docs/api/java/util/function/IntSupplier.html) 

#### Example



The *Supplier* functional interface is yet another *Function* specialization that does not take any arguments. We typically use it for lazy generation of values. For instance, let's define a function that squares a *double* value. It will not receive a value itself, but a *Supplier* of this value:

```java
public double squareLazy(Supplier<Double> lazyValue) {
    return Math.pow(lazyValue.get(), 2);
}
```

This allows us to lazily generate the argument for invocation of this function using a *Supplier* implementation. This can be useful if the generation of the argument takes a considerable amount of time. We'll simulate that using Guava's *sleepUninterruptibly* method:

```java
Supplier<Double> lazyValue = () -> {
    Uninterruptibles.sleepUninterruptibly(1000, TimeUnit.MILLISECONDS);
    return 9d;
};

Double valueSquared = squareLazy(lazyValue);
```

Another use case for the *Supplier* is defining logic for sequence generation. To demonstrate it, let’s use a static *Stream.generate* method to create a *Stream* of Fibonacci numbers:

```java
int[] fibs = {0, 1};
Stream<Integer> fibonacci = Stream.generate(() -> {
    int result = fibs[1];
    int fib3 = fibs[0] + fibs[1];
    fibs[0] = fibs[1];
    fibs[1] = fib3;
    return result;
});
```

The function that we pass to the *Stream.generate* method implements the *Supplier* functional interface. Notice that to be useful as a generator, the *Supplier* usually needs some sort of external state. In this case, its state comprises the last two Fibonacci sequence numbers.

To implement this state, we use an array instead of a couple of variables because **all external variables used inside the lambda have to be effectively final**.

Other specializations of the *Supplier* functional interface include *BooleanSupplier*, *DoubleSupplier*, *LongSupplier* and *IntSupplier*, whose return types are corresponding primitives.



### Predicate



#### Example

In mathematical logic, a predicate is a function that receives a value and returns a boolean value.

The *Predicate* functional interface is a specialization of a *Function* that receives a generified value and returns a boolean. A typical use case of the *Predicate* lambda is to filter a collection of values:

```java
List<String> names = Arrays.asList("Angela", "Aaron", "Bob", "Claire", "David");

List<String> namesWithA = names.stream()
  .filter(name -> name.startsWith("A"))
  .collect(Collectors.toList());Copy
```

In the code above, we filter a list using the *Stream* API and keep only the names that start with the letter “A”. The *Predicate* implementation encapsulates the filtering logic.

As in all of the previous examples, there are *IntPredicate*, *DoublePredicate* and *LongPredicate* versions of this function that receive primitive values.



### Operator











## docs.oracle.tutorial [Lambda Expressions](https://docs.oracle.com/javase/tutorial/java/javaOO/lambdaexpressions.html)



### Examples



stackoverflow [Return value by lambda in Java](https://stackoverflow.com/questions/38540481/return-value-by-lambda-in-java) # [A](https://stackoverflow.com/a/38540546)

```java
package com.company;


import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Main {

    static int counter = 0;

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        ExecutorService es = Executors.newSingleThreadExecutor();

        // execute only takes Runnable
        es.execute(() -> counter++);

        // force the lambda to be Runnable
        final Future<?> submit = es.submit((Runnable) () -> counter++);
        System.out.println(submit.get());

        // returns a value so it's a Callable<Integer>
        final Future<Integer> submit2 = es.submit(() -> counter++);
        System.out.println(submit2.get());

        // returns nothing so it must be Runnable
        final Future<?> submit3 = es.submit(() -> System.out.println("counter: " + counter));
        System.out.println(submit3.get());

        es.shutdown();
    }

}

```



## See also

medium [A Deep Dive into Supplier, Consumer, and Function in Java 8.](https://medium.com/@JavaFusion/a-deep-dive-into-supplier-consumer-and-function-in-java-8-cc2025cea6c1) 
