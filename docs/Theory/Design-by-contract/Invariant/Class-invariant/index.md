# Class invariant 

1、Class invariant本质上是 **predefined conditions**，参见 wikipedia [Class invariant](https://en.wikipedia.org/wiki/Class_invariant)

2、在 [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) 中，有很多关于class invariant的讨论

3、关于Class invariant，最最典型的案例就是C++ `gsl::not_null`，参见相关章节

4、作为class的设计者，我们应该清楚地说明清楚class invariant

5、在testing中，需要对class invariant进行检验， wikipedia [Class invariant](https://en.wikipedia.org/wiki/Class_invariant) :

> Defining class invariants can help programmers and testers to catch more bugs during [software testing](https://en.wikipedia.org/wiki/Software_testing).

## wikipedia [Class invariant](https://en.wikipedia.org/wiki/Class_invariant)



In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), specifically [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), a **class invariant** (or **type invariant**) is an [invariant](https://en.wikipedia.org/wiki/Invariant_(computer_science)) used to constrain [objects](https://en.wikipedia.org/wiki/Object_(computer_science)) of a [class](https://en.wikipedia.org/wiki/Class_(computer_science)). [Methods](https://en.wikipedia.org/wiki/Method_(computer_science)) of the class should preserve the invariant. The class invariant **constrains** the state stored in the object.

Class invariants are established during construction and constantly maintained between calls to public methods. Code within functions may break invariants as long as the invariants are restored before a public function ends.

An **object invariant**, or representation invariant, is a [computer programming](https://en.wikipedia.org/wiki/Computer_programming) construct consisting of a set of invariant properties that remain uncompromised(不妥协的) regardless of the state of the [object](https://en.wikipedia.org/wiki/Object_(computer_science)). This ensures that the object will always meet **predefined conditions**, and that [methods](https://en.wikipedia.org/wiki/Method_(computer_science)) may, therefore, always reference the object without the risk of making inaccurate presumptions. Defining class invariants can help programmers and testers to catch more bugs during [software testing](https://en.wikipedia.org/wiki/Software_testing).



### Class invariants and inheritance

The useful effect of class invariants in object-oriented software is enhanced in the presence of inheritance. Class invariants are inherited, that is, "the invariants of all the parents of a class apply to the class itself."[[1\]](https://en.wikipedia.org/wiki/Class_invariant#cite_note-1)

Inheritance can allow descendant classes to alter implementation data of parent classes, so it would be possible for a descendant class to change the state of instances in a way that made them invalid from the viewpoint of the parent class. The concern for this type of misbehaving descendant is one reason object-oriented software designers give for favoring [composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance) (i.e., inheritance breaks encapsulation).[[2\]](https://en.wikipedia.org/wiki/Class_invariant#cite_note-2)

However, because class invariants are inherited, the class invariant for any particular class consists of any **invariant assertions** coded immediately on that class, logically "and-ed" with all the invariant clauses inherited from the class's parents. This means that even though descendant classes may have access to the implementation data of their parents, the class invariant can prevent them from manipulating those data in any way that produces an invalid instance at runtime.



### Example

#### Inheritance

An example in [C++11](https://en.wikipedia.org/wiki/C%2B%2B11) follows:

```c++
class GameObject {
    public:
        virtual ~GameObject() {}
        virtual void update() {}
        virtual void draw() {}
        virtual void collide(GameObject objects[]) {}
};

class Visible : public GameObject {
    public:
        void draw() override { /* draw model at position of this object */ };
    private:
        Model* model;
};

class Solid : public GameObject {
    public:
        void collide(GameObject objects[]) override { /* check and react to collisions with objects */ };
};

class Movable : public GameObject {
    public:
        void update() override { /* update position */ };
};
```

Then, we have concrete classes:

- class `Player` - which is `Solid`, `Movable` and `Visible`
- class `Cloud` - which is `Movable` and `Visible`, but not `Solid`
- class `Building` - which is `Solid` and `Visible`, but not `Movable`
- class `Trap` - which is `Solid`, but neither `Visible` nor `Movable`

Note that multiple inheritance is dangerous if not implemented carefully, as it can lead to the [diamond problem](https://en.wikipedia.org/wiki/Diamond_problem). One solution to avoid this is to create classes such as `VisibleAndSolid`, `VisibleAndMovable`, `VisibleAndSolidAndMovable`, etc. for every needed combination, though this leads to a large amount of repetitive code. Keep in mind that C++ solves the diamond problem of multiple inheritance by allowing [virtual inheritance](https://en.wikipedia.org/wiki/Virtual_inheritance).



## stackoverflow [What is a class invariant in java?](https://stackoverflow.com/questions/8902331/what-is-a-class-invariant-in-java)

I googled the topic, but besides [Wikipedia](http://en.wikipedia.org/wiki/Class_invariant) I didn't find any further useful documentation or articles.

Can anybody explain to me in simple words what it means or refer me to some nice and easy to understand documentation?	

***COMMENTS***:

A more simple explanation - <[stackoverflow.com/questions/112064/what-is-an-invariant?rq=1](http://stackoverflow.com/questions/112064/what-is-an-invariant?rq=1)>

### [A](https://stackoverflow.com/a/8902410)

It doesn't mean anything in particular in reference to java.

A **class invariant** is simply a **property** that holds for all instances of a class, always, no matter what other code does.

For example,

```java
class X {
  final Y y = new Y();
}
```

X has the **class invariant** that there is a `y` property and it is never `null` and it has a value of type `Y`.

```java
class Counter {
  private int x;

  public int count() { return x++; }
}
```

fails to maintain two important **invariants**

1. That `count` never returns a negative value because of possible underflow.
2. That calls to `count` are strictly monotonically increasing.

The modified class preserves those two invariants.

```java
class Counter {
  private int x;

  public synchronized int count() {
    if (x == Integer.MAX_VALUE) { throw new IllegalStateException(); }
    return x++;
  }
}
```

but fails to preserve the **invariant** that calls to `count` always succeed normally (absent TCB-violations†) because `count` might throw an exception or it might block if a deadlocked thread owns the counter's monitor.

Each language with classes make it easy to maintain some class invariants but not others. Java is no exception:

1. Java classes consistently(一贯地) have or do not have properties and methods, so interface invariants are easy to maintain.
2. Java classes can protect their `private` fields, so invariants that rely on private data are easy to maintain.
3. Java classes can be final, so invariants that rely on there being no code that violates an invariant by crafting a malicious subclass can be maintained.
4. Java allows `null` values to sneak in in many ways, so it is tough to maintain "has a real value" invariants.
5. Java has threads which means that classes that do not synchronize have trouble maintaining invariants that rely on sequential operations in a thread happening together.
6. Java has exceptions which makes it easy to maintain invariants like "returns a result with property p or returns no result" but harder to maintain invariants like "always returns a result".

------

† - An *externality* or *TCB violation* is an event which a systems designer optimistically assumes will not happen.

Typically we just trust that the basic hardware works as advertised when talking about properties of high-level languages built on them, and our arguments that invariants hold don't take into account the possibility of:

- A programmer using debug hooks to alter local variables as a program runs in ways that code cannot.
- Your peers don't use reflection with `setAccessible` to modify `private` lookup tables.
- Loki altering physics causing your processor to incorrectly compare two numbers.

For some systems our TCB might include only parts of the system, so we might not assume that

- An administrator or privileged daemon won't kill our JVM process,

but we might assume that

- We can checkpoint to a reliable transactional file-system.

The higher-level a system, the larger its TCB typically is, but the more unreliable things you can get out of your TCB, the more likely your invariants are to hold, and the more reliable your system will be in the long run.



***COMMENTS***

[ruakh](https://stackoverflow.com/users/978917/ruakh):Is "that `count` never returns the same value twice" really considered a class invariant? 

 [Mike Samuel](https://stackoverflow.com/users/20394/mike-samuel) :  that's a good question. I'm not quite sure. Things like hashCode stability (for each instance i, i.hashCode() does not change) are often called **class invariants** which requires reasoning about values returned previously, so it seems reasonable to say that "for each instance i, i.count() not in (previous results of i.count())" is a class invariant. 

### [A](https://stackoverflow.com/a/19583347)

Invariant means something that should stick to its conditions no matter whatever changes or whoever uses/transforms it. That is to say, a property of a class always fulfills or satisfies some condition even after going through transformations by using public methods. So, the client or user of this class is ensured about the class and its property.

For example,

1. condition on function argument is that, it should always be > 0 (greater than zero) or should not be null.
2. minimum_account_balance property of an account class states, it cannot go below 100. So all public functions should respect this condition and ensure **class invariant**.
3. rule based dependency between variables, that is, value of one variable depends on another, so if one changes, using some fix-rule, other must also change. This relationship between 2 variables must be preserved. If it does not, then invariant is violated.





## stackoverflow [What is an invariant?](https://stackoverflow.com/questions/112064/what-is-an-invariant)

The word seems to get used in a number of contexts. The best I can figure is that they mean a variable that can't change. Isn't that what constants/finals (darn you Java!) are for?

### [A](https://stackoverflow.com/a/112088)

An invariant is more "conceptual" than a variable. In general, it's a property of the **program state** that is always true. A function or method that ensures that the invariant holds is said to maintain the invariant.

For instance, a binary search tree might have the invariant that for every node, the key of the node's left child is less than the node's own key. A correctly written insertion function for this tree will maintain that invariant.

As you can tell, that's not the sort of thing you can store in a variable: it's more a statement *about* the program. By figuring out what sort of invariants your program should maintain, then reviewing your code to make sure that it actually maintains those invariants, you can avoid logical errors in your code.





