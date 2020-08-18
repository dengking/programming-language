# Function pointer



## 从stored-program computer来理解function pointer

需要从program的本质出发来对其进行理解

 [Stored-program computer](https://en.wikipedia.org/wiki/Stored-program_computer) 启发了我们 stores [program instructions](https://en.wikipedia.org/wiki/Instruction_(computer_science)) in electronic memory. 所以我们的所编写的function，最终也是会存入到 RAM中的，因此，它们就和数据一样，可以通过pointer对其进行access；所以这就是function pointer的本质所在；

显然函数是可以被调用而执行的，函数的执行将在内存中stack区；



## cppreference [Pointers to functions](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_functions)

Because of the [function-to-pointer](https://en.cppreference.com/w/cpp/language/implicit_cast) implicit conversion, the address-of operator is optional:

```c++
void f(int)
{

}

int main()
{

	void (*p1)(int) = &f;
	void (*p2)(int) = f; // same as &f
}
// g++ test.cpp

```

