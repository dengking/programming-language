# `memset`

`memset`给予programmer直接操作memory的权限，这种权限是非常大的，稍有不慎，错误的使用将会导致非常严重甚至灾难性地后果。下面是一个示例：

```c++

#include <cstring>
#include <iostream>

struct TestStruct
{
	int one;
	int two;
	int three;
};

std::ostream& operator<<(std::ostream& Stream, TestStruct* s)
{
	Stream << s->one << std::endl;
	Stream << s->two << std::endl;
	Stream << s->three << std::endl;
	return Stream;
}
/**
 * 这是错误地、极其危险的memset，本意是memset s指向的内存区域；但是在重构的时候，是直接copy的原来的实现，但是此时的入参是pointer，所以&s 取的是指针的地址；
 * 所以，下面的memset就错误地将内存中其他位置的值给set了，这是非常严重的错误
 * @param s
 * @return
 */
template<typename StructT>
int DangerousMemset(StructT* s)
{
	memset(&s, 0, sizeof(s)); // 重构的时候，直接从下面的Test() copy过来
	return 0;
}

int Test()
{
	TestStruct s;
	s.one = 1;
	s.two = 2;
	s.three = 3;
	std::cout << "before memset:" << std::endl;
	std::cout << &s;
	memset(&s, 0, sizeof(s));
	std::cout << "after memset:" << std::endl;
	std::cout << &s;
	return 0;
}

int DangerousTest()
{
	TestStruct s;
	s.one = 1;
	s.two = 2;
	s.three = 3;
	std::cout << "before memset:" << std::endl;
	std::cout << &s;
	DangerousMemset(&s);
	std::cout << "after memset:" << std::endl;
	std::cout << &s;
	return 0;
}

int main()
{
	Test();
	DangerousTest();
}
```



