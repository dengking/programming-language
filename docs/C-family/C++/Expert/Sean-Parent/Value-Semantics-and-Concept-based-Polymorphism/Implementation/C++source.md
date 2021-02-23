# [C++ Source](https://sean-parent.stlab.cc/presentations/2013-03-06-value_semantics/value-semantics.cpp)

## Source code

```C++
/*
 Copyright 2013 Adobe Systems Incorporated
 Distributed under the MIT License (see license at
 http://stlab.adobe.com/licenses.html)

 This file is intended as example code and is not production quality.
 */

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

/******************************************************************************/
// Library
template<typename T>
void draw(const T &x, ostream &out, size_t position)
{
	out << string(position, ' ') << x << endl;
}

class object_t
{
public:
	template<typename T>
	object_t(T x) :
					self_(make_shared<model<T>>(move(x)))
	{
	}

	friend void draw(const object_t &x, ostream &out, size_t position)
	{
		x.self_->draw_(out, position);
	}

private:
	struct concept_t
	{
		virtual ~concept_t() = default;
		virtual void draw_(ostream&, size_t) const = 0;
	};
	template<typename T>
	struct model: concept_t
	{
		model(T x) :
						data_(move(x))
		{
		}
		void draw_(ostream &out, size_t position) const
		{
			draw(data_, out, position);
		}

		T data_;
	};

	shared_ptr<const concept_t> self_;
};

using document_t = vector<object_t>;

void draw(const document_t &x, ostream &out, size_t position)
{
	out << string(position, ' ') << "<document>" << endl;
	for (auto &e : x)
		draw(e, out, position + 2);
	out << string(position, ' ') << "</document>" << endl;
}

using history_t = vector<document_t>;

void commit(history_t &x)
{
	assert(x.size());
	x.push_back(x.back());
}
void undo(history_t &x)
{
	assert(x.size());
	x.pop_back();
}
document_t& current(history_t &x)
{
	assert(x.size());
	return x.back();
}

/******************************************************************************/
// Client
class my_class_t
{
	/* ... */
};

void draw(const my_class_t&, ostream &out, size_t position)
{
	out << string(position, ' ') << "my_class_t" << endl;
}

int main()
{
	history_t h(1);

	current(h).emplace_back(0);
	current(h).emplace_back(string("Hello!"));

	draw(current(h), cout, 0);
	cout << "--------------------------" << endl;

	commit(h);

	current(h).emplace_back(current(h));
	current(h).emplace_back(my_class_t());
	current(h)[1] = string("World");

	draw(current(h), cout, 0);
	cout << "--------------------------" << endl;

	undo(h);

	draw(current(h), cout, 0);
}
// g++ --std=c++11 -Wall -pedantic test.cpp && ./a.out

```



## 如何理解"Value Semantics and Concept-based Polymorphism"？

如何理解"Value Semantics and Concept based Polymorphism"？主要是要理解:

> NOTE: "Value Semantics and Concept-based Polymorphism"有如下理解方式:
>
> 1、Value Semantics、Concept-based Polymorphism
>
> 2、Value Semantics 和 Concept-based一起修饰Polymorphism
>
> 我目前是按照2来进行说明的

1、Value Semantics Polymorphism

2、Concept based Polymorphism



### Concept based Polymorphism

如何理解"Concept based Polymorphism"？我是通过source code中的`struct concept_t`、`struct model: concept_t`发现答案的。

Source code中的`struct concept_t`、`struct model: concept_t`让我想起来之前阅读的如下内容:

1、More C++ Idioms/[Polymorphic Value Types](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Value_Types) 

这篇文章其实就是对此的介绍，其中有这样的描述:

> ### Also Known As
>
> Run-time Concept

上述"Concept"是经典generic programming中的概念，C++中generic programming是static的，而上述"**Run-time** Concept"表明它借用了generic programming的概念，实现了run-time版的concept。

2、boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html)时，其中有这样的描述:

> A **concept** is a set of requirements consisting of valid expressions, associated types, invariants, and complexity guarantees. A type that satisfies the requirements is said to **model** the concept.

上面完整地介绍了concept、model之间的关系，显然它们都是GP中的概念



---

下面是总结: 

1、结合上述内容和source code，其实大致就理解了"Concept based Polymorphism"的含义

2、需要注意的是: Concept based Polymorphism是一种dynamic polymorphism，因为 `struct concept_t`、`struct model: concept_t` 采用的是OOP interface + template implementation。



### Value Semantics Polymorphism

对于C++ dynamic polymorphic，我们的经典认知是: 只有通过reference semantic、OOP interface + subclass implementation才能够实现dynamic polymorphic，而通过上述source code可以看出，它实现了dynamic polymorphic，但是并没有使用:

1、reference semantic

2、OOP interface + subclass implementation

```C++
using document_t = vector<object_t>;
```

是value semantic，而不是reference semantic。

显然， 这就是"Value Semantics Polymorphism"的含义。



### 通过用法验证上述分析

下面通过`main()`中的使用来验证前面的分析。

```C++
int main()
{
	/**
	 * 加入type 为 int的object
	 */
	current(h).emplace_back(0);
	/**
	 * 加入type 为 string的object
	 */
	current(h).emplace_back(string("Hello!"));

	draw(current(h), cout, 0);
}    
```

下面是`draw(current(h), cout, 0)`的实现，它是"Value Semantics and Concept-based Polymorphism"发生的地方: 

```C++
void draw(const document_t &x, ostream &out, size_t position)
{
	out << string(position, ' ') << "<document>" << endl;
	for (auto &e : x)
		draw(e, out, position + 2);
	out << string(position, ' ') << "</document>" << endl;
}
```

```C++
	/**
	 * @brief 这里使用的是friend function，其实也可以修改为member function
	 *
	 * @param x
	 * @param out
	 * @param position
	 */
	friend void draw(const object_t &x, ostream &out, size_t position)
	{
		/**
		 * dynamic polymorphism
		 */
		x.self_->draw_(out, position);
	}
```



1、上述 int object、`std::string` object之间没有inheritance关系

2、 int object、`std::string` object都是value semantic

3、实现了dynamic polymorphism

## 实现分析

有了type erasure的实现思路，再来看concept-based Polymorphism的实现就非常容易了。下面是它的实现中使用到的关键。



### Type erasure

它的实现是运用了type erasure的技术的，这些technique都是在`Type-erasure`章节中总结过的，主要是: 

1、template constructor

```C++
	/**
	 * @brief template constructor
	 *
	 * @tparam T
	 * @param x
	 */
	template<typename T>
	object_t(T x) :
					self_(make_shared<model<T>>(move(x)))
	{
	}
```



2、OOP interface + template implementation

```C++
	struct concept_t
	{
		virtual ~concept_t() = default;
		virtual void draw_(ostream&, size_t) const = 0;
	};
	template<typename T>
	struct model: concept_t
	{
		model(T x) :
						data_(move(x))
		{
		}
		void draw_(ostream &out, size_t position) const
		{
			/**
			 * static polymorphism: overload
			 */
			draw(data_, out, position);
		}
		/**
		 * 这里使用的是value，而不是pointer
		 */
		T data_;
	};
```



### External polymorphism and static polymorphism

1、使用overload、function template来实现static polymorphism，

2、此处强调 external polymorphism 的原因是，我觉得上述它的这种写法是非常 类似于 `Adapter-pattern\External-polymorphism-pattern` 章节中总结的实现方法的。

function template: 

```C++
// Library
template<typename T>
void draw(const T &x, ostream &out, size_t position)
{
	out << string(position, ' ') << x << endl;
}
```



overload: 

```C++
void draw(const my_class_t&, ostream &out, size_t position)
{
	out << string(position, ' ') << "my_class_t" << endl;
}
```

