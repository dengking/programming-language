# Double free

“double free”即重复释放一块内存，本文对它进行讨论。

## What is double free

在[What does “double free” mean?](https://stackoverflow.com/questions/21057393/what-does-double-free-mean)的[A](https://stackoverflow.com/a/21057524)中给出了较好回答

> A double free in C, technically speaking, leads to *undefined behavior*. This means that the program can behave completely arbitrarily and all bets are off about what happens. That's certainly a bad thing to have happen! In practice, double-freeing a block of memory will corrupt the state of the memory manager, which might cause existing blocks of memory to get corrupted or for future allocations to fail in bizarre ways (for example, the same memory getting handed out on two different successive calls of `malloc`).
>
> Double frees can happen in all sorts of cases. A fairly common one is when multiple different objects all have pointers to one another and start getting cleaned up by calls to `free`. When this happens, if you aren't careful, you might `free` the same pointer multiple times when cleaning up the objects. There are lots of other cases as well, though.
>
> Hope this helps!

## Example of double free

### Shallow copy

在文章[C++ Made Easier: The Rule of Three](https://www.drdobbs.com/c-made-easier-the-rule-of-three/184401400)给出了一个非常典型的C++例子：

```c++
// This class contains a subtle error
class IntVec {
public:
   IntVec(int n): data(new int[n]) { }
   ~IntVec() { delete[] data; };
   int& operator[](int n)
      { return data[n]; }
   const int& operator[](int n) const
      { return data[n]; }

private:
   int* data;
};

int main()
{
   IntVec x(100);
   IntVec y = x;   // Trouble!
   return 0;
}
```

> The reason for the trouble is that the **IntVec** class does not explicitly define a copy constructor. When a class does not define a copy constructor, the implementation synthesizes one, defining copying an object of the class in terms of copying the members of the class. In other words, when we initialize **y** as a copy of **x**, the implementation handles that definition by initializing **y.data** to be a copy of **x.data**. Forming this copy is not harmful by itself. However, when the program terminates, the local variables **x** and **y** will both be destroyed, which will result in executing **delete[]** on **x.data** and **y.data**. Because **x.data** and **y.data** have the same value, these two **delete[]** operations will try to free the same memory twice, the effect of which is undefined.

#### 运行结果

运行上述程序，进行回crash，产生core dump

```
Program terminated with signal 6, Aborted.
#0  0x00007f4f372d5207 in raise () from /lib64/libc.so.6
Missing separate debuginfos, use: debuginfo-install glibc-2.17-260.el7.x86_64 libgcc-4.8.5-39.el7.x86_64 libstdc++-4.8.5-39.el7.x86_64
(gdb) where
#0  0x00007f4f372d5207 in raise () from /lib64/libc.so.6
#1  0x00007f4f372d68f8 in abort () from /lib64/libc.so.6
#2  0x00007f4f37317d27 in __libc_message () from /lib64/libc.so.6
#3  0x00007f4f37320489 in _int_free () from /lib64/libc.so.6
#4  0x000000000040066f in IntVec::~IntVec() ()
#5  0x00000000004005fc in main ()

```

这是典型的因为shallow copy导致被double free的错误。

可以看到，`delete` operator会被编译为调用`/lib64/libc.so`中的`_int_free`函数，这个函数中应该有在下一节中讲述的机制，然后一旦发现异常情况，就调用`abort`，`raise`来发送signal 6。其实还是比较好的情况，在下一节中，描述了另外一种更加隐晦、严重的情况。



### 错误地使用[`std::shard_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 

错误地使用[`std::shard_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 也是可能导致double free的，这个例子源自[learncpp 15.6 — std::shared_ptr](https://www.learncpp.com/cpp-tutorial/15-6-stdshared_ptr/)：

```c++
#include <iostream>
#include <memory> // for std::shared_ptr
 
class Resource
{
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};
 
int main()
{
	Resource *res = new Resource;
	std::shared_ptr<Resource> ptr1(res);
	{
		std::shared_ptr<Resource> ptr2(res); // create ptr2 directly from res (instead of ptr1)
 
		std::cout << "Killing one shared pointer\n";
	} // ptr2 goes out of scope here, and the allocated Resource is destroyed
 
	std::cout << "Killing another shared pointer\n";
 
	return 0;
} // ptr1 goes out of scope here, and the allocated Resource is destroyed again
```

> The difference here is that we created two std::shared_ptr independently from each other. As a consequence, even though they’re both pointing to the same Resource, they aren’t aware of each other. When ptr2 goes out of scope, it thinks it’s the only owner of the Resource, and deallocates it. When ptr1 later goes out of the scope, it thinks the same thing, and tries to delete the Resource again. Then bad things happen.

### 错误地释放automatic variable导致double free

```C++
#include <bits/stdc++.h>
using namespace std;

//Definition for singly-linked list.
struct ListNode
{
	int val;
	ListNode *next;
	ListNode() :
					val(0), next(nullptr)
	{
	}
	ListNode(int x) :
					val(x), next(nullptr)
	{
	}
	ListNode(int x, ListNode *next) :
					val(x), next(next)
	{
	}
};

class Solution
{
public:
	ListNode* removeDuplicates(ListNode *head, bool delete_flag = false)
	{
		if (head == nullptr)
		{
			return head;
		}
		ListNode *fast = head->next, *slow = head;
		while (fast)
		{
			if (fast->val != slow->val)
			{
				ListNode *delete_head = slow->next; // 待删除子链表的头
				slow->next = fast;
				slow = slow->next;
				/**
				 * 需要将两者之间的node全部都删除，因为它们都是重复的值
				 */
				if (delete_flag)
				{
					while (delete_head != nullptr && delete_head != fast)
					{
						ListNode *n = delete_head;
						delete_head = delete_head->next; // 迭代
						cout << "删除:" << n->val << endl;
						delete n; // 删除掉对应的节点
					}
				}
			}
			fast = fast->next;
		}
		slow->next = nullptr;
		return head;
	}
};

ostream& operator <<(ostream &stream, ListNode *head)
{
	while (head)
	{
		stream << head->val << " ";
		head = head->next;
	}
	return stream;
}

void test1()
{
	ListNode *N1 = new ListNode { 1 };
	ListNode *N2 = new ListNode { 1 };
	ListNode *N3 = new ListNode { 2 };
	ListNode *N4 = new ListNode { 2 };
	ListNode *N5 = new ListNode { 5 };
	ListNode *N6 = new ListNode { 6 };
	N1->next = N2;
	N2->next = N3;
	N3->next = N4;
	N4->next = N5;
	N5->next = N6;
	cout << N1 << endl;
	Solution s;
	cout << s.removeDuplicates(N1, true) << endl;
}

void test2()
{
	ListNode N1 { 1 };
	ListNode N2 { 1 };
	ListNode N3 { 2 };
	ListNode N4 { 2 };
	ListNode N5 { 5 };
	ListNode N6 { 6 };
	N1.next = &N2;
	N2.next = &N3;
	N3.next = &N4;
	N4.next = &N5;
	N5.next = &N6;
	cout << &N1 << endl;
	Solution s;
	cout << s.removeDuplicates(&N1, true) << endl;
}
int main()
{
	test1();
	test2();
}
// g++ test.cpp -pedantic -Wall -Wextra --std=c++11

```



上述程序运行时，会出现如下错误:

```
double free or corruption (out)
Aborted
```

问题的根源在于在 `ListNode* removeDuplicates(ListNode *head)` 中，释放了链表的节点，而这些节点是分配于stack上的。

## Why does free crash when called twice?

### 分析一

在[Why does free crash when called twice?](https://stackoverflow.com/questions/3117615/why-does-free-crash-when-called-twice)的[A](https://stackoverflow.com/a/3117638)中给出的回答较好：

You are not allowed to call `free` on **unallocated memory**, the standard states that quite clearly (slightly paraphrased, my emphasis):

> The `free` function causes the space pointed to by its argument to be deallocated, that is, made available for further allocation. If the argument is a **null pointer**, no action occurs. Otherwise, if the argument does not match a **pointer** earlier returned by a **memory management function**, or ***if the space has been deallocated by a call to `free` or `realloc`, the behavior is undefined.***

> NOTE: 其实本节所提问题的答案是：没有为什么，这是标准规定的。

What happens, for example, if the address you're double-freeing has been reallocated in the middle of a new block and the code that allocated it just happened to store something there that looked like a real malloc-block header? Like:

```
 +- New pointer    +- Old pointer
 v                 v
+------------------------------------+
|                  <Dodgy bit>       |
+------------------------------------+
```

Chaos, that's what.

Memory allocation functions are a tool just like a chainsaw（电锯） and, provided you use them correctly, you should have no problems. If you misuse them, however, the consequences are your own fault, either corrupting memory or worse, or cutting off one of your arms :-)

------

And regarding the comment:

> ... it can communicate gracefully to enduser about the doubling free the same location.

Short of keeping a record of all `malloc` and `free` calls to ensure you don't double-free a block, I can't see this as being workable. It would require a huge overhead and *still* wouldn't fix all the problems.

What would happen if:

- thread A allocated and freed memory at address 42.
- thread B allocated memory a address 42 and started using it.
- thread A freed that memory a second time.
- thread C allocated memory a address 42 and started using it.

You then have threads B and C both thinking they own that memory (these don't have to be threads of execution, I'm using the term thread here as just a piece of code that runs - it could all be in the one thread of execution but called sequentially).

No, I think the current `malloc` and `free` are just fine provided you use them properly. By all means give some thought to implementing your own version, I see nothing wrong with that but I suspect you'll run into some pretty thorny performance issues.

------

If you *do* want to implement your own wrapper around `free`, you can make it safer (at the cost of a little performance hit), specifically with something like the `myFreeXxx` calls below:

```c++
#include <stdio.h>
#include <stdlib.h>

void myFreeVoid (void **p) { free (*p); *p = NULL; }
void myFreeInt  (int  **p) { free (*p); *p = NULL; }
void myFreeChar (char **p) { free (*p); *p = NULL; }

int main (void) {
    char *x = malloc (1000);
    printf ("Before: %p\n", x);
    myFreeChar (&x);
    printf ("After:  %p\n", x);
    return 0;
}
```

The upshot of the code is that you can call `myFreeXxx` with a pointer to your pointer and it will both:

- free the memory; and
- set the pointer to `NULL`.

That latter bit means that, if you try to free the pointer again, it will do nothing (because freeing `NULL` is specifically covered by the standard).

It *won't* protect you from all situations, such as if you make a copy of the pointer elsewhere, free the original, then free the copy:

```c++
char *newptr = oldptr;
myFreeChar (&oldptr);     // frees and sets to NULL.
myFreeChar (&newptr);     // double-free because it wasn't set to NULL.
```

> NOTE: 这种情况更加隐晦，使用一个已经释放的内存。

------

If you're up to using C11, there's a better way than having to explicitly call a different function for each type now that C has compile time function overloading. You can use generic selections to call the correct function while still allowing for type safety:

```c++
#include <stdio.h>
#include <stdlib.h>

void myFreeVoid (void **p) { free (*p); *p = NULL; }
void myFreeInt  (int  **p) { free (*p); *p = NULL; }
void myFreeChar (char **p) { free (*p); *p = NULL; }
#define myFree(x) _Generic((x), \
    int** :  myFreeInt,  \
    char**:  myFreeChar, \
    default: myFreeVoid  )(x)

int main (void) {
    char *x = malloc (1000);
    printf ("Before: %p\n", x);
    myFree (&x);
    printf ("After:  %p\n", x);
    return 0;
}
```

With that, you simply call `myFree` and it will select the correct function based on the type.

### 分析二

owasp的[Doubly freeing memory](https://owasp.org/www-community/vulnerabilities/Doubly_freeing_memory)文章对此也分析得比较好。

原文的例子无法运行，下面是正确版本。

```c++
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE1    512
#define BUFSIZE2    ((BUFSIZE1/2) - 8)

int main(int argc, char **argv) {
  char *buf1R1;
  char *buf2R1;
  char *buf1R2;

  buf1R1 = (char *) malloc(BUFSIZE2);
  buf2R1 = (char *) malloc(BUFSIZE2);

  free(buf1R1);
  free(buf2R1);

  buf1R2 = (char *) malloc(BUFSIZE1);
  strncpy(buf1R2, argv[1], BUFSIZE1-1);

  free(buf2R1); // double free
  free(buf1R2); 
}

```

运行该程序

```
./a.out test
```

运行结果如下

```
Core was generated by `./a.out test'.
Program terminated with signal 6, Aborted.
#0  0x00007fe69fdf8207 in raise () from /lib64/libc.so.6
Missing separate debuginfos, use: debuginfo-install glibc-2.17-260.el7.x86_64 libgcc-4.8.5-39.el7.x86_64
(gdb) bt
#0  0x00007fe69fdf8207 in raise () from /lib64/libc.so.6
#1  0x00007fe69fdf98f8 in abort () from /lib64/libc.so.6
#2  0x00007fe69fe3ad27 in __libc_message () from /lib64/libc.so.6
#3  0x00007fe69fe43489 in _int_free () from /lib64/libc.so.6
#4  0x0000000000400629 in main ()

```

如果按照“How to avoid?”节中描述的方式进行修改，可以避免double free。

```C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE1    512
#define BUFSIZE2    ((BUFSIZE1/2) - 8)

int main(int argc, char **argv) {
  char *buf1R1;
  char *buf2R1;
  char *buf1R2;

  buf1R1 = (char *) malloc(BUFSIZE2);
  buf2R1 = (char *) malloc(BUFSIZE2);

  free(buf1R1);
  buf1R1 = NULL;
  free(buf2R1);
  buf2R1 = NULL;

  buf1R2 = (char *) malloc(BUFSIZE1);
  strncpy(buf1R2, argv[1], BUFSIZE1-1);

  free(buf2R1); // double free
  free(buf1R2); 
}

```



### 分析三

[Double Free](https://heap-exploitation.dhavalkapil.com/attacks/double_free.html)

## How to avoid?

因为`delete` 、`free`对空指针不执行如何操作，所以在`delete` 、`free`后，要将原指针置为空指针。