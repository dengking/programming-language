# Singleton mixin C++



## stackoverflow [Singleton mixin C++](https://stackoverflow.com/questions/10221522/singleton-mixin-c)

**Summary:** How can I create a **singleton mixin** in C++? I am trying to avoid copying the same `get_instance()` function, the private constructor, etc. But I can't figure out a way to make this a mixin, because the static instance will be shared by everything that inherits from the mixin.



### [A](https://stackoverflow.com/a/10221826)



### [A](https://stackoverflow.com/a/10221688)

The problem is both `DerivedRegistryA` and `DerivedRegistryB` is sharing the same `name_to_object_ptr`

`get_instance` belongs to `Registry<T>`not any `DerivedRegistry` and both `DerivedRegistry` is actually `Registry<int>` e.g. both are same type. so both are sharing the same `static` storage. as **static storage belongs to a class not to an object**.

So both are getting the same copy of `instance` which is of `Registry<T>` type.

You need to have an instance function in the derived class or somehow treat every derived class as a different type. that some how can be done by making changes in your template arguments that changes the type but doesn't alter your logic. However that will be a very bad design.

you can remove the `get_instance` from `Registry` and instead

```cpp
template <typename T>
class Singleton{
  public:
  static T& get_instance(){
    static T& instance;
    return instance;
  }
};

class DerivedRegistryA : public Registry<int>, public Singleton<DerivedRegistryA>{

};
```

This would be a generic solution, and you can plug the `Singleton` class to all classes where you need singleton

## Implementation

### Mixin from above

#### 错误的实现方式

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
    Singleton s;
}
// g++ --std=c++11 test.cpp

```

通过上述code来看，它无法实现singleton，在`main()`中，可以看到`Singleton s;`能够编译通过；

#### 错误的实现方式

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