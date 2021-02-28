# Forwarding 

“forwarding”即“转发”；


## wikipedia [Forwarding (object-oriented programming)](https://en.wikipedia.org/wiki/Forwarding_(object-oriented_programming))

In [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), **forwarding** means that using a member of an [object](https://en.wikipedia.org/wiki/Object_(computer_science)) (either a [property](https://en.wikipedia.org/wiki/Property_(programming)) or a [method](https://en.wikipedia.org/wiki/Method_(computer_programming))) results in actually using the corresponding member of a different object: the use is *forwarded* to another object. Forwarding is used in a number of [design patterns](https://en.wikipedia.org/wiki/Software_design_pattern), where some members are forwarded to another object, while others are handled by the directly used object. The **forwarding object** is frequently called a **wrapper object**, and explicit forwarding members are called [wrapper functions](https://en.wikipedia.org/wiki/Wrapper_function).

> NOTE：经常在阅读过程中碰到这个词语，直到今天阅读了这篇文章才完全理解它的意思；



### Delegation

**Forwarding** is often confused with [delegation](https://en.wikipedia.org/wiki/Delegation_(object-oriented_programming)); formally, they are complementary（互补的） concepts. In both cases, there are two objects, and the first (sending, wrapper) object uses the second (receiving, wrappee) object, for example to call a method. They differ in what `self` refers to on the **receiving object** (formally, in the [evaluation environment](https://en.wikipedia.org/wiki/Evaluation_environment) of the method on the **receiving object**): in **delegation** it refers to the **sending object**, while in **forwarding** it refers to the **receiving object**. Note that `self` is often used implicitly as part of [dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch) (method resolution: which function a method name refers to method name指的是哪个function).

> The difference between **forwarding** and **delegation** is the binding of the **self** parameter in the **wrappee** when called through the wrapper. With delegation, the self parameter is bound to the wrapper, with forwarding it is bound to the wrappee. ... Forwarding is a form of automatic message resending; delegation is a form of inheritance with binding of the parent (superclass) at run time, rather than at compile/link time as with 'normal' inheritance.[[1\]](https://en.wikipedia.org/wiki/Forwarding_(object-oriented_programming)#cite_note-buechiweck-1)

For example, given the following code:

```c++
// Sender
void n() {
  print("n1");
}

// Receiver
void m() {
  print("m2, "); n();
}

void n() {
  print("n2");
}
```

under **delegation** this will output `m2, n1` because `n()` is evaluated in the context of the original (sending) object, while under forwarding this will output `m2, n2`because `n()` is evaluated in the context of the receiving object.[[1\]](https://en.wikipedia.org/wiki/Forwarding_(object-oriented_programming)#cite_note-buechiweck-1)

In casual use, **forwarding** is often referred to as "delegation", or considered a form of delegation, but in careful usage they are clearly distinguished by what `self` refers to. 

While delegation is analogous(类似于) to [inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)), allowing behavioral reuse (and concretely [code reuse](https://en.wikipedia.org/wiki/Code_reuse)) *without* changing **evaluation context**, forwarding is analogous to [composition](https://en.wikipedia.org/wiki/Object_composition), as execution depends only on the receiving (member) object, not the (original) sending object.

In both cases, reuse is dynamic, meaning determined at run time (based on the *object* to which use is delegated or forwarded), rather than static, meaning determined at compile/link time (based on the *class* which is inherited from). Like **inheritance**, **delegation** allows the sending object to modify the original behavior, but is susceptible to problems analogous to the [fragile base class](https://en.wikipedia.org/wiki/Fragile_base_class); while forwarding provides stronger encapsulation and avoids these problems; see [composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance).[[1\]](https://en.wikipedia.org/wiki/Forwarding_(object-oriented_programming)#cite_note-buechiweck-1)



### Examples

A simple example of **explicit forwarding** in Java: an instance of `B` forwards calls to the `foo` method of its `a` field:

```Java
class B {
  A a;
  T foo() { return a.foo(); }
}
```

Note that when executing `a.foo()`, the `this` object is `a` (a subtype of `A`), not the original object (an instance of `B`). Further, `a` need not be an instance of `A`: it may be an instance of a subtype. Indeed, `A` need not even be a class: it may be an interface/[protocol](https://en.wikipedia.org/wiki/Protocol_(object-oriented_programming)).

Contrast with inheritance, in which `foo` is defined in a superclass `A` (which must be a class, not an interface), and when called on an instance of a subclass `B`, it uses the code defined in `A`, but the `this` object is still an instance of `B`:

```Java
class A {
  T foo() { /* ... */ };
}

class B extends A {
}
```

In this Python example, class `B` forwards the `foo` method and the `x` property to the object in its `a` field: using these on `b` (an instance of `B`) is the same as using them on `b.a` (the instance of `A` to which these are forwarded).

```Java
class A:
    def __init__(self, x):
        self.x = x

    def foo(self):
        print(self.x)

class B:
    def __init__(self, a):
        self.a = a

    def foo(self):
        self.a.foo()

    @property
    def x(self):
        return self.a.x

    @x.setter
    def x(self, x):
        self.a.x = x

    @x.deleter
    def x(self):
        del self.a.x

a = A(42)
b = B(a)
b.foo()  # Prints '42'.
b.x  # Has value '42'
b.x = 17   # b.a.x now has value 17
del b.x  # Deletes b.a.x.
```



#### Complex

The more complex case is a [Decorator Pattern](https://en.wikipedia.org/wiki/Decorator_pattern#Java) that by using [interfaces](https://en.wikipedia.org/wiki/Interface_(Java)), **forwarding** can be made more flexible and [typesafe](https://en.wikipedia.org/wiki/Type_safety). "Flexibility" here means that `C` need not refer to `A` or `B` in any way, as the switching of forwarding is abstracted from `C`. In this example, class `C` can forward to any class that implements an interface `I`. Class `C` has a method to switch to another forwarder. Including the `implements` clauses improves [type safety](https://en.wikipedia.org/wiki/Type_safety), because each class must implement the methods in the interface. The main tradeoff is more code.

```Java
interface I {
	void f();
	void g();
}
 
class A implements I {
	public void f() { System.out.println("A: doing f()"); }
	public void g() { System.out.println("A: doing g()"); }
}
 
class B implements I {
	public void f() { System.out.println("B: doing f()"); }
	public void g() { System.out.println("B: doing g()"); }
}
 
// changing the implementing object in run-time (normally done in compile time)
class C implements I {
	I i = null;
	// forwarding
	public C(I i){ setI(i); }
	public void f() { i.f(); }
	public void g() { i.g(); }
 
	// normal attributes
	public void setI(I i) { this.i = i; }
}
 
public class Main {
	public static void main(String[] arguments) {
		C c = new C(new A());
		c.f();	// output: A: doing f()
		c.g();	// output: A: doing g()
		c.setI(new B());
		c.f();	// output: B: doing f()
		c.g();	// output: B: doing g()
	}
}
```

### Applications

Forwarding is used in many design patterns.[[2\]](https://en.wikipedia.org/wiki/Forwarding_(object-oriented_programming)#cite_note-2) Forwarding is used directly in several patterns:

- [Chain-of-responsibility pattern](https://en.wikipedia.org/wiki/Chain-of-responsibility_pattern)
- [Decorator pattern](https://en.wikipedia.org/wiki/Decorator_pattern): decorator object adds its own members, forwarding others to the decorated object.
- [Proxy pattern](https://en.wikipedia.org/wiki/Proxy_pattern): proxy object forwards member use to real object.

Forwarding may be used in other patterns, but often use is modified; for example, a method call on one object results in several different methods being called on another:

- [Adapter pattern](https://en.wikipedia.org/wiki/Adapter_pattern)
- [Bridge pattern](https://en.wikipedia.org/wiki/Bridge_pattern)
- [Facade pattern](https://en.wikipedia.org/wiki/Facade_pattern)



## stackoverflow [In OOP, what is forwarding and how is it different from delegation?](https://stackoverflow.com/questions/7816011/in-oop-what-is-forwarding-and-how-is-it-different-from-delegation)

这个回答是非常好的：https://stackoverflow.com/a/28539643

总结：It seems that the metaphor in this answer is very subtle.and it fits very well with the explanation in Wikipedia.



