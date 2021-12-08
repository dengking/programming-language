# The `template` disambiguator for dependent names

## cppreference [Dependent names](https://en.cppreference.com/w/cpp/language/dependent_name) # The `template` disambiguator for dependent names

Similarly, in a template definition, a dependent name that is not a member of the *current instantiation* is not considered to be a template name unless the disambiguation keyword `template` is used or unless it was already established as a template name:

```c++
template<typename T>
struct S
{
	template<typename U> void foo()
	{
	}
};

template<typename T>
void bar()
{
	S<T> s;
	s.foo<T>(); // error: < parsed as less than operator
	s.template foo<T>(); // OK
}

```

## Example



```C++
#include "stddef.h" // size_t
#include <vector>

typedef struct SFieldInfo
{
	/**
	 * 字段在结构体中的偏移位置
	 */
	size_t m_iOffset;
};

class CStructRtti
{

	std::vector<SFieldInfo> m_FieldMetaList;
public:
	template<typename ValueType>
	const ValueType* GetValue(void *Data, int Index) const
	{
		const SFieldInfo &FieldMeta = m_FieldMetaList[Index];
		size_t iOffset = FieldMeta.m_iOffset;
		char *DataStartAddress = reinterpret_cast<char*>(Data) + iOffset;
		return reinterpret_cast<const ValueType*>(DataStartAddress);
	}
};

class CTableInterface
{
protected:
	/**
	 * 列信息
	 */
	CStructRtti *m_Columns { nullptr };
public:
	virtual double GetDouble(size_t RowID, size_t ColID)=0;
};

template<typename RowType>
class CTableImpl: public CTableInterface
{
	RowType m_Row { };
public:
	double GetDouble(size_t RowID, size_t ColID) override
	{
		return *m_Columns->template GetValue<double>(&m_Row, ColID); // 必须要使用 The `template` disambiguator for dependent names
	}
};

struct SRow
{

};

int main()
{
	CTableImpl<SRow> t;
	t.GetDouble(1, 1);
}
// g++ --std=c++11 test.cpp


```

### Derived class template CRTP

```C++
#include <iostream>

template<typename UstTag, typename SpiType>
class CUstApi
{
protected:
	template<typename ServiceImpl, typename ServiceTrait>
	struct CServiceRspBase
	{
	public:
		CServiceRspBase()
		{

		}
		int Run()
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;
			// 回调
			return static_cast<ServiceImpl*>(this)->CallBack();
		}
	};
};

template<typename UstTag, typename SpiType>
class CUstApiImpl: public CUstApi<UstTag, SpiType>
{
protected:
	template<typename ServiceTrait>
	struct CServiceMultiSpanRsp: CUstApi<UstTag, SpiType>::template CServiceRspBase<CServiceMultiSpanRsp<ServiceTrait>, ServiceTrait>
	{
		friend typename CUstApi<UstTag, SpiType>:: template CServiceRspBase<CServiceMultiSpanRsp<ServiceTrait>, ServiceTrait>;
	protected:
		int CallBack()
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;
			return 0;
		}

	};
public:
	class CTestServiceTrait
	{
	};
	void Test()
	{
		CServiceMultiSpanRsp<CTestServiceTrait> s;
		s.Run();
	}
};

struct TestUstTag
{
};
struct TestSpiType
{
};

int main()
{
	CUstApiImpl<TestUstTag, TestSpiType> Api;
	Api.Test();
}
// g++ test.cpp -Wall -pedantic

```

如果将 `struct CServiceMultiSpanRsp: CUstApi<UstTag, SpiType>::CServiceRspBase<CServiceMultiSpanRsp<ServiceTrait>, ServiceTrait>` 中的 `template` 拿掉，则编译报错如下:

```C++
test.cpp:29:57: error: non-template ‘CServiceRspBase’ used as template
  struct CServiceMultiSpanRsp: CUstApi<UstTag, SpiType>::CServiceRspBase<CServiceMultiSpanRsp<ServiceTrait>, ServiceTrait>
                                                         ^~~~~~~~~~~~~~~
test.cpp:29:57: note: use ‘CUstApi<UstTag, SpiType>::template CServiceRspBase’ to indicate that it is a template
test.cpp:39:3: error: expected ‘{’ before ‘;’ token
  };

```



```C++
test.cpp:29:57: 错误：非模板‘CServiceRspBase’用作模板
  struct CServiceMultiSpanRsp: CUstApi<UstTag, SpiType>::CServiceRspBase<CServiceMultiSpanRsp<ServiceTrait>, ServiceTrait>
                                                         ^
test.cpp:29:57: 附注：使用‘CUstApi<UstTag, SpiType>::template CServiceRspBase’来表示这是一个模板
test.cpp:39:3: 错误：expected ‘{’ before ‘;’ token
  };

```