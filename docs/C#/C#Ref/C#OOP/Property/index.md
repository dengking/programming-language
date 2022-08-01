# C# property



## [C# guide](https://docs.microsoft.com/en-us/dotnet/csharp/) # [Properties](https://docs.microsoft.com/en-us/dotnet/csharp/properties)

Properties are first class citizens in C#. 

### Property syntax

The syntax for properties is a natural extension to fields. 

> NOTE:
>
> "属性的语法是字段的自然扩展。"





```C#
public class Person
{
    public string FirstName;

    // Omitted for bevity.
}
```

> NOTE:
>
> 从上述代码片段可以看出，C# attribute通常是以大写打头的。
>
> 



A property definition contains declarations for a `get` and `set` accessor that retrieves and assigns the value of that property:

```c#
public class Person
{
    public string FirstName { get; set; }

    // Omitted for bevity.
}
```

The syntax shown above is the *auto property* syntax. The compiler generates the storage location for the field that backs up the property. The compiler also implements the body of the `get` and `set` accessors.

