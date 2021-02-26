# Implementation of `shard_ptr`

## 理论

参见

1、wikipedia [Garbage collection](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science)) 

2、 [Reference counting](https://en.wikipedia.org/wiki/Reference_counting)

## learncpp [std::shared_ptr](https://www.learncpp.com/cpp-tutorial/15-6-stdshared_ptr/)：

> Unlike std::unique_ptr, which uses a single pointer internally, std::shared_ptr uses two pointers internally. One pointer points at the resource being managed. The other points at a “control block”, which is a dynamically allocated object that tracks of a bunch of stuff, including how many std::shared_ptr are pointing at the resource. When a std::shared_ptr is created via a std::shared_ptr constructor, the memory for the managed object (which is usually passed in) and control block (which the constructor creates) are allocated separately. However, when using std::make_shared(), this can be optimized into a single memory allocation, which leads to better performance.
>
> This also explains why independently creating two std::shared_ptr pointed to the same resource gets us into trouble. Each std::shared_ptr will have one pointer pointing at the resource. However, each std::shared_ptr will independently allocate its own control block, which will indicate that it is the only pointer owning that resource. Thus, when that std::shared_ptr goes out of scope, it will deallocate the resource, not realizing there are other std::shared_ptr also trying to manage that resource.
>
> However, when a std::shared_ptr is cloned using copy assignment, the data in the control block can be appropriately updated to indicate that there are now additional std::shared_ptr co-managing the resource.



## Implementation 1

http://www.josuttis.com/libbook/cont/countptr.hpp.html

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

## Implementation 2

[Boost.SmartPtr: The Smart Pointer Library](https://www.boost.org/doc/libs/1_72_0/libs/smart_ptr/doc/html/smart_ptr.html)





