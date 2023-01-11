# Stream

## docs.oracle [Interface Stream<T>](https://docs.oracle.com/javase/8/docs/api/java/util/stream/Stream.html)



## medium [How to use map, filter, and collect methods in Java Stream? Example Tutorial](https://medium.com/javarevisited/how-to-use-streams-map-filter-and-collect-methods-in-java-1e13609a318b)

```java
package tool;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;
/**
*
* A simple Java Program to demonstrate how to use map and filter method Java 8.
* In this program, we'll convert a list of String into a list of Integer and
* then filter all even numbers.
*/
public class Hello {
  
 public static void main(String[] args) {
    List<String> numbers = Arrays.asList("1", "2", "3", "4", "5", "6");
    System.out.println("original list: " + numbers);
    List<Integer> even = numbers.stream()
                                .map(s -> Integer.valueOf(s))
                                .filter(number -> number % 2 == 0)
                                .collect(Collectors.toList());
  
    System.out.println("processed list, only even numbers: " + even);
   }
}
```



## stackoverflow [Java 8 Streams FlatMap method example](https://stackoverflow.com/questions/22382453/java-8-streams-flatmap-method-example)



### [A](https://stackoverflow.com/a/22384132)

It doesn't make sense to `flatMap` a [Stream](http://download.java.net/jdk8/docs/api/java/util/stream/Stream.html) that's already flat, like the `Stream<Integer>` you've shown in your question.

However, if you had a `Stream<List<Integer>>` then it would make sense and you could do this:

```java
package com.company;

import java.util.Arrays;
import java.util.Collection;
import java.util.List;
import java.util.stream.Stream;

public class Main {

    public static void main(String[] args) {
        Stream<List<Integer>> integerListStream = Stream.of(
                Arrays.asList(1, 2),
                Arrays.asList(3, 4),
                Arrays.asList(5)
        );

        Stream<Integer> integerStream = integerListStream.flatMap(Collection::stream);
        integerStream.forEach(System.out::println);
    }
}

```

Which would print:

```java
1
2
3
4
5
```



