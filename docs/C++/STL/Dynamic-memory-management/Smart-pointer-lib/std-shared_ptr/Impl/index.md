# Implementation of `shard_ptr`

## 理论

参见

1、wikipedia [Garbage collection](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science)) 

2、 wikipedia [Reference counting](https://en.wikipedia.org/wiki/Reference_counting)

## Implementation

### stackoverflow [How is the std::tr1::shared_ptr implemented?](https://stackoverflow.com/questions/9200664/how-is-the-stdtr1shared-ptr-implemented)



[A](https://stackoverflow.com/a/9201435)

> NOTE: 
>
> 1、这个回答给出的source code，我觉得是最好的，非常地简单易懂
>
> 2、在`nextptr-shared_ptr-basics-and-internals`中，引用了下面的source code，并对其进行了详细分析，结合其中的内容可知，下面的实现虽然短小，但是功能是非常强大的

`shared_ptr` must manage a reference counter and the carrying of a deleter functor that is deduced by the type of the object given at initialization.

> NOTE: 
>
> 一、"deleter functor that is deduced by the type of the object given at initialization"
>
> 1、这一点非常重要，因为它保证了能够正确的析构由`shared_ptr`管理的object，它取的是管理的object的real type，而不是由programmer指定的class template parameter
>
> 2、上面这段话中的 "deleter functor "，应该指代的是default deleter
>
> 3、从下面的实现可知class template parameter 和 construct template parameter是不同的
>
> 

The `shared_ptr` class typically hosts two members: a `T*` (that is returned by `operator->` and dereferenced in `operator*`) and a `aux*` where `aux` is a inner abstract class that contains:

- a counter (incremented / decremented upon copy-assign / destroy)
- whatever is needed to make increment / decrement atomic (not needed if specific platform atomic INC/DEC is available)
- an abstract `virtual destroy()=0;`
- a virtual destructor.

Such `aux` class (the actual name depends on the implementation) is derived by a family of templatized classes (parametrized on the type given by the explicit constructor, say `U` derived from `T`), that add:

- a pointer to the object (same as `T*`, but with the actual type: this is needed to properly manage all the cases of `T` being a base for whatever `U` having multiple `T` in the derivation hierarchy)
- a copy of the `deletor` object given as deletion policy to the explicit constructor (or the default `deletor` just doing delete `p`, where `p` is the `U*` above)
- the override of the destroy method, calling the deleter functor.

A simplified sketch can be this one:

```cpp
template<class T>
class shared_ptr
{
    struct aux
    {
        unsigned count;

        aux() :count(1) {}
        virtual void destroy()=0;
        virtual ~aux() {} //must be polymorphic
    };

    template<class U, class Deleter>
    struct auximpl: public aux
    {
        U* p;
        Deleter d;

        auximpl(U* pu, Deleter x) :p(pu), d(x) {}
        virtual void destroy() { d(p); } 
    };

    template<class U>
    struct default_deleter
    {
        void operator()(U* p) const { delete p; }
    };

    aux* pa;
    T* pt;

    void inc() { if(pa) interlocked_inc(pa->count); }

    void dec() 
    { 
        if(pa && !interlocked_dec(pa->count)) 
        {  pa->destroy(); delete pa; }
    }

public:

    shared_ptr() :pa(), pt() {}

    template<class U, class Deleter>
    shared_ptr(U* pu, Deleter d) :pa(new auximpl<U,Deleter>(pu,d)), pt(pu) {}

    template<class U>
    explicit shared_ptr(U* pu) :pa(new auximpl<U,default_deleter<U> >(pu,default_deleter<U>())), pt(pu) {}

    shared_ptr(const shared_ptr& s) :pa(s.pa), pt(s.pt) { inc(); }

    template<class U>
    shared_ptr(const shared_ptr<U>& s) :pa(s.pa), pt(s.pt) { inc(); }

    ~shared_ptr() { dec(); }

    shared_ptr& operator=(const shared_ptr& s)
    {
        if(this!=&s)
        {
            dec();
            pa = s.pa; pt=s.pt;
            inc();
        }        
        return *this;
    }

    T* operator->() const { return pt; }
    T& operator*() const { return *pt; }
};
```

> NOTE: 
> 1、`aux`、`auximpl`是典型的"OOP interface + template implementation"

Where `weak_ptr` interoperability is required a second counter (`weak_count`) is required in `aux` (will be incremented / decremented by `weak_ptr`), and `delete pa` must happen only when both the counters reach zero.



### learncpp [std::shared_ptr](https://www.learncpp.com/cpp-tutorial/15-6-stdshared_ptr/)：

> Unlike std::unique_ptr, which uses a single pointer internally, std::shared_ptr uses two pointers internally. One pointer points at the resource being managed. The other points at a “control block”, which is a dynamically allocated object that tracks of a bunch of stuff, including how many std::shared_ptr are pointing at the resource. When a std::shared_ptr is created via a std::shared_ptr constructor, the memory for the managed object (which is usually passed in) and control block (which the constructor creates) are allocated separately. However, when using std::make_shared(), this can be optimized into a single memory allocation, which leads to better performance.
>
> This also explains why independently creating two std::shared_ptr pointed to the same resource gets us into trouble. Each std::shared_ptr will have one pointer pointing at the resource. However, each std::shared_ptr will independently allocate its own control block, which will indicate that it is the only pointer owning that resource. Thus, when that std::shared_ptr goes out of scope, it will deallocate the resource, not realizing there are other std::shared_ptr also trying to manage that resource.
>
> However, when a std::shared_ptr is cloned using copy assignment, the data in the control block can be appropriately updated to indicate that there are now additional std::shared_ptr co-managing the resource.



### Implementation 1

http://www.josuttis.com/libbook/cont/countptr.hpp.html

这个实现非常地鸡肋。

```c++
#ifndef COUNTED_PTR_HPP
#define COUNTED_PTR_HPP

/* class for counted reference semantics
 * - deletes the object to which it refers when the last CountedPtr
 *   that refers to it is destroyed
 */
template <class T>
class CountedPtr {
  private:
    T* ptr;        // pointer to the value
    long* count;   // shared number of owners

  public:
    // initialize pointer with existing pointer
    // - requires that the pointer p is a return value of new
    explicit CountedPtr (T* p=0)
     : ptr(p), count(new long(1)) {
    }

    // copy pointer (one more owner)
    CountedPtr (const CountedPtr<T>& p) throw()
     : ptr(p.ptr), count(p.count) {
        ++*count;
    }

    // destructor (delete value if this was the last owner)
    ~CountedPtr () throw() {
        dispose();
    }

    // assignment (unshare old and share new value)
    CountedPtr<T>& operator= (const CountedPtr<T>& p) throw() {
        if (this != &p) {
            dispose();
            ptr = p.ptr;
            count = p.count;
            ++*count;
        }
        return *this;
    }

    // access the value to which the pointer refers
    T& operator*() const throw() {
        return *ptr;
    }
    T* operator->() const throw() {
        return ptr;
    }

  private:
    void dispose() {
        if (--*count == 0) {
             delete count;
             delete ptr;
        }
    }
    
};

#endif /*COUNTED_PTR_HPP*/
```

### Implementation 2

[Boost.SmartPtr: The Smart Pointer Library](https://www.boost.org/doc/libs/1_72_0/libs/smart_ptr/doc/html/smart_ptr.html)





