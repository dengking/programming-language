# Pointer to member

## 素材

### cppreference  [Pointer declaration](https://pd.codechef.com/docs/cpp/cpp/language/pointer.html)

> NOTE: 
>
> 其中的对pointer to member进行了非常好的解释

### cppreference [Non-static data members # Usage](https://pd.codechef.com/docs/cpp/cpp/language/data_members.html)



### stackoverflow [Pointer to class data member “::*”](https://stackoverflow.com/questions/670734/pointer-to-class-data-member)

I came across this strange code snippet which compiles fine:

```cpp
class Car
{
public:
	int speed;
};

int main()
{
	int Car::*pSpeed = &Car::speed;
	return 0;
}
// g++ test.cpp

```



#### [A](https://stackoverflow.com/a/670744)

It's a "pointer to member" - the following code illustrates its use:

```cpp
#include <iostream>
using namespace std;

class Car
{
public:
	int speed;
};

int main()
{
	int Car::*pSpeed = &Car::speed;

	Car c1;
	c1.speed = 1;       // direct access
	cout << "speed is " << c1.speed << endl;
	c1.*pSpeed = 2;     // access via pointer to member
	cout << "speed is " << c1.speed << endl;
	return 0;
}
// g++ test.cpp

```

As to *why* you would want to do that, well it gives you another level of indirection that can solve some tricky problems. But to be honest, I've never had to use them in my own code.

**Edit:** I can't think off-hand of a convincing use for pointers to member data. Pointer to member functions can be used in pluggable architectures, but once again producing an example in a small space defeats me. The following is my best (untested) try - an Apply function that would do some pre &post processing before applying a user-selected member function to an object:

```cpp
void Apply(SomeClass *c, void (SomeClass::*func)())
{
	// do hefty pre-call processing
	(c->*func)();  // call user specified function
	// do hefty post-call processing
}

```

The parentheses around `c->*func` are necessary because the `->*` operator has lower precedence than the function call operator.



## Example

1、More C++ Idioms/[Member Detector](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector)

