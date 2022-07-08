# C#attribute

microsoft [Attributes (C#)](https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/concepts/attributes/)

## C#attribute VS Java annotation 

### martinfowler [Annotation](https://martinfowler.com/bliki/Annotation.html)

> NOTE: 这篇文章还是不错的

An annotation on a program element (commonly a class, method, or field) is a piece of **meta-data** added to that program element which can be used to embellish that element with extra code.



### stackoverflow [What are the similarities and differences between Java Annotations and C# Attributes?](https://stackoverflow.com/questions/553857/what-are-the-similarities-and-differences-between-java-annotations-and-c-sharp-a)



#### [A](https://stackoverflow.com/a/556791/10173843)

Control over when your metadata is made accessible is different between the two languages.

> NOTE: 其实就是static和dynamic



#### [A](https://stackoverflow.com/a/553898/10173843)

One important aspect of Java annotations which I haven't looked at in detail yet: you can write code which uses annotations within javac (as of Java 6, I believe) [as a form of metaprogramming](http://java.sun.com/j2se/1.5.0/docs/guide/apt/GettingStarted.html).

[PostSharp](https://www.postsharp.net/) allows something similar, admittedly.

(That's far from the only difference, but it's all I have time for right now.)