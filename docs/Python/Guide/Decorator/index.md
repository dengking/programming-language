# Python decorator 

## wikipedia [Python Decorators](https://en.wikipedia.org/wiki/Python_syntax_and_semantics#Decorators)

See also: [Advice (computer science)](https://en.wikipedia.org/wiki/Advice_(computer_science))

A decorator is any **callable Python object** that is used to modify a **function**, **method** or **class definition**. A decorator is passed the **original** object being defined and returns a **modified** object, which is then bound to the name in the definition. Python decorators were inspired in part by [Java annotations](https://en.wikipedia.org/wiki/Java_annotation), and have a similar syntax; the decorator syntax is pure [syntactic sugar](https://en.wikipedia.org/wiki/Syntactic_sugar), using `@` as the keyword:

```Python
@viking_chorus
def menu_item():
    print("spam")
```

is equivalent to

```Python
def menu_item():
    print("spam")
menu_item = viking_chorus(menu_item)
```

Decorators are a form of [metaprogramming](https://en.wikipedia.org/wiki/Metaprogramming); they **enhance** the action of the function or method they decorate. For example, in the sample below, `viking_chorus` might cause `menu_item` to be run 8 times (see [Spam sketch](https://en.wikipedia.org/wiki/Spam_(Monty_Python))) for each time it is called:

```Python
def viking_chorus(myfunc):
    def inner_func(*args, **kwargs):
        for i in range(8):
            myfunc(*args, **kwargs)
    return inner_func
```

> NOTE: 
>
> 一、`inner_func` 能够capture 入参 `myfunc`，即 "the **original** object "，显然它的implementation是使用了lambda的
>
> 二、它是由Python interpreter来进行自动调用的
>
> 三、Python的decorator是一定要有返回值的，否则无法进行重新绑定
>
> 四、`inner_func` 实现了对原函数的decoration

### Canonical uses of function decorators 

Canonical uses of function decorators are for

1、creating [class methods](https://en.wikipedia.org/wiki/Class_method) or [static methods](https://en.wikipedia.org/wiki/Static_method)

2、adding function attributes

3、[tracing](https://en.wikipedia.org/wiki/Tracing_(software))

4、setting [pre-](https://en.wikipedia.org/wiki/Precondition) and [postconditions](https://en.wikipedia.org/wiki/Postcondition)

5、[synchronisation](https://en.wikipedia.org/wiki/Synchronisation),[[23\]](https://en.wikipedia.org/wiki/Python_syntax_and_semantics#cite_note-26)

6、[tail recursion elimination](https://en.wikipedia.org/wiki/Tail_recursion_elimination),[[24\]](https://en.wikipedia.org/wiki/Python_syntax_and_semantics#cite_note-27)

7、[memoization](https://en.wikipedia.org/wiki/Memoization)

8、improving the writing of decorators.[[25\]](https://en.wikipedia.org/wiki/Python_syntax_and_semantics#cite_note-28)

### Decorator chain 

Decorators can be chained by placing several on adjacent lines:

```python
@invincible
@favourite_colour("Blue")
def black_knight():
    pass
```

is equivalent to

```Python
def black_knight():
    pass
black_knight = invincible(favourite_colour("Blue")(black_knight))
```

or, using intermediate variables

```Python
def black_knight():
    pass
blue_decorator = favourite_colour("Blue")
decorated_by_blue = blue_decorator(black_knight)
black_knight = invincible(decorated_by_blue)
```

In the example above, the `favourite_colour` decorator [factory](https://en.wikipedia.org/wiki/Factory_(software_concept)) takes an argument. **Decorator factories** must return a decorator, which is then called with the object to be decorated as its argument:

```Python
def favourite_colour(colour):
    def decorator(func):
        def wrapper():
            print(colour)
            func()
        return wrapper
    return decorator
```

> NOTE: 
>
> 一、从上面的解释以及例子来看，**Decorator factories**返回一个decorator，这个decorator会被调用，并且调用的时候传入的参数是**Decorator factories**所修饰的object。其实从目前所看到的这些例子来看，解释器对**decorator**的解释法则就是调用，赋值。
>
> 二、所有的argument都会被捕获
>
> 三、decorator是一个**callable**，它所返回的应该是一个和它所修饰的相似类型的object，比如，如果一个decorator所修饰的是一个function，那么它返回的应该也是一个function。



This would then decorate the `black_knight` function such that the `colour`, "Blue", would be printed prior to the `black_knight` function running. **[Closure](https://en.wikipedia.org/wiki/Closure_(computer_programming))** ensures that the `colour` argument is accessible to the innermost wrapper function even when it is returned and goes out of scope, which is what allows decorators to work.

> NOTE: 
>
> 上面这段话中的**[Closure](https://en.wikipedia.org/wiki/Closure_(computer_programming))**是非常重要的。

Despite the name, Python decorators are not an implementation of the [decorator pattern](https://en.wikipedia.org/wiki/Decorator_pattern). The decorator pattern is a [design pattern](https://en.wikipedia.org/wiki/Design_pattern) used in [statically typed](https://en.wikipedia.org/wiki/Statically_typed) [object-oriented programming languages](https://en.wikipedia.org/wiki/Object-oriented_programming_language) to allow functionality to be added to objects at run time; Python decorators add functionality to functions and methods at definition time, and thus are a higher-level construct than decorator-pattern classes. The decorator pattern itself is trivially implementable in Python, because the language is [duck typed](https://en.wikipedia.org/wiki/Duck_typed), and so is not usually considered as such.



## Python decorator VS descriptor

decorator是一种语法糖；

descriptor是一种python attribute access  protocol；

现在想来，要理解decorator和descriptor之间的关系，最好的例子是通过decorator的方式来使用`property`。因为python的`property`是一种descriptor，而它是可以通过decorator的方式来进行使用的。



## Python decorator adding function attributes

stackoverflow [How can I pass a variable in a decorator to function's argument in a decorated function?](https://stackoverflow.com/questions/1965607/how-can-i-pass-a-variable-in-a-decorator-to-functions-argument-in-a-decorated-f)

## Python decorator add attribute to object

1、stackoverflow [How can I add properties to a class using a decorator that takes a list of names as argument?](https://stackoverflow.com/questions/9125832/how-can-i-add-properties-to-a-class-using-a-decorator-that-takes-a-list-of-names)



## decorate class 

1、stackoverflow [How to decorate a class?](https://stackoverflow.com/questions/681953/how-to-decorate-a-class)

2、kite [How to decorate a class in Python](https://www.kite.com/python/answers/how-to-decorate-a-class-in-python)

3、pencilprogrammer [How to Decorate a class in Python?](https://pencilprogrammer.com/decorate-python-class/)

## class decorator

1、towardsdatascience [Using Class Decorators in Python](https://towardsdatascience.com/using-class-decorators-in-python-2807ef52d273)



## function logger

### stackoverflow [How to do logging at function entry, inside and exit in Python](https://stackoverflow.com/questions/23435488/how-to-do-logging-at-function-entry-inside-and-exit-in-python)