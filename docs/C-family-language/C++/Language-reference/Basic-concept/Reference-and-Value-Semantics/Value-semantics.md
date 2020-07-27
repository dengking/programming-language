# Value semantic



## akrzemi1 [Value semantics](https://akrzemi1.wordpress.com/2012/02/03/value-semantics/)

One of these things is *value semantics*. This feature is easily missed by programmers that come into C++ from heavy-OO world, especially that there is no language keyword associated with it. In this post I will try to describe what it is and how it can be useful.

> NOTE: python是典型的仅仅只有reference semantic，在python 4.2. Naming and binding[¶](https://docs.python.org/3/reference/executionmodel.html#naming-and-binding)中，有这样的介绍：
>
> > *Names* refer to objects.
>
> java也是如此。

### What is a value? What is an object?

The concept of a *value* has been exhaustively described in [Elements of Programming](http://www.elementsofprogramming.com/) by Alexander Stepanov and Paul McJones. It is hard to define a value, but we can show the meaning by examples. 

In an algebraic expression of the form *x* ≤ *y*, *x* and *y* are **values**. We may not know what the value *x* is, but we know this value is fixed and will always be the same value, e.g. 2. 

On the other hand, in a piece of code `x <= y`, `x` is not a **value**, because it may be 2 at one time, and 4 just a second later. `x` is an *object* that can hold different **values** at different times. 

> NOTE: `x`是variable，y可以是variable，也可以是value；2、4就是value；

This definition of an object is different than the one from OO world, where object denotes something that has methods and is an instance of a class. In the context of C++ an **object** is a piece of memory that has an **address**, a **type**, and is capable of storing **values**. The same sequence of bits stored in two objects may be interpreted as different **values** if the two objects’ **types** differ; e.g., the sequence of bits `1000001` can be interpreted as number 65 if object’s type is `short int`, or as character ‘A’ if object’s type is `char`.

> NOTE: 上面这段话对variable、value、object的分析与解释非常好，结合在cppreference object中的介绍，可以简单地总结一下：
>
> - object是一片memory
> - not every bit of object participate in value 
> - every object has a type 
> - type determine the interpretion of bits of object, which determine value

The distinction into **values** and **objects** is somewhat similar to the original distinction into *r-values* and *l-values*:

> lval = rval;

An l-value, the one on the left-hand side of the assignment, needs to name an object because we will be changing the **value** the **object** stores. In the case of an r-value, the one on the right-hand side of the assignment, we only require that it names a **value**: we will not change it, we only need to **copy** it. As in this instruction:

> x = 2;

We must be able to query `x` for its address (because under this address we will store a new value), but we do not have to query the literal 2 for address.

### Value semantics

*Value semantics* is the programming style, or the way of thinking, where we focus on **values** that are stored in the **objects** rather than **objects** themselves. The **objects** are only used to convey values: we do not care about object’s identity.

```c++
Complex mulAdd( Complex a, Complex b, Complex c )
{
  Complex ans = a * b + c;
  return ans;
}
 
Complex z = mulAdd(g, h, i); 
```

Here, the computed value of a complex number was stored in automatic object `ans`, then the **value** was (or could be) transferred to a temporary, and then to another automatic object: `z`. In fact, it might not have been this many **objects** because of optimizations known as **copy elision**; but this does not matter at all, because even though we need objects to convey **values**, we do not care about the objects, and how many of them there were, and where they were. We will be only be interested in the computed value.

In contrast to **value semantics**, sometimes we are very interested in the very object, and we want to operate exactly on this **object**, at exactly this **address**:

```c++
ostream & operator<<( ostream & out, Complex c )
{
    return out << c.real << ' ' << c.imag;
}
```

Here, the **stream** may be a global which allocated resources necessary for writing text into terminal, and we need to return the **reference** to the very **same** object. This way of programming style can be called *reference semantics*. However, note that we passed `c` by value, because in that case, again, it is only **value** that matters. In C++ we can mix value and reference semantics.



### Why we use value semantics

#### First

First, you get **value semantics** by default. When declaring function arguments or return values, if you specify only the type name (like `int`) you get **value semantics** (you pass and return *by value*). If you wan to use **reference semantics**, you must make an extra effort to add a reference or pointer type symbol.

#### Second

Second, we use **value semantics** in function declarations, because it closely follows the notation and reasoning from **mathematics**. In mathematics you operate on values. For instance, you define a function as follows:
$$
    f: int × int \to int \\
    f(x, y) = x·x + y·y
$$
This is very similar to:

```c++
int f( int x, int y )
{
  return x * x + y * y;
}
```

Or even a bit more similar with the new function notation:

```c++
auto f( int x, int y ) -> int
{
  return x * x + y * y;
}
```

#### Third

Third, we do not run into any **memory management** issues. No **dangling references** to nonexistent objects, no expensive and unnecessary free store allocation, no memory leaks, no smart or dumb pointers. The support for **value semantics** in C++ — passing variables by value — eliminates all those problems.

#### Fourth

Fourth, we avoid any *reference aliasing* problems. Andrew Koenig has neatly illustrated the problem of **reference aliasing** in [this article](http://drdobbs.com/blogs/cpp/232400151). In multi-threaded environment passing by value and ensuring that each thread has its own copy of the value helps avoid any unnecessary data races. Then you do not need to synchronize on such values, and the program runs faster, and is safer because it avoids any **deadlocks**.

#### Fifth

Fifth, for [referential transparency](https://akrzemi1.wordpress.com/2012/02/03/value-semantics/referential transparency). This means that you get no surprises where your data is modified behind the scenes without you being able to see that. For instance, consider this example from Dave Abrahams:

```c++
Matrix x = y;
y[0][0] = 3.14; // (*)
```

The value of `x` is not modified after the assignment in the second line. The objects are separated from one another. There is no connection between them anymore. This is not so for many languages that enforce OO programming model.

You can also observe **referential transparency** by comparing the following two ways of advancing an STL iterator:

```c++
i = next(i, 1); // (1)
advance(i, 1);  // (2)
```

Function `std::next` does not modify anything. It just takes one iterator pointing to one element and returns another iterator that points to the next element. If you called it three times instead of one it wouldn’t change the behaviour of the program at all:

```c++
i = (next(i, 1), next(i, 1), next(i, 1));
```



In other words, `std::next` does not have any **side effects**. In contrast, function `std::advance` changes the very object it is called with. If we called it three times instead of one, we would alter program behaviour by advancing the **iterator** further and further. Now, what if we wanted to advance the iterator by two? Forget for a moment that in each case you can pass 2 as the second argument to do that, and for the example’s sake let’s do it with 1s. In the **value semantic** approach we compose function calls:

```c++
i = next(next(i)); 
```

Using reference semantics and imperative approach, we repeat the commands:

```c++
advance(i, 1);
advance(i, 1);
```

The **value-semantic** approach isn’t entirely pure (in functional programming sense) because in the end we do modify `i` with an assignment. But it somehow separates the pure parts from impure ones.



> NOTE: 上述对比了[`next`](https://en.cppreference.com/w/cpp/iterator/next) 、[`advance`](https://en.cppreference.com/w/cpp/iterator/advance) ；
>
> `next`是value semantic
>
> `advance`是reference semantic

### Why we do not use value semantics

#### First

Whether we pass **by value** or **by reference** often depends on a programming style. For one instance, if you think in OO terms, where you want to create an object, and let it live for a significant amount of time, and want different parties to modify the object, you need to pass **by reference**. This is the case for globals like `std::cout` that need to be accessible from different places in the code, and it always has to be this very object at this very address. In other words if this is the address of the object that you are interested it, **value semantics** will not do — even if you are not operating on addresses explicitly.

Since we mention addresses, this is the right place to mention pointers here. They are very peculiar（特殊的）, because they are objects that store values. However, the values they store are addresses in memory which can hardly be used for anything else than *referring* to objects. Therefore at the higher level of abstraction passing and returning pointers to and from functions is usually considered passing by reference (reference semantics), even though technically you are **passing values**. But note that there is a limit to the analogy between **pass-by-pointer** and **pass-by-reference**, as nicely outlined in [this article](http://javadude.com/articles/passbyvalue.htm).

#### Second

Next, the reason people often resort to allocating objects in free store (on the heap) and returning a (more or less) smart pointer to it, is that value semantics does not play well with OO techniques that require access to objects via references or pointers to enable **virtual function calls**. It is also not easy to pass polymorphic types by value, because we are risking [slicing](http://en.wikipedia.org/wiki/Object_slicing). There are techniques to overcome this difficulty, like [type erasure](https://akrzemi1.wordpress.com/2013/11/18/type-erasure-part-i/), but they require some non-trivial effort to implement. They are used in `std::function`, `std::shared_ptr` (for storing allocator). There are libraries that assist with implementing type erasure like, well, [type_erasure](https://github.com/boost-vault/Miscellaneous/blob/master/type_erasure.zip) or [Adobe Poly Library](http://stlab.adobe.com/group__poly__related.html).

#### Third

One other common reason for choosing not to use **value semantics** is the fear for performance hit. Is this fear justified? The very short answer is: sometimes it is, sometimes it isn’t and sometimes it is avoiding **value semantics** that hits your performance.



### Performance of value semantics

#### Return-by-value

C++ encourages **value semantics**. The language is designed to make **value semantics** as useful and as fast as possible. Passing by value may be faster than you think. Let’s consider this example:

```c++
vector<string> getPoems()
{
  vector<string> ans;
  // fill ans;
  return ans;
}
 
vector<string> poems = getPoems();
```

How many times does the copy constructor of `vector<string>` get called? The answer is: not even once! This is because we do not require the two copies of the value — we only want to hand the value over. This is where the [move constructor](https://akrzemi1.wordpress.com/2011/08/11/move-constructor/) is chosen. Move constructor for a typical vector implementation costs six pointer assignments regardless of the types being stored or the vector’s size. So, let’s rephrase the question: how many times the move constructors gets called? In efficient compiler with all global optimizations enabled: most probably, not even once! This is owing to the compiler optimization technique known as [copy elision](http://definedbehavior.blogspot.com/2011/08/value-semantics-copy-elision.html).

#### Pass-by-value

Now, how about passing the arguments by value? First, let’s consider the situation where in the function implementation you need to make the copy of the argument. It is often the case when we implement the copy assignment and want to provide strong (commit-or-rollback) guarantee. In such cases **passing by value**, and not making the **local copy** is more efficient in general and equally efficient as passing by reference in the worst case. This is very well described by Dave Abrahams in [this article](http://web.archive.org/web/20140113221447/http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/).

In the other case, where only want to query the given value for some attributes, **passing by reference** is more efficient that **passing by value** for types with expensive copy constructors.

```c++
int remainingRoom( Dictionary const& d ) 
{
  return d.maxRoom() - d.size(); 
}
 
void fun()
{
  Dictionary d = buildDictionary(); // cheap by-value
  int remaining = remainingRoom(d); 
  use(d);
}
```







### Comments



Hi Gunnar, it looks we are getting somewhere. I am trying to identify at which point we really disagree (because we seem to agree on many things). It looks like you claim that “value semantics is completely orthogonal to polymorphism” and I claim that there is a tension between value semantics and run-time polymorphism (based on virtual table look-up). Agreed?

Now, let me go through your post:

> Polymorphism comes from the fact that each object has a hidden piece of data which points to the Virtual Table that is created for each class. This is the key to polymorphic behavior.

I agree so far, although “this is the key” seems a bit imprecise: having a virtual table pointer is a way of implementing run-time polymorphism but it is not a polymorphism per se. C++ does not require that compilers provide virtual tables if they can implement the same polymorphic behaviour by other means. (but in practise it is always virtual tables, I guess).

> It’s there regardless of whether you access the object with value semantics, reference semantics or pointer semantics.

If by “it” you mean “a hidden piece of data which points to the Virtual Table” than, this I agree that we would find “it” in a newly created object, or in an object referred to by a named reference, or in an object pointed to by a pointer.

> That’s why I say that value semantics is completely orthogonal to polymorphism and therefore, OO.

This is where I disagree with you. Only having “a hidden piece of data which points to the Virtual Table” does not mean using polymorphism. In fact, you have shown a good example the last time how one can create a polymorphic type and then choose not to use this polymorphism. Of course the example was very scarce, so I can only guess how you defined your types (sorry if I guessed wrong) but here is how I imagine you define your classes: