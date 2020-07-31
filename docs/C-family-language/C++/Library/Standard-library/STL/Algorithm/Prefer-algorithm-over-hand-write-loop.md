# Prefer algorithm over hand-write loop



## drdobbs [STL Algorithms vs. Hand-Written Loops](https://www.drdobbs.com/stl-algorithms-vs-hand-written-loops/184401446)



Internally, then, algorithms are loops. Furthermore, the breadth of STL algorithms means that many tasks you might naturally code as loops could also be written using algorithms. For example, if you have a **Widget** class that supports redrawing,

```C++
class Widget {
public:

  void redraw() const
  {
      
  }

};
```

and you’d like to redraw all the **Widget**s in a list, you could do it with a loop, like this,

```c++
list<Widget> lw;
for (list<Widget>::iterator i =lw.begin(); i != lw.end(); ++i)
{
	i->redraw();
}
```

but you could also do it with the **for_each** algorithm:

```c++
for_each(lw.begin(), lw.end(), mem_fun_ref(&Widget::redraw));
```

> NOTE: [std::mem_fun_ref](https://en.cppreference.com/w/cpp/utility/functional/mem_fun_ref)

### Example 1 `for` VS `for_each`

```c++
#include <algorithm>
#include <iostream>
#include <list>

class Widget
{
public:
	Widget(int i)
			: m_M(i)
	{
	}
	void redraw() const
	{
		std::cout << "draw:" << m_M << std::endl;
	}
	int m_M { 0 };
};

std::list<Widget> MakeWidgetList()
{
	std::list<Widget> l;
	for (auto i : { 1, 2, 3, 4, 5, 6, 7, 8 })
	{
		l.emplace_back(i);
	}
	return l;
}
void HandWriteLoop()
{

	for (auto&& W : MakeWidgetList())
	{
		W.redraw();
	}
}

void Algorithm()
{
	std::list<Widget> lw = MakeWidgetList();
	for_each(lw.begin(), lw.end(),
			[=] (const Widget& W)
			{
				W.redraw();
			});
}

int main()
{
	std::cout << "HandWriteLoop:\n";
	HandWriteLoop();
	std::cout << "Algorithm:\n";
	Algorithm();
}
// g++ --std=c++11 test.cpp
```

### Why prefer algorithm?

For many C++ programmers, writing the loop is more natural than calling the algorithm, and reading the loop is more comfortable than making sense of **`mem_fun_ref`** and the taking of **`Widget::redraw`**’s address. Yet this article argues that the algorithm call is preferable. In fact, this article argues that calling an algorithm is usually preferable to *any* hand-written loop. Why?

There are three reasons:

- **Efficiency:** Algorithms are often more efficient than the loops programmers produce.
- **Correctness:** Writing loops is more subject to errors than calling algorithms.
- **Maintainability:** Algorithm calls often yield code that is clearer and more straightforward than the corresponding explicit loops.

The remainder of this article lays out the case for algorithms.

### Efficiency

From an **efficiency** perspective, algorithms can beat explicit loops in three ways, two major, one minor. 

#### The minor efficiency argument

The minor way involves the elimination of redundant computations. Look again at the loop we just saw:

```c++
list<Widget> lw;
for (list<Widget>::iterator i =lw.begin(); i != lw.end(); ++i)
{
	i->redraw();
}
```

I’ve highlighted the **loop termination test** to emphasize that each time around the loop, **`i`** will be checked against **`lw.end()`**. That means that each time around the loop, the function **`list::end`** will be invoked. But we don’t need to call **`end`** more than once, because we’re not modifying the list. A single call to **`end`** would suffice, and, if we look again at the algorithm invocation, we’ll see that that’s exactly how many times **`end`** is evaluated:

```c++
// this call evaluates lw.end() exactly
// once
for_each(lw.begin(), lw.end(), mem_fun_ref(&Widget::redraw));
```

To be fair, STL implementers understand that **`begin`** and **`end`** (and similar functions, such as **size**) are used frequently, so they’re likely to design them for maximal efficiency. They’ll almost certainly **inline** them and strive to code them so that most compilers will be able to avoid repeated computations by hoisting（提升） their results out of loops like the one above. Experience shows that implementers don’t always succeed, however, and when they don’t, the avoidance of repeated computations is enough to give the algorithm a performance edge over the hand-written loop.

#### The first major efficiency argument

But that’s the minor efficiency argument. The first major argument is that library implementers can take advantage of their knowledge of container implementations to optimize traversals in a way that no library user ever could. For example, the objects in a **deque** are typically stored (internally) in one or more fixed-size arrays. **Pointer-based traversals** of these arrays are faster than **iterator-based traversals**, but only library implementers can use **pointer-based traversals**, because only they know the size of the internal arrays and how to move from one array to the next. Some STLs contain algorithm implementations that take their **deque**’s internal data structures into account, and such implementations have been known to clock in at more than 20 percent faster than the “normal” implementations of the algorithms.

The point is not that STL implementations are optimized for **deque**s (or any other specific container type), but that implementers know more about their implementations than you do, and they can take advantage of this knowledge in algorithm implementations. If you shun algorithm calls in favor of your own loops, you forgo the opportunity to benefit from any implementation-specific optimizations they may have provided.

#### The second major efficiency argument 

The second major efficiency argument is that all but the most trivial STL algorithms use computer science algorithms that are more sophisticated — sometimes *much* more sophisticated — than anything the average C++ programmer will be able to come up with. It’s next to impossible to beat **sort** or its kin(同族) (e.g., **stable_sort**, **nth_element**, etc.); the search algorithms for sorted ranges (e.g., **binary_search**, **lower_bound**, etc.) are equally good; and even such mundane tasks as eliminating objects from **vector**s, **deque**s, and arrays are more efficiently accomplished using the **erase-remove** idiom than the loops most programmers come up with.

> NOTE: 关于**erase-remove** idiom，参见`C++\Idiom\Container\Erase_if\Erase-remove`

If the efficiency argument for algorithms doesn’t persuade you, perhaps you’re more amenable to a plea based on correctness. One of the trickier（棘手） things about writing your own loops is making sure you use only iterators that (a) are valid and (b) point where you want them to. For example, suppose you have an array, and you’d like to take each array element, add 41 to it, then insert it into the front of a **deque**. Writing your own loop, you might come up with this:

> NOTE: 第一段话的意思：如果对算法的效率论证不能说服你，也许你更愿意接受基于正确性的辩护。

#### Example 2 `insert(d.begin())`

```c++
#include "stddef.h"
#include <deque>
#include <iostream>

const int maxNumDoubles = 5;
using namespace std;
// C API: this function takes a pointer
// to an array of at most arraySize
// doubles and writes data to it. It
// returns the number of doubles written.
size_t fillArray(double *pArray, size_t arraySize)
{
	for (int i = 0; i < arraySize; ++i)
	{
		*pArray = i;
		pArray++;
	}
	return arraySize;
}

int main()
{
	// create local array of max possible size
	double data[maxNumDoubles];

	// create deque, put data into it
	deque<double> d;
	// get array data from API
	size_t numDoubles = fillArray(data, maxNumDoubles);
	std::cout << "data in array \n";
	for (auto&& i : data)
	{
		std::cout << i << std::endl;
	}
// for each i in data, insert data[i]+41
// at the front of d; this code has a bug!
	for (size_t i = 0; i < numDoubles; ++i)
	{
		d.insert(d.begin(), data[i] + 41);
	}
	std::cout << "data in deque\n";
	for (auto&& i : d)
	{
		std::cout << i << std::endl;
	}
}
// g++ --std=c++11 test.cpp
```

> NOTE: 输出如下:
>
> ```c++
> data in array 
> 0
> 1
> 2
> 3
> 4
> data in deque
> 45
> 44
> 43
> 42
> 41
> ```
>
> 

This works, as long as you’re happy with a result where the newly inserted elements are in the reverse order of the corresponding elements in **`data`**. Because each insertion location is **`d.begin()`**, the last element inserted will go at the front of the **deque**!

If that’s not what you wanted (and admit it, it’s not), you might think to fix it like this:

#### Example 3 iterator-invalidation

```c++
#include "stddef.h"
#include <deque>
#include <iostream>

const int maxNumDoubles = 5;
using namespace std;
// C API: this function takes a pointer
// to an array of at most arraySize
// doubles and writes data to it. It
// returns the number of doubles written.
size_t fillArray(double *pArray, size_t arraySize)
{
	for (int i = 0; i < arraySize; ++i)
	{
		*pArray = i;
		pArray++;
	}
	return arraySize;
}

int main()
{
	// create local array of max possible size
	double data[maxNumDoubles];

	// create deque, put data into it
	deque<double> d;
	// get array data from API
	size_t numDoubles = fillArray(data, maxNumDoubles);
	std::cout << "data in array \n";
	for (auto&& i : data)
	{
		std::cout << i << std::endl;
	}
	// remember d’s begin iterator
	deque<double>::iterator insertLocation = d.begin();
	for (size_t i = 0; i < numDoubles; ++i)
	{
		// insert data[i]+41 at insertLocation, then
		// increment insertLocation; this code is also buggy!
		d.insert(insertLocation++, data[i] + 41);
	}
	std::cout << "data in deque\n";
	for (auto&& i : d)
	{
		std::cout << i << std::endl;
	}
}
// g++ --std=c++11 test.cpp
```

> NOTE: 输出如下:
>
> ```
> 0
> 1
> 2
> 3
> 4
> data in deque
> 41
> 41
> 41
> 41
> 41
> ```
>
> 

This looks like a double win, because it not only increments the iterator specifying the insertion position, it also eliminates the need to call **`begin`** each time around the loop; that eliminates the minor efficiency hit we discussed earlier. Alas（可叹）, this approach runs into a different problem: it yields undefined results. Each time **`deque::insert`** is called, it invalidates all iterators into the **deque**, and that includes **`insertLocation`**. After the first call to **`insert`**, **`insertLocation`** is invalidated, and subsequent loop iterations are allowed to head straight to looneyland（发疯的）.

#### Example 4 correct-for

Once you puzzle this out, you might come up with the following:

```C++
#include "stddef.h"
#include <deque>
#include <iostream>

const int maxNumDoubles = 5;
using namespace std;
// C API: this function takes a pointer
// to an array of at most arraySize
// doubles and writes data to it. It
// returns the number of doubles written.
size_t fillArray(double *pArray, size_t arraySize)
{
	for (int i = 0; i < arraySize; ++i)
	{
		*pArray = i;
		pArray++;
	}
	return arraySize;
}

int main()
{
	// create local array of max possible size
	double data[maxNumDoubles];

	// create deque, put data into it
	deque<double> d;
	// get array data from API
	size_t numDoubles = fillArray(data, maxNumDoubles);
	std::cout << "data in array \n";
	for (auto&& i : data)
	{
		std::cout << i << std::endl;
	}
	// remember d’s begin iterator
	deque<double>::iterator insertLocation = d.begin();
	for (size_t i = 0; i < numDoubles; ++i)
	{
		// insert data[i]+41 at insertLocation, then
		// increment insertLocation; this code is also buggy!
		insertLocation = d.insert(insertLocation, data[i] + 41);
		++insertLocation;
	}
	std::cout << "data in deque\n";
	for (auto&& i : d)
	{
		std::cout << i << std::endl;
	}
}

```

This code finally does what you want, but think about how much work it took to get here! Compare that to the following call to **transform**:

#### Example 5 `std::transform`



```c++
#include "stddef.h"
#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>

const int maxNumDoubles = 5;
using namespace std;
// C API: this function takes a pointer
// to an array of at most arraySize
// doubles and writes data to it. It
// returns the number of doubles written.
size_t fillArray(double *pArray, size_t arraySize)
{
	for (int i = 0; i < arraySize; ++i)
	{
		*pArray = i;
		pArray++;
	}
	return arraySize;
}

int main()
{
	// create local array of max possible size
	double data[maxNumDoubles];

	// create deque, put data into it
	deque<double> d;
	// get array data from API
	size_t numDoubles = fillArray(data, maxNumDoubles);
	std::cout << "data in array \n";
	for (auto&& i : data)
	{
		std::cout << i << std::endl;
	}
	// copy all elements from data to the
	// front of d, adding 41 to each
	transform(data, data + numDoubles, inserter(d, d.begin()), bind2nd(plus<int>(), 41));
	std::cout << "data in deque\n";
	for (auto&& i : d)
	{
		std::cout << i << std::endl;
	}
}

```

The “**`bind2nd(plus<int>(), 41)`**” might take you a couple of minutes to get right (especially if you don’t use STL’s binders very often), but the only iterator-related worries you have are specifying the beginning and end of the source range (which was never a problem) and being sure to use **`inserter`** as the beginning of the destination range. In practice, figuring out the correct initial iterators for source and destination ranges is usually easy, or at least a lot easier than making sure the body of a loop doesn’t inadvertently（无意的） invalidate an iterator you need to keep using.