# drdobbs [Templates and Duck Typing](https://www.drdobbs.com/templates-and-duck-typing/184401971)

## 前言

The distinction in C++ between **duck typing** and **inheritance** comes from C++'s **static type system**, and is part of the price we pay for having C++ programs run as quickly as they do.

> NOTE: 第一段的意思是: "在C++中，duck typing和inheritance的差异源自于C++的static type system"。
>
> 这篇文章重要强调的是template和duck typing都是behavior-based的

## 正文

C++ offers two kinds of polymorphism—

1) **runtime polymorphism**, which is based on **virtual functions** and is the foundation of **object-oriented programming**, and 

2) **compile-time polymorphism**, which is based on **templates** and is the foundation of **generic programming**. 

When we wish to select from a set of classes at **runtime**, C++ requires that those classes be related by **inheritance**. When we wish to select from a set of types at **compile time**, the relationship between those types is more subtle(难以捉摸的、微妙的). The types need be related only indirectly, and only by their **behavior**.

> NOTE: 在`Theory\Programming-paradigm\Object-oriented-programming\Polymorphism\Polymorphism.md`中，我们已经知道了“[Duck typing](https://en.wikipedia.org/wiki/Duck_typing) for polymorphism without (static) **types**”。上面这一段中描述的使用template实现的polymorphism和使用duck type实现的polymorphism非常类似。

The C++ community does not have a generally accepted term for this kind of **behavior-based relationship** between types. Accordingly, people first learning about C++ generic programming are tempted to think that **inheritance** is involved somehow, just as it is for **object-oriented programming**. For example, on several occasions we have seen questions such as "Why isn't a **bidirectional iterator** derived from a **forward iterator**?" A student who asks that question has probably already formed a significant misconception about how templates deal with types.

> NOTE: 上面这段话的意思是：当人们刚开始学习C++ generic programming 的时候，会和object-oriented programming混淆，OOP是基于inheritance的，即要求“classes be related by **inheritance**”；而generic programming是基于duck type的，即“types need be related only indirectly, and only by their **behavior**”

One way to avoid such misconceptions is to adopt a term for the kind of **type relationships** that we find in generic programs. Giving names to concepts often makes the concepts easier to understand and remember. The **C++ community** does not appear to have such a term, so we would like to borrow a term from the **Python community** and call such relationships "**duck typing**." The idea, of course, is that if it looks like a duck, walks like a duck, and quacks like a duck; then it's a duck.

> NOTE: duck type是典型的基于behavior的。

> NOTE: 证实本段观点的例子：
>
> [cppreference Iterator library](https://en.cppreference.com/w/cpp/iterator)
>
> > Instead of being defined by specific types, each category of iterator is defined by the **operations** that can be performed on it. This definition means that any type that supports the necessary operations can be used as an iterator -- for example, a pointer supports all of the operations required by [*LegacyRandomAccessIterator*](https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator), so a pointer can be used anywhere a [*LegacyRandomAccessIterator*](https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator) is expected.



## Examples

> NOTE: 下面首先列举了OOP的例子，然后列举了generic programming的例子。关于例子的标题是我添加的，原文中并没有。

### OOP

Suppose you have two classes related by inheritance:

```c++
class Employee { /* ... */ };

class Manager: public Employee { /* ... */ };
```

and another class with a member that accepts one of these classes as an argument:

```c++
class Payroll_handler {
public:
// ...
void generate_paycheck(Employee&);
// ...
};
```

You also have an object that represents a manager and another that represents a payroll handler:

```c++
Manager m;
Payroll_handler p;
```

then you expect to be able to generate **m**'s paycheck by executing

```c++
p.generate_paycheck(m);
```

even though the **generate_paycheck** function expects an **Employee**, rather than a **Manager**. Why? Because **Manager** has **Employee** as a base class, so a **Manager** "is-a" **Employee**.

In other words, you know that a function that expects an **Employee&** will accept a **Manager** argument because of **inheritance**.

### Generic programming

Now consider this example:

```c++
int x[100];
std::fill(x, x+100, 42);
```

The call to **std::fill** sets all the elements of **x** to **42**. If you look at the definition of **fill**, you find that it expects its first two arguments to be **forward iterators**. You know that **x** and **x+100** are **forward iterators** because—why?

Unlike the case with **virtual functions**, you can tell that **x** and **x+100** are **forward iterators** only by looking at their behavior in context. In particular, you need to know not only that **x** is a pointer, but also a pointer to an element of an array. Unless a pointer points to an array element, you cannot meaningfully apply **++** to it—an operation that is required of every **forward iterator**.

In other words, if **x** looks like a **forward iterator**, it is a **forward iterator**—regardless of the type that **x** actually has. Claiming that **x** is a **forward iterator** is a prime example of **duck typing**.

> NOTE: 对duck type非常好的解释。

As another example, when the description of a container says that the container's elements must be **assignable** and **copy constructable**, that description is using **duck typing**. It doesn't care what the types actually are; it cares only that they support particular **operations**. It is not always even necessary for a type to support specific operations in a specific way to be considered a particular kind of duck. For example, for an object to be considered an **output iterator**, it is required to support the **++** and **=** operations only in a very restricted form. The **ostream_iterator** classes meet this requirement by making **++** do nothing at all!



As another example, consider the **accumulate** function from the Standard Library. If you call **accumulate(p, q, x)**, the accumulate function initializes a local variable to be a copy of **x**. Let's call that variable **acc**. After initializing **acc**, the accumulate function looks at each iterator **it** in the range **[p, q)** and effectively executes the statement:

```
acc = acc + *it;
```

This execution might take place in more than one way, depending on the types of **acc** and ***it**. For example, **acc** could be of a type that has an **operator+** member. Alternatively, there could be an **operator+** defined separately that accepts, as arguments, values of the types of **acc** and ***it**. The specification of accumulate doesn't care; all it requires is that **acc** and ***it** quack in the right dialect.

> NOTE: 上面这段话的意思是：主要`acc`有成员变量`operator+`或者定义了`operator+(acc, *it)`。

## Useful Ducks

Python takes advantage of duck typing in contexts that C++ programmers may find surprising. For example, the normal Python way of printing the value of an expression on the standard output stream is:

```python
print "Hello, world!"
```

By default, the destination is the standard output stream and the output is followed by a newline. If you want to print the same message on the standard error stream, you do so this way:

```python
import sys
print >>sys.stderr, "Hello, world"
```

So far, these examples don't look much different from their C++ counterparts:

```c++
std::cout << "Hello, world!\n";
```

and

```c++
std::cerr << "Hello, world!\n";
```

The difference is that in C++, **std::cout** and **std::cerr** are objects with **<<** members that, in turn, accept string literals. In Python, what follows the **>>** is an object of any type that happens to have a method named **write**.

Suppose that for some reason, you want to make it easy to write the same text on both the standard output and standard error files. Doing so in Python is easy:

```python
import sys
class DualWriter:
   def write(self, x):
      sys.stdout.write(x)
      sys.stderr.write(x)
```

Now you can create a **DualWriter** object:

```C++
dual = DualWriter()
```

and then whenever you execute

```C++
print >>dual, x
```

the value of **x** appears on both the standard output and standard error streams. Because you know that the **>>** mechanism assumes only the existence of the **write** method, you could define a tiny class that **>>** would accept because of duck typing. Suppose we wanted to do something similar in C++. It might appear at first to be impossible, because **<<** is a member of the **ostream** library classes, and you cannot easily define such a class of your own. However, when you write an expression such as:

```C++
dual << "Hello, world!\n"
```

in C++, you don't actually require **dual** to be a member of the **ostream** hierarchy. It suffices for our purposes that **dual** support a **<<** member that can handle the right types. What are the right types? Whatever it takes to make our class look like a duck.

Here's a start:

```c++
class DualWriter {
public:
   DualWriter(std::ostream& s1, std::ostream& s2):
      s1(s1), s2(s2) { }
   template<class T> DualWriter& operator<<(const T& t) {
      s1 << t;
      s2 << t;
      return *this;
   }
private:
   std::ostream& s1;
   std::ostream& s2;
};
```

We have defined a tiny class named **DualWriter** that encapsulates references to two output streams. When you construct a **DualWriter** object, you say what those streams are. The only other work that a **DualWriter** object will do is to implement a **<<** operator that takes a (**const** reference to an) object of any type and calls each **ostream**'s **<<** operator with that object. In effect, you're saying that as far as the **<<** operation is concerned, a **DualWriter** is the same kind of duck as an **ostream**, whatever kind of duck that might be.

Of course, you can extend this class to support other operations as needed. However, it is useful even in its current sketchy form:

```c++
DualWriter dual(std::cout, std::cerr);
dual << "Hello, world!\n";
```

will say **Hello, world!** on both the standard output and standard error streams.

## Discussion

The distinction in C++ between **duck typing** and **inheritance** comes from C++'s static type system and is part of the price we pay for having C++ programs run as quickly as they do. **Runtime duck typing** is expensive, so C++ doesn't support it. When a C++ program executes **obj.f(x)**, and **obj** is a reference to a base class with a virtual function **f**, C++'s inheritance requirements ensure that **obj** actually refers to an object that has a member function named **f**, and that function's return type has the same internal representation, regardless of which derived class **f** is actually called.



In contrast, **compile-time duck typing** doesn't cost anything during runtime. Indeed, it is **duck typing** that makes it possible for the C++ library to define a single vector template that allows **vector** for any suitable type **T**, rather than requiring **T** to be derived from a class such as **vector_element**. The standard containers require their element types to be "assignable" and "copy constructible," but those notions are just ways of describing particular kinds of ducks. It is these notions' lack of inheritance requirements that lets us use types such as **vector**, even though **int** is not part of any inheritance hierarchy.

