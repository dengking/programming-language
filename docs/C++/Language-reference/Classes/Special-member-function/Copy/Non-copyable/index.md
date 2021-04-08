# Non-copyable 

## Example of non-copyable

参见 `Copyalbe-and-movable` 章节。

## ariya [C++ Class and Preventing Object Copy](https://ariya.io/2015/01/c-class-and-preventing-object-copy)

In some cases, an instance of a C++ class should not be copied at all. There are three ways to prevent such an [object copy](http://en.wikipedia.org/wiki/Object_copy): 

1 keeping the copy constructor and assignment operator private, 

2 using a special non-copyable mixin, or 

3 deleting those special member functions.

### Noncopyable example: stream of file

A class that represents a wrapper stream of a file should not have its instance copied around. It will cause a confusion in the handling of the actual I/O system. In a similar spirit, if an instance holds a unique private object, copying the pointer does not make sense. A somehow related problem but not necessarily similar is the issue of [object slicing](http://en.wikipedia.org/wiki/Object_slicing).

### Noncopyable example: class `Vehicle`

The following illustration demonstrates a simple class `Vehicle` that is supposed to have a unique owner, an instance of `Person`.

```C++
#include <string>
#include <iostream>
struct Person
{
	std::string name;
};

class Car
{
public:
	Car() :
					owner()
	{
	}
	void setOwner(Person *o)
	{
		owner = o;
	}
	Person* getOwner() const
	{
		return owner;
	}
	void info() const
	{
		if (owner)
		{
			std::cout << "Owner is " << owner->name << std::endl;
		}
		else
		{
			std::cout << "This car has no owner." << std::endl;
		}
	}
private:
	Person *owner;
};

int main()
{
	Person joe;
	joe.name = "Joe Sixpack";

	Car sedan;
	sedan.setOwner(&joe);
	sedan.info();
	Car anotherSedan = sedan;
	anotherSedan.info();
}

```

will give the output:

```
Owner is Joe Sixpack
Owner is Joe Sixpack
```

How can we prevent this accidental object copy?

### Method 1: Private copy constructor and copy assignment operator



```C++
#include <string>
#include <iostream>
struct Person
{
	std::string name;
};

class Car
{
public:
	Car() :
					owner()
	{
	}
	void setOwner(Person *o)
	{
		owner = o;
	}
	Person* getOwner() const
	{
		return owner;
	}
	void info() const
	{
		if (owner)
		{
			std::cout << "Owner is " << owner->name << std::endl;
		}
		else
		{
			std::cout << "This car has no owner." << std::endl;
		}
	}
private:
	Car(const Car&);
	Car& operator=(const Car&);
	Person *owner;
};

int main()
{
	Person joe;
	joe.name = "Joe Sixpack";

	Car sedan;
	sedan.setOwner(&joe);
	sedan.info();
	Car anotherSedan = sedan;
	anotherSedan.info();
}

```

### Method 2: Non-copyable mixin

The idea above can be extended to create a dedicated class which has the sole purpose to prevent object copying. It is often called as [Noncopyable](http://en.wikibooks.org/wiki/More_C++_Idioms/Non-copyable_Mixin) and typically used as a [mixin](http://en.wikipedia.org/wiki/Mixin). In our example, the `Car` class can then be derived from this `Noncopyable`.

[Boost](http://www.boost.org/) users may be already familiar with [boost::noncopyable](http://www.boost.org/doc/libs/master/libs/core/doc/html/core/noncopyable.html), the Boost flavor of the said mixin. A conceptual, self-contained implementation of that mixin will resemble something like the following:

```C++
#include <string>
#include <iostream>
struct Person
{
	std::string name;
};

class NonCopyable
{
protected:
	NonCopyable()
	{
	}
	~NonCopyable()
	{
	}
private:
	NonCopyable(const NonCopyable&);
	NonCopyable& operator=(const NonCopyable&);
};

class Car: private NonCopyable
{
public:
	Car() :
					owner()
	{
	}
	void setOwner(Person *o)
	{
		owner = o;
	}
	Person* getOwner() const
	{
		return owner;
	}
private:
	Person *owner;
};

```



Compared to the first method, using `Noncopyable` has the [benefit](http://stackoverflow.com/questions/7823990/what-are-the-advantages-of-boostnoncopyable) of making the intention very clear. A quick glance at the class, right on its first line, and you know right away that its instance is not supposed to be copied.

### Method 3: Deleted copy constructor and copy assignment operator

For modern applications, there is less and less reason to get stuck with the above workaround. Thanks to C++11, the solution becomes magically simple: just [delete](http://en.wikipedia.org/wiki/C%2B%2B11#Explicitly_defaulted_and_deleted_special_member_functions) the **copy constructor** and **assignment operator**. Our class will look like this instead:

```cpp
class Car {
public:
  Car(const Car&) = delete;
  void operator=(const Car&) = delete;
  Car(): owner() {}
  void setOwner(Person *o) { owner = o; }
  Person *getOwner() const { return owner; }
private:
  Person *owner;
};
```

Note that if you use `boost::noncopyable` mixin with a compiler supporting C++11, the implementation of `boost::noncopyable` also automatically deletes the said member functions.

With this approach, any accidental copy will result in a quite friendlier error message:

```
example.cpp:34:7: error: call to deleted constructor of 'Car'
  Car anotherSedan = sedan;
      ^              ~~~~~
example.cpp:10:3: note: 'Car' has been explicitly marked deleted here
  Car(const Car&) = delete;
  ^
```

So, which of the above three methods is your favorite?



## codeproject [C++ Tip: Make Your Class Non-copyable Without Boost](https://www.codeproject.com/Tips/1220451/Cplusplus-Tip-Make-Your-Class-Non-copyable-Without)

### Use delete Keyword

Delete the copy constructor and assignment operator. This works for C++11 and above.

```C++
// Works for C++11 and above
class DeletedCopyFunc
{
public:
    DeletedCopyFunc(int value): m_Value(value) {}
public:
    DeletedCopyFunc(const DeletedCopyFunc&) = delete;
    DeletedCopyFunc& operator=(const DeletedCopyFunc&) = delete;

private:
    int m_Value;
    std::mutex m_Mutex;
};
```

### Make private

Declaring the copy constructor and assignment operator `private` is another way and it is perfectly fine not to define their bodies. This technique works for all C++ versions.

```C++
// Works for all C++ versions
class PrivateCopyFunc
{
public:
    PrivateCopyFunc(int value) : m_Value(value) {}
private:
    PrivateCopyFunc(const PrivateCopyFunc&);
    PrivateCopyFunc& operator=(const PrivateCopyFunc&);

private:
    int m_Value;
    std::mutex m_Mutex;
};
```

### How Boost Does It?

It can be seen from the Boost noncopyable source that it also uses the same techniques.

```C++
  class noncopyable
  {
  protected:
#if !defined(BOOST_NO_CXX11_DEFAULTED_FUNCTIONS) && 
         !defined(BOOST_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS)
      BOOST_CONSTEXPR noncopyable() = default;
      ~noncopyable() = default;
#else
      noncopyable() {}
      ~noncopyable() {}
#endif
#if !defined(BOOST_NO_CXX11_DELETED_FUNCTIONS)
      noncopyable( const noncopyable& ) = delete;
      noncopyable& operator=( const noncopyable& ) = delete;
#else
  private:  // emphasize the following members are private
      noncopyable( const noncopyable& );
      noncopyable& operator=( const noncopyable& );
#endif
  };
```

### Bonus: When copy Constructor and Assignment Operator are Called

Having done interviews over the years, I discover to my dismay that many job candidates are not aware of when copy constructor and assignment operator are called. Run the code below to see which lines are printed.

```C++
class CopyableClass
{
public:
    CopyableClass(int value) : m_Value(value) {}
    CopyableClass(const CopyableClass& that)
    {
        std::cout << "CopyableClass Copy Constructor called!" << std::endl;
        this->m_Value = that.m_Value;
    }
    CopyableClass& operator=(const CopyableClass& that)
    {
        std::cout << "CopyableClass Assignment Operator called!" << std::endl;
        this->m_Value = that.m_Value;
        return *this;
    }

private:
    int m_Value;
};

int main()
{
    CopyableClass a(10);
    CopyableClass b = a; // CopyableClass Copy Constructor called!
    b = a; // CopyableClass Assignment Operator called!

    CopyableClass c(a); // CopyableClass Copy Constructor called!

    return 0;
}
```

The example code is hosted at [Github](https://github.com/shaovoon/cpp_noncopyable_class_example).

## stackoverflow [How do I make this C++ object non-copyable?](https://stackoverflow.com/questions/2173746/how-do-i-make-this-c-object-non-copyable)



## wikibooks [Non-copyable Mixin](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-copyable_Mixin)



## Non-copyable and singleton

Singleton是典型的non-copyable ，参见 `C++\Pattern\Singleton` 章节。



## SUMMARY

应该遵循在`C++\Language-reference\Classes\Specifics`中提出的"Use specific to make it explicit principle"，优先使用specifier。