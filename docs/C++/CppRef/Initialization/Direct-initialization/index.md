# cppreference [Direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization)

> NOTE: 
>
> 1、这种是最容易理解的
>
> 2、它和value initialization非常类似，不同在于它是带参的

|                                                     |      |                                                              |
| --------------------------------------------------- | ---- | ------------------------------------------------------------ |
| T object ( arg );<br/>T object ( arg1, arg2, ... ); |      |                                                              |
| T object { arg };                                   |      |                                                              |
| T ( other )<br/>T ( arg1, arg2, ... )               |      | [functional cast](https://en.cppreference.com/w/cpp/language/explicit_cast) |
| `static_cast< T >( other )`                         |      | explicit conversion                                          |
| `new T(args, ...)`                                  |      |                                                              |
| `Class::Class() : member(args, ...) { ... }`        |      |                                                              |
| `[arg](){ ... }`                                    |      |                                                              |



## Effect

> NOTE: 
>
> 1、目前的认知是，对应的constructor会被调用

## Notes



### Constructor

Direct-initialization is more permissive(自由) than copy-initialization: copy-initialization only considers non-[explicit](https://en.cppreference.com/w/cpp/language/explicit) constructors and non-explicit user-defined [conversion functions](https://en.cppreference.com/w/cpp/language/cast_operator), while direct-initialization considers all constructors and all user-defined conversion functions.

> NOTE: 
>
> 1、上述 [conversion functions](https://en.cppreference.com/w/cpp/language/cast_operator) 其实就是"user-defined conversion function"

### Syntax ambiguity 

> NOTE: 
>
> 1、这部分内容放到了 `Syntax-ambiguity-in-initialization-with-round-parenthese` 章节
>
> 