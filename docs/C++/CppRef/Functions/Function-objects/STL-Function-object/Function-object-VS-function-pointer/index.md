# Function object VS function pointer

一、STL更加推崇使用function object，原因如下:

1、type safety

相比之下，function object是能够实现static type  safety的，而function pointer是无法实现static type safety的

2、performance

3、policy-based design

## stackoverflow [What are C++ functors and their uses?](https://stackoverflow.com/questions/356950/what-are-c-functors-and-their-uses)

[A](https://stackoverflow.com/a/356993)

A *functor* is pretty much just a *class* which defines the `operator()`. That lets you create **objects** which "look like" a function:

```cpp
// this is a functor
struct add_x {
  add_x(int x) : x(x) {}
  int operator()(int y) const { return x + y; }

private:
  int x;
};

// Now you can use it like this:
add_x add42(42); // create an instance of the functor class
int i = add42(8); // and "call" it
assert(i == 50); // and it added 42 to its argument

std::vector<int> in; // assume this contains a bunch of values)
std::vector<int> out(in.size());
// Pass a functor to std::transform, which calls the functor on every element 
// in the input sequence, and stores the result to the output sequence
std::transform(in.begin(), in.end(), out.begin(), add_x(1)); 
assert(out[i] == in[i] + 1); // for all i
```

There are a couple of nice things about **functors**. One is that unlike regular functions, they can contain **state**. The above example creates a function which adds 42 to whatever you give it. But that value 42 is not **hardcoded**, it was specified as a constructor argument when we created our functor instance. I could create another adder, which added 27, just by calling the constructor with a different value. This makes them nicely customizable.

As the last lines show, you often pass **functors** as arguments to other functions such as `std::transform` or the other standard library algorithms(这段话描述了functor的通常用法). You could do the same with a **regular function pointer** except, as I said above, **functors** can be "customized" because they contain state, making them more flexible (If I wanted to use a function pointer, I'd have to write a function which added exactly 1 to its argument. The functor is general, and adds whatever you initialized it with), and they are also potentially more efficient. In the above example, the **compiler** knows exactly which function `std::transform` should call. It should call `add_x::operator()`. That means it can `inline` that function call. And that makes it just as efficient as if I had manually called the function on each value of the vector.

If I had passed a function pointer instead, the compiler couldn't immediately see which function it points to, so unless it performs some fairly complex global optimizations, it'd have to dereference the pointer at runtime, and then make the call.



### [Why override operator()?](https://stackoverflow.com/questions/317450/why-override-operator)

[A](https://stackoverflow.com/a/317528)

One of the primary goal when overloading `operator()` is to create a *functor*. A *functor* acts just like a function, but it has the advantages that it is **stateful**, meaning it can keep data reflecting its state between calls.

Here is a simple functor example :

```C++
struct Accumulator
{
    int counter = 0;
    int operator()(int i) { return counter += i; }
}
...
Accumulator acc;
cout << acc(10) << endl; //prints "10"
cout << acc(20) << endl; //prints "30"
```

**Functors** are heavily used with **generic programming**. Many STL algorithms are written in a very general way, so that you can plug-in your own function/functor into the algorithm. For example, the algorithm `std::for_each` allows you to apply an operation on each element of a range. It could be implemented something like that :

```C++
template <typename InputIterator, typename Functor>
void for_each(InputIterator first, InputIterator last, Functor f)
{
    while (first != last) f(*first++);
}
```

You see that this algorithm is very generic since it is parametrized by a function. By using the `operator()`, this function lets you use either a **functor** or a **function pointer**. Here's an example showing both possibilities :

```C++
void print(int i) { std::cout << i << std::endl; }
...    
std::vector<int> vec;
// Fill vec

// Using a functor
Accumulator acc;
std::for_each(vec.begin(), vec.end(), acc);
// acc.counter contains the sum of all elements of the vector

// Using a function pointer
std::for_each(vec.begin(), vec.end(), print); // prints all elements
```

------

Concerning your question about `operator()` overloading, well yes it is possible. You can perfectly write a **functor** that has several parentheses operator, as long as you respect the basic rules of method overloading (e.g. overloading only on the return type is not possible).

