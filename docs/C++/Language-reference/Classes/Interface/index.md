# Interface

推荐先阅读`The-interface-principle.md`，以获得对C++中interface的全面了解: C++中，interface不仅仅局限于OOP，它的ADL，使得interface可以拓展到非method。

## CppCoreGuidelines [I: Interfaces](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-interfaces)



## stackoverflow [How do you declare an interface in C++?](https://stackoverflow.com/questions/318064/how-do-you-declare-an-interface-in-c)

### [A](https://stackoverflow.com/a/318084)

Make a class with **pure virtual methods**. Use the interface by creating another class that overrides those virtual methods.

A pure virtual method is a class method that is defined as virtual and assigned to 0.

```cpp
class IDemo
{
    public:
        virtual ~IDemo() {} //将此函数声明成了
        virtual void OverrideMe() = 0;
};

class Child : public IDemo
{
    public:
        virtual void OverrideMe()
        {
            //do stuff
        }
};
```

### [A](https://stackoverflow.com/a/318137)

To expand on the answer by [bradtgmurray](https://stackoverflow.com/questions/318064/how-do-you-declare-an-interface-in-c#318084), you may want to make one exception（例外） to the pure virtual method list of your interface by adding a **virtual destructor**. This allows you to pass **pointer ownership** to another party without exposing the concrete derived class（这句话解释了将destructor声明为`virtual`和`inline`的目的：这允许您将指针所有权传递给另一方，而不会暴露具体的派生类）. The destructor doesn't have to do anything, because the interface doesn't have any concrete members. It might seem contradictory（自相矛盾） to define a function as both `virtual` and `inline`, but trust me - it isn't.

```cpp
class IDemo
{
    public:
        virtual ~IDemo() {}
        virtual void OverrideMe() = 0;
};

class Parent
{
    public:
        virtual ~Parent();
};

class Child : public Parent, public IDemo
{
    public:
        virtual void OverrideMe()
        {
            //do stuff
        }
};
```

You don't have to include a body for the **virtual destructor**(因为它是`inline`的) - it turns out some compilers have trouble optimizing an empty destructor and you're better off using the default.

