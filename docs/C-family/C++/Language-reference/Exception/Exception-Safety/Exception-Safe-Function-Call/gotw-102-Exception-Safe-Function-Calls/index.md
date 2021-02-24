# gotw [GotW #102: Exception-Safe Function Calls (Difficulty: 7/10)](https://herbsutter.com/gotw/_102/)

> NOTE: 
>
> 在阅读 [Advantages of using std::make_unique over new operator [duplicate]](https://stackoverflow.com/questions/37514509/advantages-of-using-stdmake-unique-over-new-operator)  # [A](https://stackoverflow.com/a/37514627) 时，其中提及了这篇文章

*[This is a C++11-updated version of the original* [GotW #56](http://www.gotw.ca/gotw/056.htm)*.]*

## JG Questions

\1. In each of the following statements, what can you say about the order of evaluation of the functions *f*, *g*, and *h* and the expressions *expr1* and *expr2*? Assume that *expr1* and *expr2* do not contain more function calls.

```C++
// Example 1(a)
//
f( expr1, expr2 );
 
// Example 1(b)
//
f( g( expr1 ), h( expr2 ) );
```



## Guru Questions

\2. In your travels through the dusty corners of your company’s code archives, you find the following code fragment:

```C++
// Example 2
 
// In some header file:
void f( T1*, T2* );
 
// At some call site:
f( new T1, new T2 );
```



Does this code have any potential exception safety or other problems? Explain.

\3. As you continue to root through the archives, you see that someone must not have liked Example 2 because later versions of the files in question were changed as follows:

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