# Copyable and movable

## Non-copyable but movable

### Object-based resource management: Resource wrapper

一、一般，一个resource wrapper，它own(拥有) resource，它的lifetime会和resource进行绑定，一般采用RAII

二、对应这样的resource wrapper，它们一般都是non-copyable but movable的，因为，一旦运行copy，那么久可能导致多个object同时执行同一个resource，如果其中的一个object end，那么在它的destructor中就会将resource给release，这就可能导致其他的依然存活的object使用这个resource的时候，出现错误，这会导致:

1、dangling pointer

2、double free

#### 我的实践



### boost [Movable but Non-Copyable Types](http://cs.swan.ac.uk/~csoliver/ok-sat-library/internet_html/doc/doc/Boost/1_53_0/doc/html/move/movable_only_classes.html)



## Example of non-copyable



### 一、Lock

Lock是典型的non-copyable的，参见: 

1、[mohaps](https://github.com/mohaps) / [lrucache](https://github.com/mohaps/lrucache) / [LRUCache.hpp](https://github.com/mohaps/lrucache/blob/master/LRUCache.hpp)

```C++
/**
 * a base lock class that can be overridden
 */
class Lock {
public:
	virtual ~Lock() {
	}
	virtual void lock() = 0;
	virtual void unlock() = 0;
protected:
	Lock() {
	}
private:
	Lock(const Lock&);
	const Lock& operator =(const Lock&);
};
```



