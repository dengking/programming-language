# `super` in C++



## fluentcpp [How to Emulate The “super” Keyword In C++](https://www.fluentcpp.com/2017/12/26/emulate-super-base/)

A derived class sometimes needs to call the code of its base class and **name it explicitly**.

But for bases classes with a long name, repeating it in the body of the derived class adds a lot of clutter to it. And C++ doesn’t have a `super` or `base` keyword to designate “the base class”, like C# and Java do.

One reason for this is that C++ supports multiple inheritance, which would make such a keyword ambiguous. But on the other hand, multiple inheritance is not used so often in C++. So how can a derived class designate its base in the case of **single inheritance**?

Fortunately there are ways to do this, to make the code of the derived class more expressive.

### When it becomes a problem

The above cases use short names for the base class. But what if the base class was a template, and located in a nested namespace different from the derived type’s namespace?

```c++
class Oval : public NA::NB::NC::Shape<FirstParameter, SecondParameter, ThirdParameter>
{
public:
    Oval(Color color) : NA::NB::NC::Shape<FirstParameter, SecondParameter, ThirdParameter>(color){}
    
    void draw() const override
    {
        NA::NB::NC::Shape<FirstParameter, SecondParameter, ThirdParameter>::draw();
    }
};
```

Ew. That’s a lot of code, and which doesn’t express anything more that the previous code snippets. It just has the same base name repeated over and over.

This is where we start eyeing the other languages with envy. But not yet! C++ has what we need to remove all that redundant information.

### You don’t have to write what the compiler already knows

One of the goals that the language is steering towards is to relieve the developer from the work that the compiler can do on its own. This is well illustrated with the `auto` keyword in C++11 and template deduction in constructors in C++17.

And even since C++98, the compiler could figure out that when the derived class talks about “`Shape`“, it is its base class that it means. So the above is exactly equivalent to:

```C++
class Oval : public NA::NB::NC::Shape<FirstParameter, SecondParameter, ThirdParameter>
{
public:
    Oval(Color color) : Shape(color){}
    
    void draw() const override
    {
        Shape::draw();
    }
};
```

This works unless there is an ambiguity, like if the derived class inherits from two different specializations of the template base class. Or from two classes with the same name but in different namespaces.

But when there isn’t such a alignment of planets as that, using the base class’s name without its template parameters or namespaces works.

### Using an alias

Still, if you want to abstract the name of the base class behind a dedidcated word, such as `super` or `base`, this is possible by using a typedef or a using declaration.

One way to go about this is to add the using declaration at the beginning of the derived class (private) section:

```c++
class Oval : public NA::NB::NC::Shape<FirstParameter, SecondParameter, ThirdParameter>
{
    using base_type = Shape;
public:
    Oval(Color color) : base_type(color){}
    
    void draw() const override
    {
        base_type::draw();
    }
};
```

But there is another place for the using declaration: **in the base class itself**. I first saw this in the [implementation of the Boost spirit](https://github.com/boostorg/spirit/blob/9a8cded55355644c749d6511504b59c048d64de3/include/boost/spirit/home/qi/nonterminal/grammar.hpp#L45) library.

If you opt for doing this, it makes sense to put the declaration in the **protected** section of the base class. This way, it cannot be mixed up as the base class of the base class itself:

```c++
template<typename T1, typename T2, typename T3>
class Shape
{
public:
    explicit Shape(Color){}
    virtual void draw() const
    {
        // base class implementation for draw
    }
protected:
    using base_type = Shape;
};
```

## stackoverflow [Using “super” in C++](https://stackoverflow.com/questions/180601/using-super-in-c)

My style of coding includes the following idiom:

```cpp
class Derived : public Base
{
   public :
      typedef Base super; // note that it could be hidden in
                          // protected/private section, instead

      // Etc.
} ;
```

This enables me to use "super" as an alias to Base, for example, in constructors:

```cpp
Derived(int i, int j)
   : super(i), J(j)
{
}
```

Or even when calling the method from the base class inside its overridden version:

```cpp
void Derived::foo()
{
   super::foo() ;

   // ... And then, do something else
}
```

It can even be chained (I have still to find the use for that, though):

```cpp
class DerivedDerived : public Derived
{
   public :
      typedef Derived super; // note that it could be hidden in
                             // protected/private section, instead

      // Etc.
} ;

void DerivedDerived::bar()
{
   super::bar() ; // will call Derived::bar
   super::super::bar ; // will call Base::bar

   // ... And then, do something else
}
```

Anyway, I find the use of "typedef super" very useful, for example, when Base is either verbose and/or templated.

The fact is that super is implemented in Java, as well as in C# (where it is called "base", unless I'm wrong). But C++ lacks this keyword.

### [A](https://stackoverflow.com/a/180634)

I've always used "inherited" rather than super. (Probably due to a Delphi background), and I always make it **private**, to avoid the problem when the 'inherited' is erroneously omitted from a class but a subclass tries to use it.

```cpp
class MyClass : public MyBase
{
private:  // Prevents erroneous use by other classes.
  typedef MyBase inherited;
...
```

My standard 'code template' for creating new classes includes the typedef, so I have little opportunity to accidentally omit it.

I don't think the chained "super::super" suggestion is a good idea- If you're doing that, you're probably tied in very hard to a particular hierarchy, and changing it will likely break stuff badly.



### [A](https://stackoverflow.com/a/180627)

One problem with this is that if you forget to (re-)define super for derived classes, then any call to super::something will compile fine but will probably not call the desired function.

For example:

```cpp
class Base
{
public:  virtual void foo() { ... }
};

class Derived: public Base
{
public:
    typedef Base super;
    virtual void foo()
    {
        super::foo();   // call superclass implementation

        // do other stuff
        ...
    }
};

class DerivedAgain: public Derived
{
public:
    virtual void foo()
    {
        // Call superclass function
        super::foo();    // oops, calls Base::foo() rather than Derived::foo()

        ...
    }
};
```

(As pointed out by Martin York in the comments to this answer, this problem can be eliminated by making the typedef private rather than public or protected.)

## TODO

http://www.martinbroadhurst.com/typedef-super.html

