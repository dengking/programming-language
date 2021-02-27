# Parameterized Base Class



## More C++ Idioms/[Parameterized Base Class](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Parameterized_Base_Class)

### Also Known As

1、Mixin-from-below

2、Parameterized Inheritance

### Motivation

A certain aspect can be abstracted out from requirements and be developed as templates (e.g., object serialization). Serialization is a **cross-cutting concern** that many classes/POD types in an application may have. Such a concern can be abstracted out in a manageable reusable module. By addition of an **aspect**, substitutability with the original type is not broken so another motivation is to have a IS-A (public inheritance) or WAS-A (private inheritance) relationship with the type parameter.

> NOTE: **cross-cutting concern**即横切关注点，它是[aspect oriented programming](https://en.wikipedia.org/wiki/Aspect-oriented_programming)中的概念



### Solution and Sample Code

```c++
template <class T>
class Serializable : public T,   /// Parameterized Base Class Idiom
                     public ISerializable
{
  public:
    Serializable (const T &t = T()) : T(t) {}
    virtual int serialize (char *& buffer, size_t & buf_size) const
    {
        const size_t size = sizeof (T);
        if (size > buf_size)
          throw std::runtime_error("Insufficient memory!");

        memcpy (buffer, static_cast<const T *>(this), size);
        buffer += size;
        buf_size -= size;
        return size;
    }
};
```

`Serializable <T>` can be used polymorphically as a `T` as well as a `ISerializable`. Above example works correctly only if `T` is a user-defined POD type without pointers.

## Variadic multiple inheritance and mixin multiple

