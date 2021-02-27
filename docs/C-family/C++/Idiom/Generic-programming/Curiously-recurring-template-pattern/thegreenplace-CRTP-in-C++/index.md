# thegreenplace [The Curiously Recurring Template Pattern in C++](https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c)

```c++
#include <iostream>
using namespace std;

template <typename Child>
struct Base
{
    void interface()
    {
        static_cast<Child*>(this)->implementation();
    }
};

struct Derived : Base<Derived>
{
    void implementation()
    {
        cerr << "Derived implementation\n";
    }
};

int main()
{
    Derived d;
    d.interface();  // Prints "Derived implementation"
}
```

The key to the technique is the strange template trickery that's being used: note that `Derived` inherits from `Base`. What gives? The idea is to "**inject**" the real type of the derived class into the base, *at compile time*, allowing the `static_cast` of `this` in the `interface` to produce the desired result. 

> NOTE: "**inject**"这个词是非常形象的。

The following example is much longer - although it is also a simplification. It presents a generic base class for visiting binary trees in various orders. This base class can be inherited to specify special handling of some types of nodes. Here is the tree node definition and the base class:

```c++
#include <iostream>
#include <assert.h> // header of assert
using namespace std;
struct TreeNode
{
    enum Kind {RED, BLUE};

    TreeNode(Kind kind_, TreeNode* left_ = NULL, TreeNode* right_ = NULL)
        : kind(kind_), left(left_), right(right_)
    {}

    Kind kind;
    TreeNode *left, *right;
};

template <typename Derived>
class GenericVisitor
{
public:
    // 先序遍历
    void visit_preorder(TreeNode* node)
    {
        if (node) {
            dispatch_node(node);
            visit_preorder(node->left);
            visit_preorder(node->right);
        }
    }
	// 中序遍历
    void visit_inorder(TreeNode* node)
    {
        if (node) {
            visit_inorder(node->left);
            dispatch_node(node);
            visit_inorder(node->right);
        }
    }
	// 后序遍历
    void visit_postorder(TreeNode* node)
    {
        if (node) {
            visit_postorder(node->left);
            visit_postorder(node->right);
            dispatch_node(node);
        }
    }

    void handle_RED(TreeNode* node)
    {
        cerr << "Generic handle RED\n";
    }

    void handle_BLUE(TreeNode* node)
    {
        cerr << "Generic handle BLUE\n";
    }

private:
    // Convenience method for CRTP
    //
    Derived& derived()
    {
        return *static_cast<Derived*>(this);
    }

    void dispatch_node(TreeNode* node)
    {
        switch (node->kind) {
            case TreeNode::RED:
                derived().handle_RED(node);
                break;
            case TreeNode::BLUE:
                derived().handle_BLUE(node);
                break;
            default:
                assert(0);
        }
    }
};
```

And a simple derived class:

```c++
class SpecialVisitor : public GenericVisitor<SpecialVisitor>
{
public:
    // 会override基类的方法
    void handle_RED(TreeNode* node)
    {
        cerr << "RED is special\n";
    }
};
```



```c++
int main()
{
	TreeNode N10(TreeNode::RED);
	TreeNode N11(TreeNode::RED);
	TreeNode N00(TreeNode::BLUE, &N10, &N11);
	SpecialVisitor Visitor;
	Visitor.visit_preorder(&N00);
	return 0;
}
```

> NOTE: 将上述三段代码进行合并，然后编译`g++ test.cpp`，运行程序，得到如下输出
>
> ```
> Generic handle BLUE
> RED is special
> RED is special
> ```

Now you can easily implement special handling of various kinds of nodes in subclasses, and use visiting services provided by the base class.

> NOTE: 使用[Inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)) 的目的是为了[Reusability](https://en.wikipedia.org/wiki/Reusability)。

To reiterate - this is a simplified example, as there are only two kinds of nodes, but in reality there can be many more. Such code would be quite useful inside compilers, where the source is usually parsed into a tree with many different kinds of nodes. Multiple passes in the compiler then process the trees by implementing their own visitors. As a matter of fact, the [Clang compiler frontend](http://clang.llvm.org/) has such a class, named `RecursiveASTVisitor`, which implements a much more complete version of the visitor displayed above.

Without CRTP, there's no way to implement such functionality except resorting to dynamic polymorphism and virtual functions [[2\]](https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c#id4).

Another interesting example is the following:

```c++
template <typename Derived>
struct Comparisons
{
};


template <typename Derived>
bool operator==(const Comparisons<Derived>& o1, const Comparisons<Derived>& o2)
{
    const Derived& d1 = static_cast<const Derived&>(o1);
    const Derived& d2 = static_cast<const Derived&>(o2);

    return !(d1 < d2) && !(d2 < d1);
}


template <typename Derived>
bool operator!=(const Comparisons<Derived>& o1, const Comparisons<Derived>& o2)
{
    return !(o1 == o2);
}
```

This is a generic base class with some external comparison functions that act on it. What this makes possible is to create a derived class that only defines the `<` operator, making other comparison operators (`==` and `!=` here, but others are trivial to add) possible. Here's a sample derived class:

```c++
class Person : public Comparisons<Person>
{
public:
    Person(string name_, unsigned age_)
        : name(name_), age(age_)
    {}

    friend bool operator<(const Person& p1, const Person& p2);
private:
    string name;
    unsigned age;
};


bool operator<(const Person& p1, const Person& p2)
{
    return p1.age < p2.age;
}
```

Again, this is using CRTP to implement something that could only be possible with virtual functions had we wanted dynamic polymorphism. Sometimes a class like `Comparisons` above is called a *mixin class*:

> In object-oriented programming languages, a mixin is a class that provides a certain functionality to be inherited or just reused by a subclass, while not meant for instantiation (the generation of objects of that class). Inheriting from a mixin is not a form of specialization but is rather a means of collecting functionality. A class may inherit most or all of its functionality from one or more mixins through multiple inheritance.
>
> [[Wikipedia quote](http://en.wikipedia.org/wiki/Mixin)]

So how often is CRTP used in "real life"? I don't have any actual usage statistics, but it appears that this is a useful tool in a C++ programmer's toolbox. The `RecursiveASTVisitor` class from Clang I mentioned above is a very real use case. Clang's parent project LLVM uses CRTP in at least another place (the `HeuristicBase` class in the code generator module).

Boost also uses CRTP for its [Iterator Facade](http://www.boost.org/doc/libs/1_46_0/libs/iterator/doc/iterator_facade.html):

> `iterator_facade` is a base class template that implements the interface of standard iterators in terms of a few core functions and associated types, to be supplied by a derived iterator class.

And finally, Microsoft's Active Template Library (ATL) uses CRTP comprehensively. See, for example, the [CWindowImpl template](http://msdn.microsoft.com/en-us/library/h4616bh2(v=vs.80).aspx).



