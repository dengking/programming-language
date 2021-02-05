# [macmade](https://github.com/macmade)/**[CPP-ARC](https://github.com/macmade/CPP-ARC)**

## About

**C++ Automatic Reference Counting**
This project intends to simplify memory management in C++, using reference counting.

When allocating memory with the `new` operator, CPP-ARC automatically adds a few bytes of memory to manage a **retain count** on the memory area.

> NOTE: 
>
> 1、将此称为memory meta info
>
> 2、通过overload new operator来实现的

Memory can then be **retained** or **released**.

An allocation sets the retain count to 1. A `retain` increments it, and a `release` decrements it.

> NOTE: 这种需要programmer来显式地调用`retain` 、 `release` 的做法肯定不如`std::shared_ptr`的基于constructor、destructor的做法好

When the retain count reaches 0, the memory is freed.

> NOTE: 这是它"automatic"特性的所在

C++ classes are fully compatible with this scheme, as the destructors are called when the memory is freed.

This project also includes a template used to auto-release allocated memory, when it's not used anymore.



## Read code

它的实现并没有太多地体现reference counting，它的实现给我的启发有:

1、overload new operator

2、memory meta info: 即每次分配多一些内存来保存一些meta data

### Memory meta info: `AllocInfos`

```C++
namespace CPPARC
{
    typedef struct _AllocInfos
    {
        std::size_t retainCount;
        std::size_t allocSize;
    } AllocInfos;
}

```



### Custom allocator

1、每次allocation都会额外分配`AllocInfos`，用于存放memory meta info

2、byte type

3、

```C++
namespace CPPARC
{

	void* allocate(std::size_t size);
	void* allocate(std::size_t size)
	{
		std::size_t allocSize;
		char *mem;
		AllocInfos *infos;

		allocSize = size + sizeof(AllocInfos);
		mem = reinterpret_cast<char*>(malloc(allocSize));

		if (mem == NULL)
		{
			return NULL;
		}

		infos = reinterpret_cast<AllocInfos*>(mem);
		infos->retainCount = 1;
		infos->allocSize = size;

		return reinterpret_cast<void*>(mem + sizeof(AllocInfos));
	}

	void deallocate(void *p);
	void deallocate(void *p)
	{
		char *mem;
		AllocInfos *infos;

		if (p == NULL)
		{
			return;
		}

		mem = reinterpret_cast<char*>(p);
		mem -= sizeof(AllocInfos);
		infos = reinterpret_cast<AllocInfos*>(mem);

		if (infos->retainCount > 0)
		{
			std::cerr << "Warning: deallocating an object with a retain count greater than zero!" << std::endl;

			exit (EXIT_FAILURE);
		}

		free(mem);
	}
}

```



### Overload `new` and  `delete` operator

```C++
#pragma mark -
#pragma mark Global namespace

void* operator new(std::size_t size) throw (std::bad_alloc);
void* operator new(std::size_t size) throw (std::bad_alloc)
{
	void *p;

	p = CPPARC::allocate(size);

	if (p == NULL)
	{
		throw std::bad_alloc();
	}

	return p;
}

void* operator new(std::size_t size, const std::nothrow_t &nothrow) throw ();
void* operator new(std::size_t size, const std::nothrow_t &nothrow) throw ()
{
	(void) nothrow;

	return CPPARC::allocate(size);
}

void* operator new[](std::size_t size) throw (std::bad_alloc);
void* operator new[](std::size_t size) throw (std::bad_alloc)
{
	void *p;

	p = CPPARC::allocate(size);

	if (p == NULL)
	{
		throw std::bad_alloc();
	}

	return p;
}

void* operator new[](std::size_t size, const std::nothrow_t &nothrow) throw ();
void* operator new[](std::size_t size, const std::nothrow_t &nothrow) throw ()
{
	(void) nothrow;

	return CPPARC::allocate(size);
}

void operator delete(void *ptr) throw ();
void operator delete(void *ptr) throw ()
{
	CPPARC::deallocate(ptr);
}

void operator delete(void *ptr, const std::nothrow_t &nothrow) throw ();
void operator delete(void *ptr, const std::nothrow_t &nothrow) throw ()
{
	(void) nothrow;

	CPPARC::deallocate(ptr);
}

void operator delete[](void *ptr) throw ();
void operator delete[](void *ptr) throw ()
{
	CPPARC::deallocate(ptr);
}

void operator delete[](void *ptr, const std::nothrow_t &nothrow) throw ();
void operator delete[](void *ptr, const std::nothrow_t &nothrow) throw ()
{
	(void) nothrow;

	CPPARC::deallocate(ptr);
}
```



### `class AutoPointer`

```C++
namespace CPPARC
{

template<typename T>
class AutoPointer
{
public:

	class Exception
	{
	public:

		Exception()
		{
			this->message = "Unknow exception";
		}

		Exception(std::string msg)
		{
			this->message = msg;
		}

		std::string message;
	};

	AutoPointer(void)
	{
		this->_ptr = NULL;
		this->_retainCount = 0;
	}

	AutoPointer(T *ptr)
	{
		this->_ptr = ptr;
		this->_retainCount = 0;

		if (this->_ptr != NULL)
		{
			this->_retainCount++;

			CPPARC::retain(this->_ptr);
		}
	}

	AutoPointer(const AutoPointer<T> &arp)
	{
		this->_ptr = arp._ptr;
		this->_retainCount = arp._retainCount;

		if (this->_ptr != NULL)
		{
			this->_retainCount++;

			CPPARC::retain(this->_ptr);
		}
	}

	~AutoPointer(void)
	{
		if (this->_ptr != NULL)
		{
			this->_retainCount--;

			CPPARC::release(this->_ptr);

			if (this->_retainCount == 0)
			{
				CPPARC::release(this->_ptr);
			}
		}
	}

	AutoPointer<T>& operator =(const AutoPointer<T> &arp)
	{
		if (this == &arp)
		{
			return *(this);
		}

		if (this->_ptr == arp._ptr)
		{
			return *(this);
		}

		this->_ptr = arp._ptr;
		this->_retainCount = arp._retainCount;

		if (this->_ptr != NULL)
		{
			this->_retainCount++;

			CPPARC::retain(this->_ptr);
		}

		return *(this);
	}

	void* operator new(std::size_t size) throw ()
	{
		(void) size;

		throw CPPARC::AutoPointer<T>::Exception("CPPARC::AutoPointer class cannot be allocated using new!");

		return NULL;
	}

	T& operator *(void)
	{
		return *(this->_ptr);
	}

	T* operator ->(void)
	{
		return this->_ptr;
	}

private:

	T *_ptr;
	std::size_t _retainCount;
};
}

```

