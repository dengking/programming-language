# gotw [GotW #102: Exception-Safe Function Calls (Difficulty: 7/10)](https://herbsutter.com/gotw/_102/)

> NOTE: 
>
> 在阅读 [Advantages of using std::make_unique over new operator [duplicate]](https://stackoverflow.com/questions/37514509/advantages-of-using-stdmake-unique-over-new-operator)  # [A](https://stackoverflow.com/a/37514627) 时，其中提及了这篇文章

*[This is a C++11-updated version of the original* [GotW #56](http://www.gotw.ca/gotw/056.htm)*.]*

## JG Questions

### 1、

In each of the following statements, what can you say about the order of evaluation of the functions *f*, *g*, and *h* and the expressions *expr1* and *expr2*? Assume that *expr1* and *expr2* do not contain more function calls.

```C++
// Example 1(a)
//
f( expr1, expr2 );
 
// Example 1(b)
//
f( g( expr1 ), h( expr2 ) );
```



## Guru Questions

### 2、

In your travels through the dusty corners of your company’s code archives, you find the following code fragment:

```C++
// Example 2
 
// In some header file:
void f( T1*, T2* );
 
// At some call site:
f( new T1, new T2 );
```



Does this code have any potential exception safety or other problems? Explain.

### 3、

As you continue to root through the archives, you see that someone must not have liked Example 2 because later versions of the files in question were changed as follows:

```C++
// Example 3
 
// In some header file:
void f( std::unique_ptr<T1>, std::unique_ptr<T2> );
 
// At some call site:
f( std::unique_ptr<T1>{ new T1 }, std::unique_ptr<T2>{ new T2 } );
```



What improvements does this version offer over Example 2, if any? Do any exception safety problems remain? Explain.

\4. Demonstrate how to write a *make_unique* facility that solves the safety problems in Example 3 and can be invoked as follows:

```c++
// Example 4
 
// In some header file:
void f( std::unique_ptr<T1>, std::unique_ptr<T2> );
 
// At some call site:
f( make_unique<T1>(), make_unique<T2>() );
```

## Solution

### 1、

> NOTE: 
>
> 1、这一节重要讨论order of evaluation，参见 `C++Order-of-evaluation` 章节
>
> 2、本节的内容是非常好的，它可以作为理解function call expression的order of evaluation的实例
>
> 3、通过本节的分析可知:
>
> 下面两个expression，虽然能够实现相同的执行效果，但是expression a可能的ordering是比expression b可能的ordering要多得多的，这是因为expression a中额外多了一层 new expression(包括 `new T1`、 `new T2`)，这就expression a的order of evaluation相比于expression b多了更多的combination，这其中的一些combination是非exception safe的(interleave)，可能导致resource leak的情况，所以最好使用expression b。
>
> a、`f( std::unique_ptr<T1>{ new T1 }, std::unique_ptr<T2>{ new T2 } );`
>
> b、`f( make_unique<T1>(), make_unique<T2>() );`
>
> 这提示了我们，涉及new的expression应该尽可能的简单，在"Guidelines"章节，作者进行了总结

In each of the following statements, what can you say about the order of evaluation of the functions `f`, `g`, and `h` and the expressions `expr1` and `expr2`? Assume that `expr1` and `expr2` do not contain more function calls.



The answer hinges on(遵循) the following basic rules:

1、All of a function’s arguments must be fully evaluated before the function is called. This includes the completion of any side effects of expressions used as function arguments.

2、Once the execution of a function begins, no expressions from the calling function may begin or continue to be evaluated until execution of the called function has completed. **Function executions never interleave with each other**.

> NOTE: 
>
> 1、"**Function executions never interleave with each other**"是一个非常重要的guarantee

3、Expressions used as function arguments may generally be evaluated in any order, including interleaved, except as otherwise restricted by the other rules.





## Some Function Call Exception Safety Problems

### 2、

In your travels through the dusty corners of your company’s code archives, you find the following code fragment:

```C++
// Example 2
 
// In some header file:
void f( T1*, T2* );
 
// At some call site:
f( new T1, new T2 );
```



Does this code have any potential exception safety or other problems? Explain.

Yes, there are several potential exception safety problems.

Brief recap(简明扼要地重述): An expression such as *new T1* is called, simply enough, a new-expression. Recall what a new-expression really does (ignoring in-place and array forms for simplicity, because they’re not very relevant here):

1、it allocates memory;

2、it constructs a new object in that memory; and

3、if the construction fails because of an exception the allocated memory is freed.

So each new-expression is essentially a series of two function calls: 

1、one call to *operator new* (either the global one, or one provided by the type of the object being created), and then 

2、a call to the constructor.



## Guidelines: