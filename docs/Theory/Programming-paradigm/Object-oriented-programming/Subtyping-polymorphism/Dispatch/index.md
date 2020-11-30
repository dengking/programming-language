# Dynamic dispatch

本文介绍OOP subtyping polymorphism的实现。

## Single dispatch and multiple dispatch

关于single dispatch和multiple dispatch的讨论，参见`Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism\Implementation`章节。

## Dynamic dispatch

### wikipedia [Dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **dynamic dispatch** is the process of selecting which implementation of a [polymorphic](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) operation ([method](https://en.wikipedia.org/wiki/Method_(computer_programming)) or function) to call at [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)). It is commonly employed in, and considered a prime characteristic of, [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming) (OOP) languages and systems.[[1\]](https://en.wikipedia.org/wiki/Dynamic_dispatch#cite_note-1)



#### [Single and multiple dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch#Single_and_multiple_dispatch)



#### [Dynamic dispatch mechanisms](https://en.wikipedia.org/wiki/Dynamic_dispatch#Dynamic_dispatch_mechanisms)

> NOTE: 描述如何试下dynamic dispatch

## Multiple dispatch



### wikipedia [Multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch)



**Multiple dispatch** or **multimethods** is a feature of some [programming languages](https://en.wikipedia.org/wiki/Programming_language) in which a [function](https://en.wikipedia.org/wiki/Subroutine) or [method](https://en.wikipedia.org/wiki/Method_(computer_programming)) can be [dynamically dispatched](https://en.wikipedia.org/wiki/Dynamic_dispatch) based on the [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)) (dynamic) type or, in the more general case, some other attribute of more than one of its [arguments](https://en.wikipedia.org/wiki/Parameter_(computer_programming)).[[1\]](https://en.wikipedia.org/wiki/Multiple_dispatch#cite_note-1) This is a generalization of [single dispatch](https://en.wikipedia.org/wiki/Single_dispatch) [polymorphism](https://en.wikipedia.org/wiki/Polymorphism_in_object-oriented_programming) where a function or method call is dynamically dispatched based on the derived type of the object on which the method has been called. 

**Multiple dispatch routes the dynamic dispatch to the implementing function or method using the combined characteristics of one or more arguments.**

> NOTE:最后一段话是点睛之笔。关于"route"这个词，参见文章`Abstract-and-concrete`。

#### Examples

**Languages with built-in multiple dispatch**

##### C#

[C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) introduced support for dynamic multimethods in version 4[[8\]](https://en.wikipedia.org/wiki/Multiple_dispatch#cite_note-8) (April 2010) using the '`dynamic`' keyword. 

```C#
   class Program
   {
        static void Main(string[] args)
        {
            // Static dispatch to Collider.Collide method
            Console.WriteLine(Collider.Collide(new Asteroid(101), new Spaceship(300)));
            Console.WriteLine(Collider.Collide(new Asteroid(10), new Spaceship(10)));
            Console.WriteLine(Collider.Collide(new Spaceship(101), new Spaceship(10)));
        }
    }

    static class Collider
    {
        public static string Collide(SpaceObject x, SpaceObject y) => (x, y) switch
        {
            _ when x.Size > 100 && y.Size > 100 => "big-boom",
            _ => CollideWith(x as dynamic, y as dynamic)  // Dynamic dispatch to CollideWith method
        };

        // C# does not support global functions.  Class methods are the only way to implement
        // the CollideWith functions.  You could define these as non-private static methods in
        // a separate class and use a 'using static' directive to reference them as if they
        // were global.  This would not require any change to the Collide method above.
        private static string CollideWith(Asteroid x, Asteroid y) => "a/a";
        private static string CollideWith(Asteroid x, Spaceship y) => "a/s";
        private static string CollideWith(Spaceship x, Asteroid y) => "s/a";
        private static string CollideWith(Spaceship x, Spaceship y) => "s/s";
    }

    abstract class SpaceObject
    {
        public SpaceObject(int size)
        {
            Size = size;
        }

        public int Size { get; }
    }

    class Asteroid : SpaceObject
    {
        public Asteroid(int size) : base(size) { }
    }

    class Spaceship : SpaceObject
    {
        public Spaceship(int size) : base(size) { }
    }

```

Output:

```
big-boom
a/s
s/s
```

## Double dispatch

### wikipedia [Double dispatch](https://en.wikipedia.org/wiki/Double_dispatch) 

In [software engineering](https://en.wikipedia.org/wiki/Software_engineering), **double dispatch** is a special form of [multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch), and a mechanism that dispatches a function call to different concrete functions depending on the runtime types of two objects involved in the call.



### TODO

[Double Dispatch: the next best thing with respect to Dependency Injection](https://dzone.com/articles/double-dispatch-next-best)



[How does double dispatch work in Visitor pattern?](https://stackoverflow.com/questions/6762256/how-does-double-dispatch-work-in-visitor-pattern)



[Double Dispatch is a Code Smell · Los Techies](https://lostechies.com/derekgreer/2010/04/19/double-dispatch-is-a-code-smell/)



[A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/)

