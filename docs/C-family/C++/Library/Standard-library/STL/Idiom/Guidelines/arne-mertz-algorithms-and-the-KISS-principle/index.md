# arne-mertz [algorithms and the KISS principle](https://arne-mertz.de/2019/05/algorithms-and-the-kiss-principle/)

> NOTE: 
>
> 1、这篇文章的主题并不明确

You’ve probably heard about the rule to prefer standard algorithms over raw for loops. The major reason is that those algorithms say what is happening in the name and encapsulate the loop logic. But they’re not always the best choice.

> NOTE: 
>
> 1、intentional programming
>
> 2、它们不总是best choice

## Use standard algorithms

Especially the more complex algorithms can be quite messy when we implement them by hand. So, besides having the name of the algorithm in the code, it’s a good thing to have the algorithm part separated from the rest of the logic. It makes the code less complicated and thus adheres to the [KISS principle](https://en.wikipedia.org/wiki/KISS_principle). There’s a [famous talk from Sean Parent](https://channel9.msdn.com/Events/GoingNative/2013/Cpp-Seasoning) about the topic, I suggest you watch it.

> NOTE: 翻译如下:
>
> "特别是更复杂的算法，当我们手工实现它们时，可能会非常混乱。所以，除了在代码中有算法的名字之外，把算法部分和其他逻辑部分分开也是一件好事。它使代码不那么复杂，因此遵循KISS原则。Sean Parent有一个关于这个话题的著名演讲，我建议你看。"

## Tranform with a condition



```C++
  std::map<std::string, unsigned> salariesByName;

  for (auto const& employee : employees) {
    if (!employee.isManager()) {
      salariesByName[employee.uniqueName()] = employee.salary();
    }
  }
```

The loop gets slightly more involved but is still readable. We might not be convinced that using an algorithm here is necessary to make it more readable. But let’s see how it looks like if we do. Usually, algorithms with a condition, or, in standardese, a predicate, have the suffix `_if` in their name. There’s `std::copy_if` to copy only things that satisfy a condition, and `std::find_if` and `std::remove_if` work on elements that match a predicate instead of a value. So, the algorithm we’re looking for is `transform_if`. But that doesn’t exist in the standard library. Bummer. Luckily, it’s not hard to implement when we have a peek at the implementations of `std::transform` and `std::copy_if`. So now we start our own algorithms library. The whole code now looks like this:

```C++
  template <typename InIter, typename OutIter,
            typename UnaryOp, typename Pred>
  OutIter transform_if(InIter first, InIter last,
            OutIter result, UnaryOp unaryOp, Pred pred) {
    for(; first != last; ++first) {
      if(pred(*first)) {
        *result = unaryOp(*first);
        ++result;
      }
    }
    return result;
  }

  //...

  std::map<std::string, unsigned> salariesByName;

  transform_if(
    std::begin(employees), 
    std::end(employees), 
    std::inserter(salariesByName, std::end(salariesByName)),
    [](auto const& employee) {
      return std::make_pair(
        employee.uniqueName(),
        employee.salary()
      );
    },
    [](auto const& employee) {
      return !employee.isManager();
    }
  );
```

