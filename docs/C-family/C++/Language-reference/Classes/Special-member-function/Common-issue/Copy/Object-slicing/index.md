# Object slicing

1、已经多次在其他文章中遇到了object  slicing，本文对它进行总结。

2、object slicing是能够体现value semantic的

## stackoverflow [What is object slicing?](https://stackoverflow.com/questions/274626/what-is-object-slicing)



### [A](https://stackoverflow.com/a/274634)

"Slicing" is where you **assign** an object of a **derived class** to an instance of a **base class**, thereby losing part of the information - some of it is "sliced" away.

For example,

```cpp
class A {
   int foo;
};

class B : public A {
   int bar;
};
```

So an object of type `B` has two data members, `foo` and `bar`.

Then if you were to write this:

```cpp
B b;

A a = b;
```

Then the information in `b` about member `bar` is lost in `a`.

> NOTE: 完整测试程序如下:

```C++
class A
{
	int foo;
};

class B: public A
{
	int bar;
};
int main()
{
	B b;

	A a = b;
}
// g++ -Wall -pedantic -pthread main.cpp && ./a.out


```

### [A](https://stackoverflow.com/a/14461532)

Most answers here fail to explain what the actual problem with slicing is. They only explain the benign cases of slicing, not the treacherous(危险的) ones. Assume, like the other answers, that you're dealing with two classes `A` and `B`, where `B` derives (publicly) from `A`.

In this situation, C++ lets you pass an instance of `B` to `A`'s **assignment operator** (and also to the **copy constructor**). This works because an instance of `B` can be converted to a `const A&`, which is what **assignment operators** and **copy-constructors** expect their arguments to be.

#### The benign case

```cpp
B b;
A a = b;
```

Nothing bad happens there - you asked for an instance of `A` which is a copy of `B`, and that's exactly what you get. Sure, `a` won't contain some of `b`'s members, but how should it? It's an `A`, after all, not a `B`, so it hasn't even *heard* about these members, let alone would be able to store them.

#### The treacherous case

```cpp
B b1;
B b2;
A& a_ref = b2;
a_ref = b1;
//b2 now contains a mixture of b1 and b2!
```

> NOTE: 
>
> 1、上述例子和 wikipedia [Object slicing](https://en.wikipedia.org/wiki/Object_slicing) 中给出的相同，wikipedia [Object slicing](https://en.wikipedia.org/wiki/Object_slicing) 中的例子是完整的，因此此处不对它进行补充

You might think that `b2` will be a copy of `b1` afterward. But, alas(唉（表悲伤、遗憾、恐惧、关切等等）), it's **not**! If you inspect it, you'll discover that `b2` is a Frankensteinian creature, made from some chunks of `b1` (the chunks that `B` inherits from `A`), and some chunks of `b2` (the chunks that only `B` contains). Ouch!

> NOTE: 
>
> 1、结合下面的 wikipedia [Object slicing](https://en.wikipedia.org/wiki/Object_slicing) 来理解上面这段话

What happened? Well, C++ by default doesn't treat **assignment operators** as `virtual`. Thus, the line `a_ref = b1` will call the assignment operator of `A`, not that of `B`. This is because, for **non-virtual functions**, the **declared** (formally: *static*) type (which is `A&`) determines which function is called, as opposed to the **actual** (formally: *dynamic*) type (which would be `B`, since `a_ref` references an instance of `B`). Now, `A`'s assignment operator obviously knows only about the members declared in `A`, so it will copy only those, leaving the members added in `B` unchanged.

> NOTE: 
>
> 1、上面这段话的解释是非常好的，它非常好的说明了static type、dynamic type 和 value semantic、reference semantic

#### A solution

Assigning only to parts of an object usually makes little sense, yet C++, unfortunately, provides no built-in way to forbid this. You can, however, roll your own. 

1、The first step is making the assignment operator *virtual*. This will guarantee that it's always the **actual** type's assignment operator which is called, not the **declared** type's. 

2、The second step is to use `dynamic_cast` to verify that the assigned object has a compatible type. 

3、The third step is to do the actual assignment in a (protected!) member `assign()`, since `B`'s `assign()` will probably want to use `A`'s `assign()` to copy `A`'s, members.

```cpp
class A {
public:
  virtual A& operator= (const A& a) {
    assign(a);
    return *this;
  }

protected:
  void assign(const A& a) {
    // copy members of A from a to this
  }
};

class B : public A {
public:
  virtual B& operator= (const A& a) {
    if (const B* b = dynamic_cast<const B*>(&a))
      assign(*b);
    else
      throw bad_assignment();
    return *this;
  }

protected:
  void assign(const B& b) {
    A::assign(b); // Let A's assign() copy members of A from b to this
    // copy members of B from b to this
  }
};
```

Note that, for pure convenience, `B`'s `operator=` covariantly overrides the return type, since it **knows** that it's returning an instance of `B`.

> NOTE: 
>
> 完整的测试程序如下，这个测试程序是对 wikipedia [Object slicing](https://en.wikipedia.org/wiki/Object_slicing) 中的例子的稍微改写。

```C++
#include <stdexcept>
#include <exception>
#include <string>
#include <iostream>

class bad_assignment: public std::exception
{
	const char* what() const throw ()
	{
		return "bad assignment";
	}
};

class A
{
public:
	int a_var; // 为了测试便利将它放在public
	A(int a) :
					a_var(a)
	{
	}
	virtual ~A() = default;
	virtual A& operator=(const A &a)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		assign(a);
		return *this;
	}

protected:
	void assign(const A &a)
	{
		// copy members of A from a to this
		a_var = a.a_var;
	}
};

class B: public A
{
public:
	int b_var; // 为了测试便利将它放在public
	B(int a, int b) :
					A(a), b_var(b)
	{
	}
	virtual ~B() = default;
	virtual B& operator=(const A &a)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		if (const B *b = dynamic_cast<const B*>(&a))
			assign(*b);
		else
			throw bad_assignment();
		return *this;
	}

protected:
	void assign(const B &b)
	{
		A::assign(b); // Let A's assign() copy members of A from b to this
		// copy members of B from b to this
		b_var = b.b_var;
	}
};

B& getB()
{
	static B b(1, 2);
	return b;
}

int main()
{
	// Normal assignment by value to a
	A a(3);
	//a.a_var == 3
	a = getB();
	// a.a_var == 1, b.b_var not copied to a

	B b2(3, 4);
	//b2.a_var == 3 ,b2.b_var == 4
	A &a2 = b2;

	a2 = getB();
	if (b2.a_var == getB().a_var && b2.b_var == getB().b_var)
	{
		std::cout << "success" << std::endl;
	}
	else
	{
		std::cout << "fail" << std::endl;
	}
	return 0;
}
// g++ --std=c++11 -Wall -pedantic -pthread main.cpp && ./a.out


```

> NOTE: 
>
> 1、上述程序的输出为: 
>
> ```C++
> virtual A& A::operator=(const A&)
> virtual B& B::operator=(const A&)
> success
> ```
>
> 2、通过上述输出可以看出，修改失效了，并且测试通过了，



## wikipedia [Object slicing](https://en.wikipedia.org/wiki/Object_slicing)



```C++
struct A
{
	A(int a) :
					a_var(a)
	{
	}
	int a_var;
};

struct B: public A
{
	B(int a, int b) :
					A(a), b_var(b)
	{
	}
	int b_var;
};

B& getB()
{
	static B b(1, 2);
	return b;
}

int main()
{
	// Normal assignment by value to a
	A a(3);
	//a.a_var == 3
	a = getB();
	// a.a_var == 1, b.b_var not copied to a

	B b2(3, 4);
	//b2.a_var == 3 ,b2.b_var == 4
	A &a2 = b2;
	// Partial assignment by value through reference to b2
	a2 = getB();
	// b2.a_var == 1, b2.b_var == 4!

	return 0;
}
// g++ -Wall -pedantic -pthread main.cpp && ./a.out


```



## TODO

https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Exception


https://isocpp.org/wiki/faq/value-vs-ref-semantics#pass-by-value

在其中也谈及了object slice