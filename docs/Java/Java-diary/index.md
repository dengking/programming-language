# Java assertion



## baeldung [Using Java Assertions](https://www.baeldung.com/java-assert)

Therefore, for backward compatibility, **the JVM disables assertion validation by default**. They must be explicitly enabled using either the *-enableassertions* command line argument, or its shorthand *-ea:*

```bash
java -ea com.baeldung.assertion.Assertion
```



## Java resource management



### baeldung [Is There a Destructor in Java?](https://www.baeldung.com/java-destructor)

### cnblogs [浅谈 Java 中的 AutoCloseable 接口 ](https://www.cnblogs.com/vivotech/p/13886291.html)





## Java file io

baeldung [List Files in a Directory in Java](https://www.baeldung.com/java-list-directory-files)



stackoverflow [Create a directory if it does not exist and then create the files in that directory as well](https://stackoverflow.com/questions/28947250/create-a-directory-if-it-does-not-exist-and-then-create-the-files-in-that-direct) # [A](https://stackoverflow.com/a/59376080)

> ```java
> Files.createDirectories(Paths.get("/Your/Path/Here"));
> ```

## Java stream

### stream to map



geeksforgeeks [How to convert a Stream into a Map in Java](https://www.geeksforgeeks.org/how-to-convert-a-stream-into-a-map-in-java/)



### Java 8 写法



```java
List<T> list;
list.parallelStream()
                .collect(Collectors.toMap(
                        section -> section,
                        section -> getNearbySections(searcher, section)
                ));
```



## Java regex



```java
.find()
```



stackoverflow [Regex returns a boolean not a string [duplicate]](https://stackoverflow.com/questions/38759472/regex-returns-a-boolean-not-a-string) # [A](https://stackoverflow.com/a/38759516)



## Java data structure

stackoverflow [Initialization of an ArrayList in one line](https://stackoverflow.com/questions/1005073/initialization-of-an-arraylist-in-one-line) # [A](https://stackoverflow.com/a/3676539)

### java partition list by size

stackoverflow [Efficient way to divide a list into lists of n size](https://stackoverflow.com/questions/5824825/efficient-way-to-divide-a-list-into-lists-of-n-size) 

stackoverflow [Is there a common Java utility to break a list into batches?](https://stackoverflow.com/questions/12026885/is-there-a-common-java-utility-to-break-a-list-into-batches) 

baeldung [Partition a List in Java](https://www.baeldung.com/java-list-split) 

printstacktrace [Divide a list to lists of n size in Java 8](https://e.printstacktrace.blog/divide-a-list-to-lists-of-n-size-in-Java-8/) 



stackoverflow [Partition a Set into smaller Subsets and process as batch](https://stackoverflow.com/questions/19423326/partition-a-set-into-smaller-subsets-and-process-as-batch) 



### Java flat list of list

stackoverflow [How can I turn a List of Lists into a List in Java 8?](https://stackoverflow.com/questions/25147094/how-can-i-turn-a-list-of-lists-into-a-list-in-java-8) 



## Java generic programming

docs.oracle [Restrictions on Generics](https://docs.oracle.com/javase/tutorial/java/generics/restrictions.html)



## Mac install java



### stackoverflow [How to install Java 8 on Mac](https://stackoverflow.com/questions/24342886/how-to-install-java-8-on-mac) 

[A](https://stackoverflow.com/a/28635465)

```SHELL
brew tap adoptopenjdk/openjdk
brew install --cask adoptopenjdk8
```



[A](https://stackoverflow.com/a/32811065)
