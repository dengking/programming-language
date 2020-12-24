# Scope resolution operator



## cppreference [Scope resolution operator](https://en.cppreference.com/w/cpp/language/identifiers#Qualified_identifiers) 



## microsoft [Scope resolution operator: `::`](https://docs.microsoft.com/en-us/cpp/cpp/scope-resolution-operator?view=msvc-160&viewFallbackFrom=vs-2017)



## stackoverflow [Using :: in C++](https://stackoverflow.com/questions/15649580/using-in-c)

[A](https://stackoverflow.com/a/15649711)

The `::` are used to dereference scopes.



## Use a name in global namespace

在 cppreference [Scope resolution operator](https://en.cppreference.com/w/cpp/language/identifiers#Qualified_identifiers) 中有如下说明:

> The expression `::tolower` names the function tolower in the global namespace. 

在 cppreference [new expression # Allocation](https://en.cppreference.com/w/cpp/language/new#Allocation) 中有如下说明:

> As described in [allocation function](https://en.cppreference.com/w/cpp/memory/new/operator_new), the C++ program may provide global and class-specific replacements for these functions. If the new-expression begins with the optional :: operator, as in ::new T or ::new T[n], class-specific replacements will be ignored (the function is [looked up](https://en.cppreference.com/w/cpp/language/lookup) in global [scope](https://en.cppreference.com/w/cpp/language/scope)). Otherwise, if `T` is a class type, lookup begins in the class scope of `T`.

在 cppreference [Qualified name lookup](https://en.cppreference.com/w/cpp/language/qualified_lookup) 中有如下说明:

> If there is nothing on the left hand side of the `::`, the lookup considers only declarations made in the **global namespace scope** (or introduced into the global namespace by a [using declaration](https://en.cppreference.com/w/cpp/language/namespace)). This makes it possible to refer to such names even if they were hidden by a local declaration

其中给出的例子非常好。

### stackoverflow [What does the “::” mean in “::tolower”?](https://stackoverflow.com/questions/5270780/what-does-the-mean-in-tolower)

I've seen code like this:

```C++
std::string str = "wHatEver";
std::transform(str.begin(), str.end(), str.begin(), ::tolower);
```

And I have a question: what does mean **`::`** before tolower?

and `std::tolower` not works, but `::tolower` works OK

[A](https://stackoverflow.com/a/5270813)

Means that it is explicitly using the `tolower` in the global namespace (which is presumably the stdc lib one).

> NOTE: disambiguation(消除歧义)

Example:

```C++
void foo()
{
	// This is your global foo
}

namespace bar
{
void foo()
{
	// This is bar's foo
}
}

using namespace bar;

void test()
{
	foo();   // Ambiguous - which one is it?
	::foo(); // This is the global foo()
}
int main()
{
	test();

}
// g++ test.cpp

```

> NOTE: 上述代码编译报错如下:
>
> ```C++
> test.cpp: In function ‘void test()’:
> test.cpp:18:6: error: call of overloaded ‘foo()’ is ambiguous
>   foo();   // Ambiguous - which one is it?
>       ^
> test.cpp:18:6: note: candidates are:
> test.cpp:1:6: note: void foo()
>  void foo()
>       ^
> test.cpp:8:6: note: void bar::foo()
>  void foo()
> 
> ```
>
> 