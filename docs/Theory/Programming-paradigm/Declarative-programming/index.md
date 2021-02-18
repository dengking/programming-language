# Declarative programming



## ybrikman [Six programming paradigms that will change how you think about coding](https://www.ybrikman.com/writing/2014/04/09/six-programming-paradigms-that-will/)

> NOTE: 在 ybrikman [Six programming paradigms that will change how you think about coding](https://www.ybrikman.com/writing/2014/04/09/six-programming-paradigms-that-will/) 中，对 declarative programming 有着非常好的描述:

Here’s the gist: in most mainstream languages, you describe *how* to solve a particular problem; in declarative languages, you merely describe *the result you want*, and the language itself figures out how to get there.

If you were sorting numbers in a declarative language like [Prolog](https://en.wikipedia.org/wiki/Prolog), you’d instead describe the output you want: “I want the same list of values, but each item at index `i` should be less than or equal to the item at index `i + 1`”. Compare the previous C solution to this Prolog code:

```prolog
sort_list(Input, Output) :-
  permutation(Input, Output),
  check_order(Output).
  
check_order([]).
check_order([Head]).
check_order([First, Second | Tail]) :-
  First =< Second,
  check_order([Second | Tail]).
```

If you’ve used SQL, you’ve done a form of declarative programming and may not have realized it: when you issue a query like `select X from Y where Z`, you are describing the data set you’d like to get back; it’s the database engine that actually figures out *how* to execute the query. You can use the explain command in most databases to see the execution plan and figure out what happened under the hood.

The beauty of declarative languages is that they allow you to work at a much higher level of abstraction: your job is just to describe the specification for the output you want. 