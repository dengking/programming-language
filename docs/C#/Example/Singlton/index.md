# C# Singleton



## [C# in Depth](https://csharpindepth.com/) # [Implementing the Singleton Pattern in C#](https://csharpindepth.com/articles/singleton)

```c++
using System;
public sealed class Singleton
{
    private static readonly Singleton instance = new Singleton();

    // Explicit static constructor to tell C# compiler
    // not to mark type as beforefieldinit
    static Singleton()
    {
        Console.WriteLine("static constructor");
    }

    private Singleton()
    {
        Console.WriteLine("instance constructor");

    }

    public static Singleton Instance
    {
        get
        {
            return instance;
        }
    }
    public void Test()
    {
        Console.WriteLine("Test");

    }

}

public class MainClass
{
    public static void Main(String[] args)
    {
        Console.WriteLine("begin main");
        Singleton.Instance.Test();
        Singleton.Instance.Test();
        Singleton.Instance.Test();
        Console.WriteLine("end main");

    }
}
```



构造函数在main函数被执行之前就已经执行了。

```
begin main
instance constructor
static constructor
Test
Test
Test
end main
```

