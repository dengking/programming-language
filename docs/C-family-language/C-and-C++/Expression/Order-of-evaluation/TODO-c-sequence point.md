# [comp.lang.c FAQ list · Question 3.8](http://c-faq.com/expr/seqpoints.html)

## **Q:** 

How can I understand complex expressions like the ones in this section, and avoid writing undefined ones? What's a ``sequence point''?

## **A:**

A sequence point is a point in **time** at which the dust has settled and all side effects which have been seen so far are guaranteed to be complete. The **sequence points** listed in the C standard are:

- at the end of the evaluation of a [full expression](http://c-faq.com/sx1/index.html#full expression) (a full expression is an **expression statement**, or any other expression which is not a subexpression within any larger expression);

- at the `||`, `&&`, `?:`, and comma operators; and

- at a function call (after the evaluation of all the arguments, and just before the actual call).

  ***SUMMARY*** : 这说明`f(i++)`是允许的，因为它会先执行`i++`

The Standard states that

> Between the previous and next **sequence point** an object shall have its stored value modified at most once by the evaluation of an expression. Furthermore, the prior value shall be accessed only to determine the value to be stored.

***SUMMARY*** : 上面这段话的最后一句话其实说明的是这种情况：`i = i + 1`

These two rather opaque sentences say several things. First, they talk about operations bounded by the `previous and next sequence points`; such operations usually correspond to **full expressions**. (In an expression statement, `the next sequence point` is usually at the terminating semicolon, and the `previous sequence point` is at the end of the previous statement. An expression may also contain **intermediate sequence points**, as listed above.)

The first sentence rules out both the examples

```c
i++ * i++
```

and

```c
i = i++
```

***SUMMARY*** : `i++`有side effect，但是在上述两个expression中，没有sequence point来保证它的effect已经完成了，所以上述expression是存在歧义的，而对于program而言，是万万不可存在歧义的，所以它就是undefined behavior；

from questions [3.2](http://c-faq.com/expr/evalorder2.html) and [3.3](http://c-faq.com/expr/ieqiplusplus.html)--in both cases, `i` has its value modified twice within the expression, i.e. between sequence points. (If we were to write a similar expression which did have an internal sequence point, such as

```c
i++ && i++
```

it *would* be well-defined, if questionably useful.)

The second sentence can be quite difficult to understand. It turns out that it disallows code like

```c
	a[i] = i++
```

from question [3.1](http://c-faq.com/expr/evalorder1.html). (Actually, the other expressions we've been discussing are in violation of the second sentence, as well.) To see why, let's first look more carefully at what the Standard is trying to allow and disallow.

Clearly, expressions like

```c
	a = b
```

and

```c
	c = d + e
```

which read some values and use them to write others, are well-defined and legal. Clearly, [[footnote\]](http://c-faq.com/expr/fn16.html)

```
i = i++
```

which modify the same value twice are abominations which needn't be allowed (or in any case, needn't be well-defined, i.e. we don't have to figure out a way to say what they do, and compilers don't have to support them). Expressions like these are disallowed by the **first sentence**.

It's also clear [[footnote\]](http://c-faq.com/expr/fn16.html) that we'd like to disallow expressions like

```c
	a[i] = i++
```

which modify `i`*and*

```c
	i = i + 1
```

which use and modify `i` but only  modify it later when it's reasonably easy to ensure that the final store of the final value (into `i`, in this case) doesn't interfere with the earlier accesses.

And that's what the **second sentence** says: if an object is written to within a full expression, any and all accesses to it within the same expression must be directly involved in the computation of the value to be written. This rule effectively constrains legal expressions to those in which the accesses demonstrably precede the modification. For example, the old standby `i = i + 1` is allowed, because the access of `i` is used to determine `i`'s final value. The example

```c
a[i] = i++
```

is disallowed because one of the accesses of `i` (the one in `a[i]`) has nothing to do with the value which ends up being stored in `i` (which happens over in `i++`), and so there's no good way to define--either for our understanding or the compiler's--whether the access should take place before or after the incremented value is stored. Since there's no good way to define it, the Standard declares that it is undefined, and that portable programs simply must not use such constructs.



See also questions [3.9](http://c-faq.com/expr/evalorder4.html) and [3.11](http://c-faq.com/expr/confused.html).