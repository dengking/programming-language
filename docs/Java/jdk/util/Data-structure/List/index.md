# List



## stackoverflow [How to make a new List in Java](https://stackoverflow.com/questions/858572/how-to-make-a-new-list-in-java)



[A](https://stackoverflow.com/a/858590)

```java
List myList = new ArrayList();
```

or with generics ([Java 7](https://docs.oracle.com/javase/tutorial/java/generics/types.html#diamond) or later)

```java
List<MyType> myList = new ArrayList<>();
```

or with generics (Old java versions)

```java
List<MyType> myList = new ArrayList<MyType>();
```



[A](https://stackoverflow.com/a/858602)

Additionally, if you want to create a list that has things in it (though it will be fixed size):

```java
List<String> messages = Arrays.asList("Hello", "World!", "How", "Are", "You");
```



[A](https://stackoverflow.com/a/48673336)

### In Java 8

To create a non-empty list of fixed size (operations like add, remove, etc., are not supported):

```java
List<Integer> list = Arrays.asList(1, 2); // but, list.set(...) is supported
```

To create a non-empty mutable list:

```java
List<Integer> list = new ArrayList<>(Arrays.asList(3, 4));
```

### In Java 9

Using a new [`List.of(...)`](https://docs.oracle.com/javase/9/docs/api/java/util/List.html#of-E...-) static factory methods:

```java
List<Integer> immutableList = List.of(1, 2);

List<Integer> mutableList = new ArrayList<>(List.of(3, 4));
```

### In Java 10

Using the [Local Variable Type Inference](https://developer.oracle.com/java/jdk-10-local-variable-type-inference.html):

```java
var list1 = List.of(1, 2);

var list2 = new ArrayList<>(List.of(3, 4));

var list3 = new ArrayList<String>();
```

And follow best practices...

### [Don't use raw types](https://stackoverflow.com/questions/2770321/what-is-a-raw-type-and-why-shouldnt-we-use-it)

Since Java 5, generics have been a part of the language - you should use them:

```java
List<String> list = new ArrayList<>(); // Good, List of String

List list = new ArrayList(); // Bad, don't do that!
```

### [Program to interfaces](https://stackoverflow.com/questions/383947/what-does-it-mean-to-program-to-an-interface)

For example, program to the `List` interface:

```java
List<Double> list = new ArrayList<>();
```

Instead of:

```java
ArrayList<Double> list = new ArrayList<>(); // This is a bad idea!
```