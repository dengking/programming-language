# Singleton mixin C++



## My implementation



### Mixin from above

#### 错误的实现方式: 未private constructor

```C++
/**
 * @brief 单例mixin
 *
 * @tparam DerivedClass
 */
class noncopyable
{
public:
	noncopyable() = default;
	~noncopyable() = default;
	// C++ 11
	// =======
	// We can use the better technique of deleting the methods
	// we don't want.

	// Note: Scott Meyers mentions in his Effective Modern
	//       C++ book, that deleted functions should generally
	//       be public as it results in better error messages
	//       due to the compilers behavior to check accessibility
	//       before deleted status
	noncopyable(const noncopyable&) = delete;
	noncopyable& operator=(const noncopyable&) = delete;
};

template<typename DerivedClass>
class SingletonMixin: public noncopyable
{
public:
	static DerivedClass& GetInstance()
	{
		static DerivedClass instance;
		return instance;
	}
};

class Singleton: public SingletonMixin<Singleton>
{

};
int main()
{
	Singleton::GetInstance();
    Singleton s; // 错误的使用方式也能够编译通过
}
// g++ --std=c++11 test.cpp

```

通过上述code来看，它无法实现singleton，在`main()`中，可以看到`Singleton s;`能够编译通过；

#### 错误的实现方式: 在base class中，无法access private constructor

```C++
/**
 * @brief 单例mixin
 *
 * @tparam DerivedClass
 */
class noncopyable
{
public:
	noncopyable() = default;
	~noncopyable() = default;
	// C++ 11
	// =======
	// We can use the better technique of deleting the methods
	// we don't want.

	// Note: Scott Meyers mentions in his Effective Modern
	//       C++ book, that deleted functions should generally
	//       be public as it results in better error messages
	//       due to the compilers behavior to check accessibility
	//       before deleted status
	noncopyable(const noncopyable&) = delete;
	noncopyable& operator=(const noncopyable&) = delete;
};

template<typename DerivedClass>
class SingletonMixin: public noncopyable
{
public:
	static DerivedClass& GetInstance()
	{
		static DerivedClass instance;
		return instance;
	}
};

class Singleton: public SingletonMixin<Singleton>
{
private:
    Singleton(){};    
};
int main()
{
	Singleton::GetInstance();
    Singleton s;
}
// g++ --std=c++11 test.cpp

```



上述实现方式，将`DerivedClass`的constructor声明为private，则上述代码是会编译报错的:

```C++
test.cpp: In instantiation of ‘static DerivedClass& SingletonMixin<DerivedClass>::GetInstance() [with DerivedClass = Singleton]’:
test.cpp:26:13:   required from here
test.cpp:20:2: error: ‘Singleton::Singleton()’ is private
  Singleton()
  ^
test.cpp:12:23: error: within this context
   static DerivedClass instance;
```

#### 正确的实现方式: CRTP + friend base class

```C++
/**
 * @brief 单例mixin
 *
 * @tparam DerivedClass
 */
class noncopyable
{
public:
	noncopyable() = default;
	~noncopyable() = default;
	// C++ 11
	// =======
	// We can use the better technique of deleting the methods
	// we don't want.

	// Note: Scott Meyers mentions in his Effective Modern
	//       C++ book, that deleted functions should generally
	//       be public as it results in better error messages
	//       due to the compilers behavior to check accessibility
	//       before deleted status
	noncopyable(const noncopyable&) = delete;
	noncopyable& operator=(const noncopyable&) = delete;
};

template<typename DerivedClass>
class SingletonMixin: public noncopyable
{
public:
	static DerivedClass& GetInstance()
	{
		static DerivedClass instance;
		return instance;
	}
protected:
    SingletonMixin()=default;  
};

class Singleton: public SingletonMixin<Singleton>
{
public:
    friend class SingletonMixin<Singleton>;
private:
    Singleton(){};    
};
int main()
{
	Singleton::GetInstance();
    // Singleton s;
}
// g++ --std=c++11 test.cpp

```

1、在derive class中，通过friend将private constructor开放给base class: 添加上 `friend class SingletonMixin<Singleton>;` 从而能够消除前面的错误

2、上述friend的用法，是参考自stackexchange [Modern C++ Singleton Template](https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template)。

3、需要将`SingletonMixin`的constructor protected

### Mixin from below



#### 实现方式一

```C++
#include <iostream>

/**
 * @brief 单例mixin
 *
 * @tparam DerivedClass
 */
template<class SingletonClass>
class SingletonMixin: public SingletonClass
{
public:
	/** Singleton creation function */
	static SingletonClass& GetInstance()
	{
		static SingletonClass _instance;
		return _instance;
	}
};

class Singleton
{
public:
	void Test()
	{
		std::cout << this << std::endl;
	}
protected:
	Singleton()
	{
	}
};

int main()
{
	SingletonMixin<Singleton>::GetInstance().Test();
	SingletonMixin<Singleton>::GetInstance().Test();
}
// g++ test.cpp

```

