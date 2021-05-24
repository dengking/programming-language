# [Guidelines For Rvalue References In APIs](https://foonathan.net/blog/2018/03/26/rvalue-references-api-guidelines.html)

I’ll be giving a talk at [ACCU](https://conference.accu.org/2018/sessions.html#XIjustwantedtopointtosomething) about when to use which pointer types and why.

While working on that I made some guidelines for rvalue references in interfaces which didn’t quite fit the talk, so I’m writing about them here.

When should you use **rvalue references** as function parameters?

When as return types?

What are ref-qualified member functions and when and how should you use them?

Let’s tackle it one by one.



**TODO**



## Rvalue Reference Parameters

Some functions take an rvalue reference:

```c++
void foo(T&& param);
```

Note that this function does not take an rvalue reference:

```c++
template <typename T>
void foo(T&& param);
```

Because `T` is a template parameter of the function, different rules kick in(起作用) transforming it into a *forwarding reference*. This is a hack used to fulfill a completely different purpose, perfect forwarding. Read more about it [here](https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers), for example.

> Note: They’re still called “universal references” in that post; they have been renamed since.

So why do functions take an rvalue reference?

I’ve identified three use cases: one common, one rare and one experimental that may be useful.

### 1. Rvalue + Lvalue Reference Parameter Combination For Performance

The best example of this is `std::vector<T>::push_back()`:

```
void push_back(const T& obj);
void push_back(T&& obj);
```

> Note that even though `T` is a template parameter it is a template parameter of the *class*, not the `push_back()`function. This means that `T&&` is still an rvalue reference.

When you have a parameter where you want to store in a different place, a good guideline is to pass that parameter by value and move it into the final location.

The classical example of it is a constructor:

```
explicit person(std::string name)
: name_(std::move(name))
{}
```

That way when you pass an lvalue, you pay for a copy (into the parameter) and a move (into the final location), and when you pass an rvalue, you pay for a move (into the parameter) and a move (into the final location).

Taking a `const T&` would have a copy (into the final location) for both lvalues and rvalues, and passing `T&&` only would prevent passing lvalues. So with `T` both works and because moves are [generally](https://foonathan.net/blog/2017/09/14/destructive-move.html)cheap, there isn’t much downside.

However the perfect solution would copy lvalues and move rvalues, so you don’t have to pay for the additional move from the parameter into the final location. In order to achieve that, you have to overload your function for both lvalues and rvalues.

But this leads to `2^n` duplicated functions, where `n` is the number of arguments, so you should only do it when:

- Performance of this piece of code absolutely matters.
- You don’t know the types you’re dealing with (generic code) or they are expensive to move (i.e. because they don’t have a move constructor).
- You have only few parameters.

With the `person` example, performance is probably not critical and we know that `std::string` is cheap to move, so there is no need to duplicate the constructor.

But with `std::vector<T>::push_back()` all three conditions are true, which is why the two overloads exist.

### 2. Rvalue Reference Parameter For Conditional Moving

Move is not just an optimization of copy, it has an important semantic meaning — transferring ownership.

Consider a function that wants to take ownership from a `std::unique_ptr`. In order to do that it needs to move from the argument.

This gives three options for the parameter type:

- `std::unique_ptr<T>&`
- `std::unique_ptr<T>`
- `std::unique_ptr<T>&&`

Taking an lvalue reference is a bad idea: it is not obvious for the caller that the pointer is going to be in the moved-from state after the function returns. Also the function cannot take real rvalues — temporaries.

Taking it by value works as now the caller has to write `std::move()` when moving an existing an object (lvalue). This comes with an extra move operation again, but this is usually negligible.

An rvalue reference parameter is the same for the caller, but without the extra move internally, so it seems superior. But consider this function:

```
void foo(std::unique_ptr<T>&&) { /* do nothing */ }
```

This function will not actually move from the argument, so it will stay the same after the call!

An rvalue reference parameter here thus has slightly different semantics: it allows for *conditional*moving. Maybe the function will take ownership, maybe not.

However, this use case is pretty rare.

### 3. Rvalue Reference Parameters For Forcing Move

An rvalue reference parameter has an additional difference to a by value parameter: It forces the caller to use `std::move()` for lvalues, even if the type is not really moveable!

```
void foo(int&& i);
…
auto i = 42;
foo(i); // error: i is an lvalue
foo(std::move(i)); // okay
```

But this can be used for our advantage: In a new project I’m working I have the following (simplified) piece of code.

```
// some block of memory
struct memory_block
{
    std::byte* address;
    std::size_t size;
};

memory_block new_block(std::size_t size)
{
    // allocate block using ::operator new
}

void delete_block(memory_block&& block)
{
    // deallocate block using ::operator delete
}
```

Note that `delete_block()` takes the `memory_block` by rvalue reference — even though it is a trivially copyable type. But forcing the caller to write `delete_block(std::move(block))` makes it really obvious that the memory block is being turned unusable.

I’ve only started using this recently, so I don’t have much experience with it. But I encourage you to experiment.

## Rvalue Ref-qualified Member Functions

C++11 added a relatively obscure feature, ref-qualifiers for `*this`.

> With “obscure” I mean: if you’re not writing libraries, you’ve probably haven’t heard of it.

You could always qualify member functions with `const`, to allow them being called on `const` objects.

> You could also always qualify them with `volatile`, but … `volatile`.

Similar, in C++11 you can qualify them with `&` and `&&` as well. Due to backwards compatibility, they have slightly different rules:

- A member function qualified with `&` cannot be called on rvalues (unless it is also qualified with `const`).
- A member function qualified with `&&` cannot be called on lvalues.
- A member function not qualified can be called on lvalues and rvalues.

Usually, you don’t have just a single member function qualified with `&&`, for example, but have multiple overloads with the different qualification. The combination of overloads depends on the use case.

### 1. Passing The Value Category Along In A Getter Function

Consider `std::optional<T>`: It (maybe) contains an object of type `T` and provides access to it with `.value()`.

> It also has `operator*()`, but I always pretend it doesn’t exist.

Like usually, it gives you a `const T` when you call it on a `const` object, and a non-`const` `T` when you call it on a non-`const` object:

```
std::optional<int> opt(42);
opt.value() = 43; // okay

const std::optional<int> opt(42);
opt.value() = 43; // error
```

And - you’ve guessed it - it gives you a `T&&` when called on an rvalue and a `T&` when called on an lvalue.

Why was that done?

Consider this:

```
std::optional<std::string> make();

…

std::string str = make().value();
```

Here `make()` returns an rvalue optional, so it is safe to move the string out. And this is exactly what is done, thanks to the rvalue qualified `value()` that is being called!

In order to achieve this use case, there are four overloads of `std::optional<T>::value()`, one for each combination of `const` and rvalue-ness:

```
// assume a `get_pointer()` function that returns a pointer to the object being stored

T& value() & // non-const lvalue
{
    return *get_pointer();
}

T&& value() && // non-const rvalue
{
    return std::move(*get_pointer()); // propagate rvalue-ness
}

const T& value() const & // const lvalue
{
    return *get_pointer();
}

const T&& value() const && // const rvalue
{
    return std::move(*get_pointer()); // propagate rvalue-ness
}
```

> If you think “the last combination is somewhat useless” - you’re right. But not having it [breaks weird code](https://cplusplus.github.io/LWG/issue2485).

While this use-case is cool, unless you’re writing types like `std::optional`, you probably don’t need it.

### 2. Optimize Member Functions

But maybe a more general idea is useful: You have a `const` member function, that does some expensive calculation, maybe it needs to copy internal state for the result:

```
expensive_state foo(int arg) const
{
    expensive_state copy = my_state_;
    do_sth(copy, arg); 
    return copy;
}
```

An rvalue qualified overload can get rid of the copy by using the internal state directly - after all the object is a temporary and/or no longer needed, so no need to keep it intact:

```
expensive_state&& foo(int arg) &&
{
    do_sth(my_state_, arg);
    return std::move(my_state_);
}
```

When optimizing certain functions for rvalues, you usually have two overloads: one `const` and one `&&`qualified. Make sure the optimization is worth the code duplication though!

### 3. Prevent Dangling References

I’ve talked about it in detail in [this blog post](https://foonathan.net/blog/2017/03/22/string_view-temporary.html): if `std::string_view` is used outside of a function parameter, be very careful!

For example, consider this simple getter:

```
std::string_view get_name() const
{
    return name_;
}
```

Over time it is decided that the name should be split into first and last name.

> Even though [this is not a good idea](https://www.kalzumeus.com/2010/06/17/falsehoods-programmers-believe-about-names/).

In a late night refactor the getter is changed:

```
std::string_view get_name() const
{
    return first_name_ + last_name_;
}
```

This blows up, because now we’ve created a view to a temporary string!

One way this could have been prevented is by disabling the conversion to `std::string_view` for rvalues. Right now we have this:

```
std::string::operator std::string_view() const { return …; }
```

> Ignoring the templates here.

By adding a second overload, we can prevent the use for temporaries:

```
std::string::operator std::string_view() const & { return …; }
std::string::operator std::string_view() const && = delete;
```

That way, overload resolution will select this overload when called on an rvalue, then issue an error because the function is deleted.

Read more about deleting functions in [my series about controlling overload resolution](https://foonathan.net/blog/2015/10/16/overload-resolution-1.html).

### 4. Mark One Time Operations that Destroy the Objects

I recently have a lot of immutable objects, so I use the builder pattern a lot:

```
class Foo
{
public:
    class Builder
    {
    public:
        Builder() = default; 

        void set_bar(Bar b) { … }

        void set_baz(Baz b) { … }

        Foo&& finish()
        {
            return std::move(obj);
        }

    private:
        Foo obj_;
    };

private:
    Foo() = default;
};
```

Note the `finish()` function: once the object is done it will be moved out. But this will destroy the builder, i.e. you can’t use it again anymore.

Granted, with a method name `finish()` it may be obvious. But still, qualify the member function with `&&`:

```
Foo&& finish() &&
{
    return std::move(obj);
}
```

Then you will be reminded that the object is turned unusable:

```
auto obj = builder.finish(); // error!
auto obj2 = std::move(builder).finish(); // okay
```

## Rvalue References As Return Types

When using rvalue references as return types, returning temporaries or function local variables still creates a dangling reference, just like with lvalue references.

So it is only really applicable if you’re returning something like member variables or reference parameters and you’re moving them out of place.

There is only one function that moves a reference parameter out: `std::move`.

But we’ve already seen a couple of function that move member variables out: The recent `Builder::finish()` as well as the `optional::value()` for rvalues. Both of those could return it by value, so should they use an rvalue reference?

It’s complicated.

Returning an rvalue in the `optional::value()` case ensures that `decltype()` is consistent with the other cases. However, it can lead to dangling references if you do something like this:

```
optional<T> foo();

auto&& val = foo().value();
```

The temporary returned by the function is destroyed, so `val` references the destroyed member variable. However, if `value()` returned a `T` by value, lifetime extension would ensure that the reference is still valid.

On the other hand, returning by reference may save one extra move.

So should you use rvalue return types?

I’d say you should only do it if you have an rvalue qualified member function but think about the consequences.

## Rvalue References As Class Members

Don’t put references into classes, it will make it hard to write an `operator=`.

Instead store a pointer — or better yet — something which is never null like my `type_safe::object_ref<T>`.

## Conclusion

Use rvalue references as function parameters for conditional moving, to force callers to write `std::move()`, and — together with a `const T&` overload — for more efficient input parameters.

Use ref-qualified member functions to mirror value categories in getters, optimize member functions, prevent operations on temporaries, and to mark member functions that destroy the object’s internal state.

Be careful about rvalue reference return types and don’t put them into classes.



*This post was made possible by my Patreon supporters. If you'd like to support me as well, please head over to my Patreon and do so! One dollar per month can make all the difference.*