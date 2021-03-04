# Factory mixin

```C++
#include<utility>

template<typename DerivedClass>
class CFactoryMixin
{
public:
	template<typename ...Args>
	static DerivedClass* New(Args &&...args)
	{
		return new DerivedClass(std::forward<Args>(args)...);
	}
};

class CTest: public CFactoryMixin<CTest>
{

};
int main()
{
	CTest *obj = CTest::New();
	delete obj;
}
// g++ --std=c++11 test.cpp

```