上述代码编译报错如下:

```C++
test.cpp: In instantiation of ‘static SingletonClass& SingletonMixin<SingletonClass>::GetInstance() [with SingletonClass = Singleton]’:
test.cpp:35:29:   required from here
test.cpp:28:2: error: ‘Singleton::Singleton()’ is protected
  Singleton()
  ^
test.cpp:15:25: error: within this context
   static SingletonClass _instance;
```



#### 实现方式二

```C++
#include <iostream>

/**
 * @brief 单例mixin
 *
 * @tparam DerivedClass
 */
template<class SingletonClass>
class SingletonMixin: public SingletonClass
{
public:
	/** Singleton creation function */
	static SingletonMixin& GetInstance()
	{
		static SingletonMixin _instance;
		return _instance;
	}
};

class Singleton
{
public:
	void Test()
	{
		std::cout << this << std::endl;
	}
protected:
	Singleton()
	{
	}
};

int main()
{
	SingletonMixin<Singleton>::GetInstance().Test();
	SingletonMixin<Singleton>::GetInstance().Test();
}
// g++ test.cpp

```

能够正常编译通过。

## codereview.stackexchange [Modern C++ Singleton Template](https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template)



I recently read about the C++17 `static inline` member declaration and thought that this will make templates a little bit cleaner, since static members can now be initialized inside a templated class.

> NOTE: 
>
> 1、对C++17对static  inline的总结较好

Here is the template:

```C++
template<typename T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static MemGuard g; // clean up on program end
		if (!m_instance)
		{
			m_instance = new T();
		}
		return *m_instance;
	}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton) = delete;

protected:
	Singleton()
	{
	}
	;
	virtual ~Singleton()
	{
	}

private:
	inline static T *m_instance = nullptr;

	class MemGuard
	{
	public:
		~MemGuard()
		{
			delete m_instance;
			m_instance = nullptr;
		}
	};
};

class Test final : public Singleton<Test>
{
	friend class Singleton<Test> ;
public:
	void TestIt()
	{
	}
	;
private:
	Test()
	{
	}
	~Test()
	{ /* Test intern clean up */
	}
};

int main()
{
	Test::GetInstance();
}
// g++ --std=c++17 test.cpp -Wall -pedantic

```

> NOTE: 
>
> 一、实现点:
>
> 1、mixin from above + friend base class
>
> 2、exit guard
>
> 二、上述实现方式，相比于Meyer's singleton太复杂了
>
> 三、上述实现方式存在如下问题:
>
> 1、race condition

### [A](https://codereview.stackexchange.com/a/173935)

#### Singleton is bad practice

