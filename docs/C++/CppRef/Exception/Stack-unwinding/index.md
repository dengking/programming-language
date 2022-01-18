# Stack unwinding

"stack unwind"是C++的运行机制之一，参见

1、 `Call-stack`

2、`Abstract-machine\Execution-model`

在cppreference中，stack unwinding特指由于exception而导致的函数提前退出，关于此，参见:

1、cppreference [Destructors](https://en.cppreference.com/w/cpp/language/destructor) # [Exceptions](https://en.cppreference.com/w/cpp/language/destructor#Exceptions)



## stackoverflow [What is stack unwinding?](https://stackoverflow.com/questions/2331316/what-is-stack-unwinding)

> note: 解答地非常好；和我的印象是一致的。其中提RAII、exception safety。

[A](https://stackoverflow.com/a/2332865)

All this relates to C++:

**Definition**: As you create objects statically (on the stack as opposed to allocating them in the heap memory) and perform function calls, they are "stacked up".

When a **scope** (anything delimited by `{` and `}`) is exited (by using `return XXX;`, reaching the end of the scope or throwing an exception) everything within that scope is destroyed (**destructors** are called for everything). **This process of destroying local objects and calling destructors is called stack unwinding.**

> NOTE: 上述C++ stack unwind的介绍是非常好的

You have the following issues related to stack unwinding:

1) avoiding memory leaks (anything dynamically allocated that is not managed by a local object and cleaned up in the destructor will be leaked) - see RAII [referred to](https://stackoverflow.com/questions/2331316/what-is-stack-unwinding/2331413#2331413) by Nikolai, and [the documentation for boost::scoped_ptr](http://www.boost.org/doc/libs/1_41_0/libs/smart_ptr/scoped_ptr.htm) or this example of using [boost::mutex::scoped_lock](http://www.google.com/codesearch/p?hl=en#so90T49b54s/przyklady/cpprec.zip|g3JV5CE7tsM/12-02.cpp&q=mutex::scoped_lock).

2) program consistency: the C++ specifications state that you should never throw an exception before any existing exception has been handled(在existing exception被处理之前，不要throw an exception). This means that **the stack unwinding process should never throw an exception** (either use only code guaranteed not to throw in destructors, or surround everything in destructors with `try {` and `} catch(...) {}`).

> NOTE: 上面这一段说明了"A destructor may not fail"的原因，

If any destructor throws an exception during stack unwinding you end up in the *land of undefined behavior* which could cause your program to terminate unexpectedly (most common behavior) or the universe to end (theoretically possible but has not been observed in practice yet).

[A](https://stackoverflow.com/a/2331413)

**Stack unwinding** is usually talked about in connection with exception handling. Here's an example:

```cpp
void func( int x )
{
    char* pleak = new char[1024]; // might be lost => memory leak
    std::string s( "hello world" ); // will be properly destructed

    if ( x ) throw std::runtime_error( "boom" );

    delete [] pleak; // will only get here if x == 0. if x!=0, throw exception
}

int main()
{
    try
    {
        func( 10 );
    }
    catch ( const std::exception& e )
    {
        return 1;
    }

    return 0;
}
```

Here memory allocated for `pleak` will be lost if an exception is thrown, while memory allocated to `s` will be properly released by `std::string` destructor in any case. The objects allocated on the stack are "unwound" when the scope is exited (here the scope is of the function `func`.) This is done by the compiler inserting calls to destructors of automatic (stack) variables.

Now this is a very powerful concept leading to the technique called [RAII](http://en.wikipedia.org/wiki/RAII), that is ***Resource Acquisition Is Initialization\***, that helps us manage resources like memory, database connections, open file descriptors, etc. in C++.

Now that allows us to provide [exception safety guarantees](http://www.gotw.ca/gotw/056.htm).

[A](https://stackoverflow.com/a/2331351)

In a general sense, a stack "unwind" is pretty much synonymous with the end of a function call and the subsequent popping of the stack.

However, specifically in the case of C++, stack unwinding has to do with how C++ calls the destructors for the objects allocated since the started of any code block. Objects that were created within the block are deallocated in reverse order of their allocation.

## bogotobogo [STACK UNWINDING - 2020](https://www.bogotobogo.com/cplusplus/stackunwinding.php)



## Exception and stack unwind

参见`./Exception-and-stack-unwind`。

