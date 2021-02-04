# Happens before 

## stackoverflow [Confusion about happens before relationship in concurrency](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency)

Below is an example given in **Concurrency in Action** , and the author says the `assert` may fire, but I don't understand why.

```cpp
#include <atomic>
#include <thread>
#include <assert.h>
std::atomic<bool> x,y;
std::atomic<int> z;
void write_x_then_y()
{
  x.store(true,std::memory_order_relaxed);
  y.store(true,std::memory_order_relaxed);
}
void read_y_then_x()
{
  while(!y.load(std::memory_order_relaxed));
  if(x.load(std::memory_order_relaxed))
  ++z;
}
int main()
{
  x=false;
  y=false;
  z=0;
  std::thread a(write_x_then_y);
  std::thread b(read_y_then_x);
  a.join();
  b.join();
  assert(z.load()!=0);
}
```

> NOTE:  `read_y_then_x()` 中，`while(!y.load(std::memory_order_relaxed));` 表明它需要等待 `y`，即它依赖`y`

As far as I know, in each single thread, `sequenced before` also means `happens before`. So in thread `a` the store to `x` happens before `y`, which means `x` should be modified before `y` and the result `x.store` should be visible before `y` is modified.

But in this example the author says that the store between `x` and `y` could be reordered, why? Does that violate the rule of `sequenced before` and `happens before`?

### comments

This might explain things about **relaxed ordering** [en.cppreference.com/w/cpp/atomic/memory_order#Relaxed_ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Relaxed_ordering) – [Sami Kuhmonen](https://stackoverflow.com/users/1806780/sami-kuhmonen) [Jul 29 '18 at 13:54](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90127305_51580966)

@SamiKuhmonen **sequenced before** relationship does not prevent reordering? Then what's the usage of sequenced before? – [scottxiao](https://stackoverflow.com/users/9133378/scottxiao) [Jul 29 '18 at 14:13](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90127596_51580966)

`x.store` happens-before `y.store` and `y.load` happens-before `x.load` - but `x.store` doesn't happen-before `x.load`. Happens-before relationship is not necessarily transitive in the presence of relaxed operations; that's kind of their whole point. – [Igor Tandetnik](https://stackoverflow.com/users/1670129/igor-tandetnik) [Jul 29 '18 at 14:20](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90127697_51580966)

> NOTE: 上述happens-before relation是从源程序中抽象出来的

@IgorTandetnik but `y.store` happens before `while(!y.load(std::memory_order_relaxed))` , so `x.store` hapeens before `x.load`? – [scottxiao](https://stackoverflow.com/users/9133378/scottxiao) [Jul 29 '18 at 14:21](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90127709_51580966)

> NOTE: 并不能保证 : `y.store` happens before `while(!y.load(std::memory_order_relaxed))` 

The conclusion doesn't follow from the premise(前提). Yes, `y.store` happens-before `y.load`. No, `x.store` does not happen-before `x.load`. You have a sequentially-consistent model of execution in your head - but relaxed operations violate that model; that's why they are called "relaxed", and that's why they are difficult to reason about. – [Igor Tandetnik](https://stackoverflow.com/users/1670129/igor-tandetnik) [Jul 29 '18 at 14:23](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90127735_51580966) 

@IgorTandetnik since `y.store` happens before `y.load==true`, and happens before relationship is trasitive, why doesn't `x.store` happens before `x.load`? – [scottxiao](https://stackoverflow.com/users/9133378/scottxiao) [Jul 29 '18 at 14:38](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90127998_51580966)

Again, happens-before relationship is **not** in general transitive. In particular, it is not transitive in the presence of relaxed operations. Would it be too much to ask for you to read a comment in full before responding? – [Igor Tandetnik](https://stackoverflow.com/users/1670129/igor-tandetnik) [Jul 29 '18 at 14:58](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90128326_51580966) 

The **Concurrency in Action** says "It’s also a transitive relation: if A **inter-thread happens-before** B and B inter-thread happens-before C, then A inter-thread happens-before C.". – [scottxiao](https://stackoverflow.com/users/9133378/scottxiao) [Jul 29 '18 at 15:02](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90128383_51580966) 

*happens-before* and *inter-thread happens-before* are two different relations. Here, `y.load` does not inter-thread happens-before `x.load` (since both are on the same thread). – [Igor Tandetnik](https://stackoverflow.com/users/1670129/igor-tandetnik) [Jul 29 '18 at 15:14](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90128578_51580966) 

@IgorTandetnik What's the differences between happens before and inter-thread happens before? – [scottxiao](https://stackoverflow.com/users/9133378/scottxiao) [Jul 29 '18 at 15:41](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90129053_51580966)

[eel.is/c++draft/intro.multithread#intro.races-10](http://eel.is/c++draft/intro.multithread#intro.races-10) – [Igor Tandetnik](https://stackoverflow.com/users/1670129/igor-tandetnik) [Jul 29 '18 at 15:45](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90129122_51580966)

@IgorTandetnik Your linkage says they are not different relations, inter-thread happens before belongs to happens before. – [scottxiao](https://stackoverflow.com/users/9133378/scottxiao) [Jul 29 '18 at 16:14](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90129628_51580966)

They are different relations, where one is a subset of the other. The subset is transitive; the superset is not. In particular, if `A` inter-thread happens-before `B` and `B` is sequenced-before `C`, this doesn't necessarily mean that `A` happens-before `C` (even though `A` happens-before `B` and `B` happens-before `C`). – [Igor Tandetnik](https://stackoverflow.com/users/1670129/igor-tandetnik) [Jul 29 '18 at 16:23](https://stackoverflow.com/questions/51580966/confusion-about-happens-before-relationship-in-concurrency#comment90129770_51580966)

### [A](https://stackoverflow.com/a/65337588)

From thread `a` perspective, it looks as if `x.store` happens before `y.store`. However, from thread b perspective, it can look as if they are reordered. https://koheiotsuka701.medium.com/memory-model-basic-d8b5f8fddd5f