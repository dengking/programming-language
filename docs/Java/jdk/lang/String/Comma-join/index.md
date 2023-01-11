# comma join string



## stackoverflow [What's the best way to build a string of delimited items in Java?](https://stackoverflow.com/questions/63150/whats-the-best-way-to-build-a-string-of-delimited-items-in-java)



### [A](https://stackoverflow.com/a/63258)

#### Pre Java 8:

Apache's commons lang is your friend here - it provides a join method very similar to the one you refer to in Ruby:

[`StringUtils.join(java.lang.Iterable,char)`](http://commons.apache.org/proper/commons-lang/apidocs/org/apache/commons/lang3/StringUtils.html#join(java.lang.Iterable, char))

------

#### Java 8:

Java 8 provides joining out of the box via `StringJoiner` and `String.join()`. The snippets below show how you can use them:

[`StringJoiner`](https://docs.oracle.com/javase/8/docs/api/java/util/StringJoiner.html)

```java
StringJoiner joiner = new StringJoiner(",");
joiner.add("01").add("02").add("03");
String joinedString = joiner.toString(); // "01,02,03"
```

------

[`String.join(CharSequence delimiter, CharSequence... elements))`](https://docs.oracle.com/javase/8/docs/api/java/lang/String.html#join-java.lang.CharSequence-java.lang.CharSequence...-)

```java
String joinedString = String.join(" - ", "04", "05", "06"); // "04 - 05 - 06"
```

------

[`String.join(CharSequence delimiter, Iterable elements)`](https://docs.oracle.com/javase/8/docs/api/java/lang/String.html#join-java.lang.CharSequence-java.lang.Iterable-)

```java
List<String> strings = new LinkedList<>();
strings.add("Java");strings.add("is");
strings.add("cool");
String message = String.join(" ", strings);
//message returned is: "Java is cool"
```



### [A](https://stackoverflow.com/a/27599779)

in Java 8 you can do this like:

```java
list.stream().map(Object::toString)
        .collect(Collectors.joining(delimiter));
```

if list has nulls you can use:

```java
list.stream().map(String::valueOf)
        .collect(Collectors.joining(delimiter))
```

it also supports prefix and suffix:

```java
list.stream().map(String::valueOf)
        .collect(Collectors.joining(delimiter, prefix, suffix));
```



### [A](https://stackoverflow.com/a/15815631)

Since Java 8:

```java
String.join(",", slist);
```

From Apache Commons library:

```java
import org.apache.commons.lang3.StringUtils
```

Use:

```java
StringUtils.join(slist, ',');
```

Another similar question and answer [here](https://stackoverflow.com/a/63258/1651233)



## see also

baeldung [Convert a List to a Comma-Separated String](https://www.baeldung.com/java-list-comma-separated-string)