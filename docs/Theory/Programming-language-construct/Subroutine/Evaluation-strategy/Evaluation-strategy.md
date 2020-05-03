# [Evaluation strategy](https://en.wikipedia.org/wiki/Evaluation_strategy)

**Evaluation strategies** are used by [programming languages](https://en.wikipedia.org/wiki/Programming_languages) to determine *when* to evaluate the argument(s) （参数）of a function call (for function, also read: operation, method, or relation) and *what kind* of value to pass to the function. 

总结：这段话是非常重要的，它的描述是非常清楚的；有的时候，我们调用一个函数的时候，传递给函数的参数不是一个值，而是一个表达式；那这就涉及到了一个问题：什么时候evaluate这个表达式的值呢？显然evaluation strategy就是描述这个问题的。

For example, call by value/call by reference specifies that a function application evaluates the argument before it proceeds to the evaluation of the function's body and that it passes two capabilities to the function, namely, the ability to look up the current value of the argument and to modify it via an assignment statement.[[1\]](https://en.wikipedia.org/wiki/Evaluation_strategy#cite_note-1) The notion of [reduction strategy](https://en.wikipedia.org/wiki/Reduction_strategy_(lambda_calculus)) in [lambda calculus](https://en.wikipedia.org/wiki/Lambda_calculus) is similar but distinct.

总结：这段话的含义是：call by value/call by reference指定在evaluate function body之前先evaluate argument，也就是它具有look up参数当前值的能力和通过赋值语句对齐修改的能力。

In practical terms, many modern programming languages have converged(融合) on a call-by-value/call-by-reference[*clarification needed*] evaluation strategy for function calls (C#, Java). Some languages, especially lower-level languages such as C++, combine several notions of parameter passing. Historically, call by value and call by name date back to [ALGOL 60](https://en.wikipedia.org/wiki/ALGOL_60), a language designed in the late 1950s. Call by reference is used by [PL/I](https://en.wikipedia.org/wiki/PL/I) and some [Fortran](https://en.wikipedia.org/wiki/Fortran) systems.[[2\]](https://en.wikipedia.org/wiki/Evaluation_strategy#cite_note-2) Purely functional languages like [Haskell](https://en.wikipedia.org/wiki/Haskell_(programming_language)), as well as non-purely functional languages like [R](https://en.wikipedia.org/wiki/R_(programming_language)), use call by need.

The evaluation strategy is specified by the programming language definition, and is not a function of any specific implementation.

总结：evaluation strategy由编程语言定义指定，而不是任何特定实现的功能。

## evaluation strategy 分类

- 1 [Strict evaluation](https://en.wikipedia.org/wiki/Evaluation_strategy#Strict_evaluation)

  - 1.1 [Applicative order](https://en.wikipedia.org/wiki/Evaluation_strategy#Applicative_order)
  - 1.2 [Call by value](https://en.wikipedia.org/wiki/Evaluation_strategy#Call_by_value)
    - 1.2.1 [Implicit limitations](https://en.wikipedia.org/wiki/Evaluation_strategy#Implicit_limitations)
  - 1.3 [Call by reference](https://en.wikipedia.org/wiki/Evaluation_strategy#Call_by_reference)
  - 1.4 [Call by sharing](https://en.wikipedia.org/wiki/Evaluation_strategy#Call_by_sharing)
  - 1.5 [Call by copy-restore](https://en.wikipedia.org/wiki/Evaluation_strategy#Call_by_copy-restore)
  - 1.6 [Partial evaluation](https://en.wikipedia.org/wiki/Evaluation_strategy#Partial_evaluation)

- 2 [Non-strict evaluation](https://en.wikipedia.org/wiki/Evaluation_strategy#Non-strict_evaluation)

  - 2.1 [Normal order](https://en.wikipedia.org/wiki/Evaluation_strategy#Normal_order)
  - 2.2 [Call by name](https://en.wikipedia.org/wiki/Evaluation_strategy#Call_by_name)
  - 2.3 [Call by need](https://en.wikipedia.org/wiki/Evaluation_strategy#Call_by_need)
  - 2.4 [Call by macro expansion](https://en.wikipedia.org/wiki/Evaluation_strategy#Call_by_macro_expansion)

- 3 [Nondeterministic strategies](https://en.wikipedia.org/wiki/Evaluation_strategy#Nondeterministic_strategies)

  - 3.1 [Full β-reduction](https://en.wikipedia.org/wiki/Evaluation_strategy#Full_%CE%B2-reduction)
  - 3.2 [Call by future](https://en.wikipedia.org/wiki/Evaluation_strategy#Call_by_future)
  - 3.3 [Optimistic evaluation](https://en.wikipedia.org/wiki/Evaluation_strategy#Optimistic_evaluation)


## Strict evaluation

Main article: [Eager evaluation](https://en.wikipedia.org/wiki/Eager_evaluation)

In *strict evaluation,* the arguments to a [function](https://en.wikipedia.org/wiki/Subroutine) are always evaluated completely before the function is applied.

Under [Church encoding](https://en.wikipedia.org/wiki/Church_encoding), [eager evaluation](https://en.wikipedia.org/wiki/Eager_evaluation) of [operators](https://en.wikipedia.org/wiki/Operator_(programming)) maps to strict evaluation of functions; for this reason, strict evaluation is sometimes called "eager". Most existing programming languages use strict evaluation for functions.

总结：也叫eager evaluation



### Call by sharing

*Call by sharing* (also referred to as *call by object* or *call by object-sharing*) is an **evaluation strategy** first named by [Barbara Liskov](https://en.wikipedia.org/wiki/Barbara_Liskov) et al. for the language [CLU](https://en.wikipedia.org/wiki/CLU_programming_language) in 1974.[[6\]](https://en.wikipedia.org/wiki/Evaluation_strategy#cite_note-CLU_Reference_Manual-6)It is used by languages such as [Python](https://en.wikipedia.org/wiki/Python_(programming_language)),[[7\]](https://en.wikipedia.org/wiki/Evaluation_strategy#cite_note-Lundh_Call_By_Object-7) [Iota](https://en.wikipedia.org/wiki/Iota_and_Jot),[[8\]](https://en.wikipedia.org/wiki/Evaluation_strategy#cite_note-8) [Java](https://en.wikipedia.org/wiki/Java_(programming_language)) (for object references), [Ruby](https://en.wikipedia.org/wiki/Ruby_(programming_language)), [JavaScript](https://en.wikipedia.org/wiki/JavaScript), Scheme, OCaml, [AppleScript](https://en.wikipedia.org/wiki/AppleScript), and many others. However, the term "call by sharing" is not in common use; the **terminology** is inconsistent across different sources. For example, in the Java community, they say that Java is **call by value**.[[9\]](https://en.wikipedia.org/wiki/Evaluation_strategy#cite_note-9) **Call by sharing** implies that **values** in the language are based on **objects** rather than [primitive types](https://en.wikipedia.org/wiki/Primitive_types), i.e. that all values are "[boxed](https://en.wikipedia.org/wiki/Boxed_type)". Can be said to **pass by copy of reference**(where **primitives** are boxed before passing and unboxed at called function).

The semantics of **call by sharing** differ from **call by reference**: "In particular it is not **call by value** because mutations of arguments performed by the called routine will be visible to the caller. And it is not **call by reference** because access is not given to the variables of the caller, but merely to certain objects（这段关于它和call by reference的对比没有理解清楚）"[[10\]](https://en.wikipedia.org/wiki/Evaluation_strategy#cite_note-FOOTNOTECLU_Reference_Manual197414-15-10). So e.g. if a variable was passed, it is not possible to simulate an **assignment** on that variable in the callee's scope[[11\]](https://en.wikipedia.org/wiki/Evaluation_strategy#cite_note-11). However, since the function has access to the same object as the caller (no copy is made), mutations to those objects, if the objects are [mutable](https://en.wikipedia.org/wiki/Mutable_object), within the function are visible to the caller, which may appear to differ from call by value semantics. Mutations of a mutable object within the function are visible to the caller because the object is not copied or cloned — it is shared. For example, in Python, lists are mutable, so:

```python
def f(list):
    list.append(1)

m = []
f(m)
print(m)
```

outputs `[1]` because the `append` method modifies the object on which it is called.

**Assignments** within a function are not noticeable to the caller, because, in these languages, **passing the variable** only means passing (access to) the actual object referred to by the variable, not access to the original (caller's) variable. Since the **rebound variable** only exists within the scope of the function, the counterpart in the caller retains its original binding. Compare the Python mutation above with this code that binds the formal argument to a new object:

```
def f(list):
    list = [1]

m = []
f(m)
print(m)
```

outputs `[]`, because the statement `list = [1]` reassigns a new list to the *variable* rather than to the location it references.

For [immutable objects](https://en.wikipedia.org/wiki/Immutable_object), there is no real difference between call by sharing and call by value, except if object identity is visible in the language. The use of call by sharing with mutable objects is an alternative to [input/output parameters](https://en.wikipedia.org/wiki/Output_parameter):[[12\]](https://en.wikipedia.org/wiki/Evaluation_strategy#cite_note-CA1021-12) the parameter is not assigned to (the argument is not overwritten and object identity is not changed), but the object (argument) is mutated.

Although this term has widespread usage in the Python community, identical semantics in other languages such as Java and Visual Basic are often described as **call by value**, where the value is implied to be a reference to the object.[*citation needed*]

## Non-strict evaluation

In *non-strict evaluation,* arguments to a function are not evaluated unless they are actually used in the evaluation of the function body.

总结：

Under Church encoding, [lazy evaluation](https://en.wikipedia.org/wiki/Lazy_evaluation) of operators maps to non-strict evaluation of functions; for this reason, non-strict evaluation is often referred to as "lazy". Boolean expressions in many languages use a form of non-strict evaluation called [short-circuit evaluation](https://en.wikipedia.org/wiki/Short-circuit_evaluation), where evaluation returns as soon as it can be determined that an unambiguous Boolean will result—for example, in a disjunctive expression where *true* is encountered, or in a conjunctive expression where *false* is encountered, and so forth. Conditional expressions also usually use lazy evaluation, where evaluation returns as soon as an unambiguous branch will result.

总结：也叫lazy evaluation