Okay so first of the obligatory [Singletons are bad practice](https://stackoverflow.com/questions/137975/what-is-so-bad-about-singletons) so you probably shouldn't make it easy to write bad code

#### Race condition

Ignoring the fact that the class probably shouldn't exist at all we can look at the code.

```cpp
static T& GetInstance() {
  static MemGuard g; // clean up on program end
  if (!m_instance) {
    m_instance = new T(); 
  }
  return *m_instance;
}
```

If multiple threads access this instance simultaneously before it is created, you have a data race and `m_instance` may end up be being constructed multiple times or other kinds of undefined behaviour. You need to add mutex locks around the `if` block or use `std::call_once` which is preferred.

As it is supposed to be a singleton you're not supposed to be able to create more instances as the meaning of a singleton is to just have one instance but it appears that it is fully possible to construct multiple instances of `Test` simply by creating them as local variables. So this is a design flaw in your template.

#### Meyer's singleton

A much better way of creating a singleton is to rely on C++11 [Magic Statics (N2660)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2660.htm). And simply do this:

```cpp
class Test{
private:
    Test(); // Disallow instantiation outside of the class.
public:
    Test(const Test&) = delete;
    Test& operator=(const Test &) = delete;
    Test(Test &&) = delete;
    Test & operator=(Test &&) = delete;

    static auto& instance(){
        static Test test;
        return test;
    }
}; 
```

Which is much easier to write than your code, it's thread safe and fixes the issues with allowing `Test` to be instantiated. The properties of magic statics guarantee that `test` will be initialised exactly once the first time the function body is entered by any thread, even in the presence of multiple threads that might otherwise cause a data-race. The instance will be deconstructed when your `main()` function returns (in the static destruction stage) which makes the whole `MemGuard` thing unnecessary.

### [A](https://codereview.stackexchange.com/a/173942)

#### Singleton is bad practice

Singletons make it hard to test your code, and in my job I'd reject this at review for encouraging the development of untestable features. That said, I'll continue reviewing despite that.

#### No need for helper class

The `MemGuard` appears to be a poor man's reimplementation of `std::unique_ptr`. It would be much simpler for you to declare `m_instance` as a `std::unique_ptr<T>`, and then just return `*m_instance` from your accessor.

There's a race condition when two or more threads try to create the instance (when both see a null pointer "before" the other has set it). You *could* work around this with a mutex lock, but it's simpler to use a local static variable, [which is thread-safe](https://stackoverflow.com/q/8102125):

```cpp
#include <memory>
template<typename T>
T& Singleton<T>::instance()
{
    static const std::unique_ptr<T> instance{new T{}};
    return *instance;
}
```

------

#### We don't need a destructor

There's no need for the empty virtual destructor, as the constructed object will always be deleted as its declared type.

------

#### Revised implementation

> NOTE: 实现总结:
>
> 1、constructor token 
>
> 2、mixin from above

With my changes, the code reduces to

```cpp
template<typename T>
class Singleton {
public:
    static T& instance();

    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

protected:
    struct token {};
    Singleton() {}
};

#include <memory>
template<typename T>
T& Singleton<T>::instance()
{
    static const std::unique_ptr<T> instance{new T{token{}}};
    return *instance;
}
```

I'm using a constructor token to allow the base class to call the subclass's constructor without needing to be a `friend`.

#### Example

An example `T` looks like:

```cpp
#include <iostream>
class Test final : public Singleton<Test>
{
public:
    Test(token) { std::cout << "constructed" << std::endl; }
    ~Test() {  std::cout << "destructed" << std::endl; }

    void use() const { std::cout << "in use" << std::endl; };
};
```

Although the constructor is public, it can't be called without a `Singleton<T>::token` object, meaning that access to it is now controlled.

#### Tests:

>  NOTE: 
>
> 完整测试程序如下: 

```cpp
#include <memory>

template<typename T>
class Singleton
{
public:
	static T& instance();

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton) = delete;

protected:
	struct token
	{
	};
	Singleton()
	{
	}
};

template<typename T>
T& Singleton<T>::instance()
{
	static const std::unique_ptr<T> instance { new T { token { } } };
	return *instance;
}
#include <iostream>
class Test final : public Singleton<Test>
{
public:
	Test(token)
	{
		std::cout << "constructed" << std::endl;
	}
	~Test()
	{
		std::cout << "destructed" << std::endl;
	}

	void use() const
	{
		std::cout << "in use" << std::endl;
	}
	;
};

int main()
{
	// Test cannot_create; /* ERROR */

	std::cout << "Entering main()" << std::endl;
	{
		auto const &t = Test::instance();
		t.use();
	}
	{
		auto const &t = Test::instance();
		t.use();
	}
	std::cout << "Leaving main()" << std::endl;
}
// g++ --std=c++11 test.cpp
```

> NOTE:
>
> 1、输出如下:
>
> ```C++
> Entering main()
> constructed
> in use
> in use
> Leaving main()
> destructed
> ```
>
> 

------

#### Afterthought:

There's no need for the smart pointer; ordinary memory management works here:

```cpp
template<typename T>
T& Singleton<T>::instance()
{
    static T instance{token{}};
    return instance;
}
```



## theimpossiblecode [C++11 generic singleton pattern](https://www.theimpossiblecode.com/blog/c11-generic-singleton-pattern/)

> NOTE: 看了一下，实现非常一般

### Using C++11

The solution described here is using [lambda expressions](http://en.cppreference.com/w/cpp/language/lambda) and the C++11 thread safety guarantee for [function-local statics](http://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables).



## stackoverflow [Singleton mixin C++](https://stackoverflow.com/questions/10221522/singleton-mixin-c) # [A](https://stackoverflow.com/a/10225376)

this should help to solve your problem. Be aware that this singleton is not thread safe. But you can change it if you need to.

See [CRTP](http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) for more details.

```cpp
#include <iostream>
#include <map>
#include <string>

// simple singleton
template <class T>
class Singleton {
public:
    static T& Instance() { static T instance; return instance; }
protected:
    Singleton(){}
};

// your Registry Base
template <class T>
class Registry : public Singleton< Registry<T> > {
    friend class Singleton<Registry>;
public:
    void register_name( const std::string& name, T value ){ m_data[name] = value; }
    const T& lookup_name( const std::string& name ){ return m_data[name]; }

private:
    Registry(){}
    Registry(const Registry&){} // to prevent copies, you have to use ::Instance()

    std::map<std::string, T> m_data;
};

int main(int argc, char *argv[])
{
    Registry<int>& instance = Registry<int>::Instance();

    instance.register_name("Value1",1);
    Registry<int>::Instance().register_name("Value2",2);

    int value = instance.lookup_name("Value1");
    std::cout << "Value1=" << value << std::endl;
    std::cout << "Value2=" << Registry<int>::Instance().lookup_name("Value2") << std::endl;

    return 0;
}
```

> mixin from above + friend base class

## chromium singleton

source code: https://github.com/chromium/chromium/blob/master/base/memory/singleton.h

这个实现非常复杂