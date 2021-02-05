#include "include/cpp-arc.h"

/*******************************************************************************
 * CPP ARC namespace
 ******************************************************************************/

#pragma mark -
#pragma mark CPP ARC namespace

namespace CPPARC
{
typedef struct _AllocInfos
{
	std::size_t retainCount;
	std::size_t allocSize;
} AllocInfos;

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

template<typename T> void release(T *p);
template<typename T> void release(T *p)
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

	infos->retainCount--;

	if (infos->retainCount == 0)
	{
		delete p;
	}
}

template<typename T> T* retain(T *p);
template<typename T> T* retain(T *p)
{
	char *mem;
	AllocInfos *infos;

	if (p == NULL)
	{
		return NULL;
	}

	mem = reinterpret_cast<char*>(p);
	mem -= sizeof(AllocInfos);
	infos = reinterpret_cast<AllocInfos*>(mem);

	infos->retainCount++;

	return p;
}

std::size_t getRetainCount(void *p);
std::size_t getRetainCount(void *p)
{
	char *mem;
	AllocInfos *infos;

	if (p == NULL)
	{
		return 0;
	}

	mem = reinterpret_cast<char*>(p);
	mem -= sizeof(AllocInfos);
	infos = reinterpret_cast<AllocInfos*>(mem);

	return infos->retainCount;
}

template<typename T> class AutoPointer
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

/*******************************************************************************
 * Global namespace
 ******************************************************************************/

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
