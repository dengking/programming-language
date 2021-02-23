# [C++ Source](https://sean-parent.stlab.cc/presentations/2013-03-06-value_semantics/value-semantics.cpp)



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



## 实现分析

有了type erasure的实现思路，再来看concept-based Polymorphism的实现就非常容易了。下面是它的实现中使用到的关键。

### Type erasure

这些都是在`Type-erasure`章节中总结的:

1、template constructor

2、OOP interface + template implementation

### Model concept



### 使用overload、function template来实现static polymorphic

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

```c++
void draw(const document_t &x, ostream &out, size_t position)
{
	out << string(position, ' ') << "<document>" << endl;
	for (auto &e : x)
		draw(e, out, position + 2);
	out << string(position, ' ') << "</document>" << endl;
}
```

overload: 

```C++
void draw(const my_class_t&, ostream &out, size_t position)
{
	out << string(position, ' ') << "my_class_t" << endl;
}
```

