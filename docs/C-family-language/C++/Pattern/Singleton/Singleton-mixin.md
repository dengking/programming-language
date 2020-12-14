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

```C++
/**
 * @brief 单例
 *
 * @tparam DerivedClass
 */
template<typename DerivedClass>
class SingletonMixin
{
public:
	static DerivedClass& GetInstance()
	{
		static DerivedClass instance;
		return instance;
	}
};
```

