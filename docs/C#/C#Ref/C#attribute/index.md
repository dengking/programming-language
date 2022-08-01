# C#attribute

microsoft [Attributes (C#)](https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/concepts/attributes/)

## C#attribute VS Java annotation 

### martinfowler [Annotation](https://martinfowler.com/bliki/Annotation.html)

> NOTE: 这篇文章还是不错的

An annotation on a program element (commonly a class, method, or field) is a piece of **meta-data** added to that program element which can be used to embellish(修饰) that element with extra code.



### stackoverflow [What are the similarities and differences between Java Annotations and C# Attributes?](https://stackoverflow.com/questions/553857/what-are-the-similarities-and-differences-between-java-annotations-and-c-sharp-a)



#### [A](https://stackoverflow.com/a/556791/10173843)

Control over when your metadata is made accessible is different between the two languages.

> NOTE: 其实就是static和dynamic



#### [A](https://stackoverflow.com/a/553898/10173843)

One important aspect of Java annotations which I haven't looked at in detail yet: you can write code which uses annotations within javac (as of Java 6, I believe) [as a form of metaprogramming](http://java.sun.com/j2se/1.5.0/docs/guide/apt/GettingStarted.html).

[PostSharp](https://www.postsharp.net/) allows something similar, admittedly.

(That's far from the only difference, but it's all I have time for right now.)



## [Tutorials](https://docs.microsoft.com/en-us/dotnet/csharp/tutorials/) # [Use Attributes in C#](https://docs.microsoft.com/en-us/dotnet/csharp/tutorials/attributes)

Attributes provide a way of associating information with code in a **declarative way**. They can also provide a reusable element that can be applied to a variety of targets.

> NOTE: meta-programming、code-reuse。

### How to add attributes to code

In C#, attributes are classes that inherit from the `Attribute` base class. Any class that inherits from `Attribute` can be used as a sort of "tag" on other pieces of code.



### How to create your own attribute

Creating an attribute is as simple as inheriting from the `Attribute` base class.



Attributes in the .NET base class library like `ObsoleteAttribute` trigger certain behaviors within the compiler. However, any attribute you create acts only as **metadata**, and doesn't result in any code within the attribute class being executed. It's up to you to act on that metadata elsewhere in your code (more on that later in the tutorial).

There is a 'gotcha' here to watch out for. As mentioned above, only certain types are allowed to be passed as arguments when using attributes. 

> NOTE: "gotcha"的意思是"陷阱"



### How to use attributes attached to a code element

Attributes act as metadata. Without some **outward force**, they won't actually do anything.

> NOTE: 
>
> 一、"outward force"的意思是"外力"

To find and act on attributes, [Reflection](https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/concepts/reflection) is generally needed. I won't cover Reflection in-depth in this tutorial, but the basic idea is that Reflection allows you to write code in C# that examines other code.



```c#
public class MyUIClass : INotifyPropertyChanged
{
    public event PropertyChangedEventHandler PropertyChanged;

    public void RaisePropertyChanged([CallerMemberName] string propertyName = null)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }

    private string _name;
    public string Name
    {
        get { return _name;}
        set
        {
            if (value != _name)
            {
                _name = value;
                RaisePropertyChanged();   // notice that "Name" is not needed here explicitly
            }
        }
    }
}
```

