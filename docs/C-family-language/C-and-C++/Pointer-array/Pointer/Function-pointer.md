# Function pointer



## 从stored-program computer来理解function pointer

参见: 

- 工程Hardware的`Computer-architecture\Function and data model`章节

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

