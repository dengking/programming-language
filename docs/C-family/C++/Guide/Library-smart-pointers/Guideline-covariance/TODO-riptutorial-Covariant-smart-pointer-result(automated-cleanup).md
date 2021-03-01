# riptutorial [3. Covariant smart pointer result (automated cleanup).](https://riptutorial.com/cplusplus/example/19282/3--covariant-smart-pointer-result--automated-cleanup--)

```c++
#include <iostream>
#include <memory>

using namespace std;
using std::unique_ptr;

template<class Type>
auto up(Type* p)
{
	return unique_ptr<Type>(p);
}

class Top
{
private:
	virtual Top* virtual_clone() const = 0;

public:
	unique_ptr<Top> clone() const
	{
		return up(virtual_clone());
	}

	virtual ~Top() = default;       // Necessary for `delete` via Top*.
};

class D: public Top
{
private:
	/* ← Covariant return */
	D* virtual_clone() const override
	{
		return new D(*this);
	}

public:
	/* ← Apparent covariant return */
	unique_ptr<D> clone() const
	{
		return up(virtual_clone());
	}
};

class DD: public D
{
private:
	int answer_ = 42;
	/* ← Covariant return */
	DD* virtual_clone() const override
	{
		return new DD(*this);
	}

public:
	int answer() const
	{
		return answer_;
	}

	/* ← Apparent covariant return */
	unique_ptr<DD> clone() const
	{
		return up(virtual_clone());
	}
};

int main()
{
	auto p1 = unique_ptr<DD>(new DD());
	auto p2 = p1->clone();
	// Correct dynamic type DD for *p2 is guaranteed by the static type checking.

	cout << p2->answer() << endl;          // "42"
	// Cleanup is automated via unique_ptr.
}

```

