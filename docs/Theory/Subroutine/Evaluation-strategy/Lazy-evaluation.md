# [Lazy evaluation](https://en.wikipedia.org/wiki/Lazy_evaluation)

In [programming language theory](https://en.wikipedia.org/wiki/Programming_language_theory), **lazy evaluation**, or **call-by-need**[[1\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-1) is an [evaluation strategy](https://en.wikipedia.org/wiki/Evaluation_strategy) which delays the evaluation of an [expression](https://en.wikipedia.org/wiki/Expression_(computer_science)) until its value is needed ([non-strict evaluation](https://en.wikipedia.org/wiki/Non-strict_evaluation)) and which also avoids repeated evaluations ([sharing](https://en.wikipedia.org/wiki/Sharing_(computer_science))).[[2\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-WattFindlay2004-2)[[3\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-3) The sharing can reduce the running time of certain functions by an exponential factor over other non-strict evaluation strategies, such as [call-by-name](https://en.wikipedia.org/wiki/Call-by-name).[*citation needed*]

The benefits of lazy evaluation include:

- The ability to define [control flow](https://en.wikipedia.org/wiki/Control_flow) (structures) as abstractions instead of primitives.
- The ability to define [potentially infinite](https://en.wikipedia.org/wiki/Actual_infinity) [data structures](https://en.wikipedia.org/wiki/Data_structure). This allows for more straightforward implementation of some algorithms.
- Performance increases by avoiding needless calculations, and error conditions in evaluating compound expressions.

Lazy evaluation is often combined with [memoization](https://en.wikipedia.org/wiki/Memoization), as described in [Jon Bentley](https://en.wikipedia.org/wiki/Jon_Bentley_(computer_scientist))'s *Writing Efficient Programs*.[[4\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-4) After a function's value is computed for that parameter or set of parameters, the result is stored in a lookup table that is indexed by the values of those parameters; the next time the function is called, the table is consulted to determine whether the result for that combination of parameter values is already available. If so, the stored result is simply returned. If not, the function is evaluated and another entry is added to the lookup table for reuse.

**Lazy evaluation** can lead to reduction in memory footprint（内存占用）, since values are created when needed.[[5\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-Smith2009-5) However, lazy evaluation is difficult to combine with imperative features such as [exception handling](https://en.wikipedia.org/wiki/Exception_handling) and [input/output](https://en.wikipedia.org/wiki/Input/output), because the order of operations becomes indeterminate. Lazy evaluation can introduce [memory leaks](https://en.wikipedia.org/wiki/Memory_leak).[[6\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-FOOTNOTELaunchbury1993-6)[[7\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-7)

The opposite of lazy evaluation is [eager evaluation](https://en.wikipedia.org/wiki/Eager_evaluation), sometimes known as strict evaluation. Eager evaluation is the evaluation strategy employed in most [programming languages](https://en.wikipedia.org/wiki/Programming_language).

## Applications

Delayed evaluation is used particularly in [functional programming](https://en.wikipedia.org/wiki/Functional_programming) languages. When using delayed evaluation, an expression is not evaluated as soon as it gets bound to a variable, but when the evaluator is forced to produce the expression's value. That is, a statement such as `x = expression;` (i.e. the assignment of the result of an expression to a variable) clearly calls for the expression to be evaluated and the result placed in `x`, but what actually is in `x` is irrelevant until there is a need for its value via a reference to `x` in some later expression whose evaluation could itself be deferred, though eventually the rapidly growing tree of dependencies would be pruned to produce some symbol rather than another for the outside world to see.[[12\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-Wadler2006-12)

总结：上面这段话中所提及的内容在此之前我是非常混淆的。

Delayed evaluation has the advantage of being able to create calculable infinite lists without infinite loops or size matters interfering（干扰） in computation. For example, one could create a function that creates an infinite list (often called a *stream*) of [Fibonacci numbers](https://en.wikipedia.org/wiki/Fibonacci_number). The calculation of the *n*-th Fibonacci number would be merely the extraction of that element from the infinite list, forcing the evaluation of only the first n members of the list.[[13\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-M%C3%A9tayer2002-13)[[14\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-MachineryLanguages2002-14)

总结：calculable infinite list的特性是非常吸引人的。

For example, in the [Haskell](https://en.wikipedia.org/wiki/Haskell_(programming_language)) programming language, the list of all Fibonacci numbers can be written as:[[14\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-MachineryLanguages2002-14)

```
 fibs = 0 : 1 : zipWith (+) fibs (tail fibs)
```

In Haskell syntax, "`:`" prepends an element to a list, `tail` returns a list without its first element, and `zipWith` uses a specified function (in this case addition) to combine corresponding elements of two lists to produce a third.[[13\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-M%C3%A9tayer2002-13)

Provided the programmer is careful, only the values that are required to produce a particular result are evaluated. However, certain calculations may result in the program attempting to evaluate an infinite number of elements; for example, requesting the length of the list or trying to sum the elements of the list with a [fold operation](https://en.wikipedia.org/wiki/Fold_(higher-order_function)) would result in the program either failing to terminate or running [out of memory](https://en.wikipedia.org/wiki/Out_of_memory).

总结：显然在上面的这个例子中就会存在上面这段中描述的问题。

### Control structures

In almost all common "eager" languages, *if* statements evaluate in a lazy fashion.

### Working with infinite data structures

Many languages offer the notion of *infinite data-structures*. These allow definitions of data to be given in terms of infinite ranges, or unending recursion, but the actual values are only computed when needed. Take for example this trivial program in Haskell:

```haskell
numberFromInfiniteList :: Int -> Int
numberFromInfiniteList n =  infinity !! n - 1
    where infinity = [1..]

main = print $ numberFromInfiniteList 4
```



### Other uses

#### 在 [windowing systems](https://en.wikipedia.org/wiki/Windowing_system)中的应用

In computer [windowing systems](https://en.wikipedia.org/wiki/Windowing_system), the painting of information to the screen is driven by *expose events* which drive the display code at the last possible moment. By doing this, windowing systems avoid computing unnecessary display content updates.[[15\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-Lampson-15)

#### copy-on-write

Another example of laziness in modern computer systems is [copy-on-write](https://en.wikipedia.org/wiki/Copy-on-write) page allocation or [demand paging](https://en.wikipedia.org/wiki/Demand_paging), where memory is allocated only when a value stored in that memory is changed.[[15\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-Lampson-15)

#### high performance

Laziness can be useful for high performance scenarios. An example is the Unix [mmap](https://en.wikipedia.org/wiki/Mmap) function, which provides *demand driven* loading of pages from disk, so that only those pages actually touched are loaded into memory, and unneeded memory is not allocated.

#### copy-on-edit

[MATLAB](https://en.wikipedia.org/wiki/MATLAB) implements *copy on edit*, where arrays which are copied have their actual memory storage replicated only when their content is changed, possibly leading to an *out of memory* error when updating an element afterwards instead of during the copy operation.

## Implementation

Some programming languages delay evaluation of expressions by default, and some others provide [functions](https://en.wikipedia.org/wiki/Subroutine) or special [syntax](https://en.wikipedia.org/wiki/Syntax_of_programming_languages) to delay evaluation. In [Miranda](https://en.wikipedia.org/wiki/Miranda_(programming_language)) and [Haskell](https://en.wikipedia.org/wiki/Haskell_(programming_language)), evaluation of function arguments is delayed by default. In many other languages, evaluation can be delayed by explicitly suspending the computation using special syntax (as with [Scheme's](https://en.wikipedia.org/wiki/Scheme_(programming_language)) "`delay`" and "`force`" and [OCaml](https://en.wikipedia.org/wiki/OCaml)'s "`lazy`" and "`Lazy.force`") or, more generally, by wrapping the expression in a [thunk](https://en.wikipedia.org/wiki/Thunk_(delayed_computation)). The object representing such an explicitly delayed evaluation is called a *lazy future.* [Perl 6](https://en.wikipedia.org/wiki/Perl_6) uses lazy evaluation of lists, so one can assign infinite lists to variables and use them as arguments to functions, but unlike Haskell and Miranda, Perl 6 doesn't use lazy evaluation of arithmetic operators and functions by default.[[12\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-Wadler2006-12)

## Laziness and eagerness

### Controlling eagerness in lazy languages

### Simulating laziness in eager languages

#### Python

In [Python](https://en.wikipedia.org/wiki/Python_(programming_language)) 2.x the `range()` function[[18\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-18) computes a list of integers. The entire list is stored in memory when the first assignment statement is evaluated, so this is an example of **eager** or immediate evaluation:

```python
 >>> r = range(10)
 >>> print r
 [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
 >>> print r[3]
 3
```

In Python 3.x the `range()` function[[19\]](https://en.wikipedia.org/wiki/Lazy_evaluation#cite_note-19) returns a special range object which computes elements of the list on demand. Elements of the range object are only generated when they are needed (e.g., when `print(r[3])` is evaluated in the following example), so this is an example of lazy or deferred evaluation:

```python
 >>> r = range(10)
 >>> print(r)
 range(0, 10)
 >>> print(r[3])
 3
```





# 20181217

## lazy evaluation
==已经阅读==  https://www.cnblogs.com/tiger-xc/p/4341035.html

==已经阅读==
https://en.wikipedia.org/wiki/Lazy_evaluation

python lazy evaluation的作用：
http://python.jobbole.com/85553/



# 