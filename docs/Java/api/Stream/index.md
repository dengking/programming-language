# Stream



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



## docs.oracle [Interface Stream<T>](https://docs.oracle.com/javase/8/docs/api/java/util/stream/Stream.html)