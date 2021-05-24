# Implicitly-defined

对于[Constructor](./Constructor.md)、[Destructor](./Destructor/Destructor.md)、[Assignment](./Assignment/Assignment.md)，compiler在user没有指定的情况下，会提供默认实现，当然，这种默认实现是不一定保证正确的。

## draft: implicit defined copy是shallow copy

可能导致double free，例子: 

1、drdobbs [C++ Made Easier: The Rule of Three](https://www.drdobbs.com/c-made-easier-the-rule-of-three/184401400)

2、在HS的时候，遇到过类似的错误: "implicit-define-copy-constructor-shallow-copy-double-free"

3、从ownership的角度来进行思考，"implicit defined copy constructor shallow copy double free"问题，其实就是典型对ownership描述不清而导致的问题，它到底是unique ownership还是shared ownership？显然raw pointer无法描述清楚，shallow copy对应的更多是shared ownership，而raw pointer无法表达shared ownership，它会导致double free。

## draft: compiler不生成movable的规则

1、stackoverflow [Rule of Zero confusion?](https://stackoverflow.com/questions/44997955/rule-of-zero-confusion) # [A](https://stackoverflow.com/a/45016295) 

As Scott Meyers pointed out, if you do have to add a destructor for whatever reason, the implicit generation of move constructors and move assignment operator are disabled, even if the compiler could generate them.

> NOTE: 
>
> 1、一旦用户添加了destructor，则compiler就不再implicit define movable



## draft: cppreference [The rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three)

### Rule of three

The implicitly-defined special member functions are typically incorrect if the class manages a resource whose handle is an object of non-class type (raw pointer, POSIX file descriptor, etc), whose destructor does nothing and copy constructor/assignment operator performs a "shallow copy" (copy the value of the handle, without duplicating the underlying resource).

> NOTE: 
>
> 1、"implicit-define-copy-constructor-shallow-copy-double-free"
>
> 2、"implicit-defined-destructor-do nothing-resource leak"



### Rule of zero

When a base class is intended for polymorphic use, its destructor may have to be declared public and virtual. This blocks implicit moves (and deprecates implicit copies), and so the special member functions have to be declared as defaulted[[2\]](https://en.cppreference.com/w/cpp/language/rule_of_three#cite_note-2)

> NOTE: 
>
> 下面的是的测试程序，和上面描述的不同:
>
> ````C++
> #include<iostream>
> #include<memory>
> #include<utility>
> #include <vector>
> class foo
> {
> public:
> 	/**
> 	 * @brief user defined destructor
> 	 *
> 	 */
> 	virtual ~foo()
> 	{
> 		std::cout << __PRETTY_FUNCTION__ << std::endl;
> 	}
> private:
> 	int x = 10;
> 	std::vector<int> v = { 1, 2, 3, 4, 5 };
> };
> 
> void bar(foo foo)
> {
> }
> int main()
> {
> 	foo f;
> 	foo f1 = f;
> 	foo f2(std::move(f));
> 	foo f3 = std::move(f);
> 	bar(f3);
> 	bar(std::move(f3));
> }
> // g++ test.cpp -pedantic -Wall -Wextra --std=c++11
> 
> ````
>
> 显然，user defined destructor并没有"blocks implicit moves"
>
> 



## draft: move disable copy

## TODO



### stackoverflow [Conditions under which compiler will not define implicits (constructor, destructor, copy constructor, copy assignment) [duplicate]](https://stackoverflow.com/questions/15590832/conditions-under-which-compiler-will-not-define-implicits-constructor-destruct)



### stackoverflow [Conditions for automatic generation of default/copy/move ctor and copy/move assignment operator?](https://stackoverflow.com/questions/4943958/conditions-for-automatic-generation-of-default-copy-move-ctor-and-copy-move-assi)



### stackoverflow [Why user-defined move-constructor disables the implicit copy-constructor?](https://stackoverflow.com/questions/11255027/why-user-defined-move-constructor-disables-the-implicit-copy-constructor)

> NOTE: 