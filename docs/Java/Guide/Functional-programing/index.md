# function



## baeldung [Lambda Expressions and Functional Interfaces: Tips and Best Practices](https://www.baeldung.com/java-8-lambda-expressions-tips)



### **1. Overview**

Now that Java 8 has reached wide usage, patterns and best practices have begun to emerge for some of its headlining features. In this tutorial, we'll take a closer look at functional interfaces and lambda expressions.

### **2. Prefer Standard Functional Interfaces**

Functional interfaces, which are gathered in the **[java.util.function](https://docs.oracle.com/en/java/javase/11/docs/api/java.base/java/util/function/package-summary.html)** package, satisfy most developers' needs in providing target types for lambda expressions and method references. Each of these interfaces is general and abstract, making them easy to adapt to almost any lambda expression. Developers should explore this package before creating new functional interfaces.

Let's consider an interface *Foo*:

```java
@FunctionalInterface
public interface Foo {
    String method(String string);
}
```

In addition, we have a method *add()* in some class *UseFoo*, which takes this interface as a parameter:

```java
public String add(String string, Foo foo) {
    return foo.method(string);
}
```

To execute it, we would write:

```java
Foo foo = parameter -> parameter + " from lambda";
String result = useFoo.add("Message ", foo);
```

If we look closer, we'll see that *Foo* is nothing more than a function that accepts one argument and produces a result. Java 8 already provides such an interface in *[Function](https://docs.oracle.com/en/java/javase/11/docs/api/java.base/java/util/function/Function.html)* from the [java.util.function](https://docs.oracle.com/en/java/javase/11/docs/api/java.base/java/util/function/package-summary.html) package.

Now we can remove interface *Foo* completely and change our code to:

```java
public String add(String string, Function<String, String> fn) {
    return fn.apply(string);
}Copy
```

To execute this, we can write:

```java
Function<String, String> fn = 
  parameter -> parameter + " from lambda";
String result = useFoo.add("Message ", fn);
```

### **5. Instantiate Functional Interfaces With Lambda Expressions**

The compiler will allow us to use an inner class to instantiate a functional interface; however, this can lead to very verbose code. We should prefer to use lambda expressions:

```java
Foo foo = parameter -> parameter + " from Foo";
```

Over an inner class:

```java
Foo fooByIC = new Foo() {
    @Override
    public String method(String string) {
        return string + " from Foo";
    }
};
```

**The lambda expression approach can be used for any suitable interface from old libraries.** It is usable for interfaces like *Runnable*, *Comparator*, and so on; **however, this doesn't mean that we should review our whole older code base and change everything.**



## baeldung [Functional Interfaces in Java 8](https://www.baeldung.com/java-8-functional-interfaces)



### **1. Introduction**

This tutorial is a guide to different functional interfaces present in Java 8, as well as their general use cases, and usage in the standard JDK library.

### **2. Lambdas in Java 8**

Java 8 brought a powerful new syntactic improvement in the form of lambda expressions. A lambda is an anonymous function that we can handle as a first-class language citizen. For instance, we can pass it to or return it from a method.

Before Java 8, we would usually create a class for every case where we needed to encapsulate a single piece of functionality. This implied a lot of unnecessary boilerplate code to define something that served as a primitive function representation.

The article [“Lambda Expressions and Functional Interfaces: Tips and Best Practices”](https://www.baeldung.com/java-8-lambda-expressions-tips) describes in more detail the functional interfaces and best practices of working with lambdas. This guide focuses on some particular functional interfaces that are present in the *java.util.function* package.

### **3. Functional Interfaces**

It's recommended that all functional interfaces have an informative *@FunctionalInterface* annotation. This clearly communicates the purpose of the interface, and also allows a compiler to generate an error if the annotated interface does not satisfy the conditions.

**Any interface with a SAM(Single Abstract Method) is a functional interface**, and its implementation may be treated as lambda expressions.

Note that Java 8's *default* methods are not *abstract* and do not count; a functional interface may still have multiple *default* methods. We can observe this by looking at the *Function's* [documentation](https://docs.oracle.com/en/java/javase/11/docs/api/java.base/java/util/function/Function.html).

### **4. Functions**

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

### **6. Two-Arity Function Specializations**

To define lambdas with two arguments, we have to use additional interfaces that contain “*Bi”* keyword in their names: *BiFunction*, *ToDoubleBiFunction*, *ToIntBiFunction*, and *ToLongBiFunction*.



```java
Map<String, Integer> salaries = new HashMap<>();
salaries.put("John", 40000);
salaries.put("Freddy", 30000);
salaries.put("Samuel", 50000);

salaries.replaceAll((name, oldValue) -> 
  name.equals("Freddy") ? oldValue : oldValue + 10000);
```



### **7. Suppliers**



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

### **8. Consumers**

As opposed to the *Supplier*, the *Consumer* accepts a generified argument and returns nothing. It is a function that is representing side effects.

For instance, let’s greet everybody in a list of names by printing the greeting in the console. The lambda passed to the *List.forEach* method implements the *Consumer* functional interface:

```java
List<String> names = Arrays.asList("John", "Freddy", "Samuel");
names.forEach(name -> System.out.println("Hello, " + name));Copy
```

There are also specialized versions of the *Consumer* — *DoubleConsumer*, *IntConsumer* and *LongConsumer* — that receive primitive values as arguments. More interesting is the *BiConsumer* interface. One of its use cases is iterating through the entries of a map:

```java
Map<String, Integer> ages = new HashMap<>();
ages.put("John", 25);
ages.put("Freddy", 24);
ages.put("Samuel", 30);

ages.forEach((name, age) -> System.out.println(name + " is " + age + " years old"));Copy
```

Another set of specialized *BiConsumer* versions is comprised of *ObjDoubleConsumer*, *ObjIntConsumer*, and *ObjLongConsumer,* which receive two arguments; one of the arguments is generified, and the other is a primitive type.

### **9. Predicates**

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

### **10. Operators**

*Operator* interfaces are special cases of a function that receive and return the same value type. The *UnaryOperator* interface receives a single argument. One of its use cases in the Collections API is to replace all values in a list with some computed values of the same type:

```java
List<String> names = Arrays.asList("bob", "josh", "megan");

names.replaceAll(name -> name.toUpperCase());Copy
```