# Execution model[¶](https://docs.python.org/3/reference/executionmodel.html#execution-model)

## 4.1. Structure of a program

> NOTE: 这才是核心所在，后续的很多概念都是基于此而创建的；

A Python program is constructed from **code blocks**. A *block* is a piece of Python program text that is executed as a **unit**（单位）. The following are blocks: a module（the top-level **code block**） ,a function body and a **class definition**(执行class definition的结果是创建了这个class). Each command typed interactively is a **block**. A script file (a file given as standard input to the interpreter or specified as a command line argument to the interpreter) is a **code block**. A script command (a command specified on the interpreter command line with the ‘**-c**’ option) is a **code block**. The string argument passed to the built-in functions [`eval()`](https://docs.python.org/3/library/functions.html#eval)and [`exec()`](https://docs.python.org/3/library/functions.html#exec) is a code block.

A **code block** is executed in an *execution frame*. A **frame** contains some **administrative information** (used for debugging) and determines where and how execution continues after the code block’s execution has completed.

> NOTE: python interpreter对code block的判断应该会受level of indentation的影响，关于python interpreter是如何来判断level of indentation，参考<https://docs.python.org/3/reference/lexical_analysis.html#indentation>

> NOTE: 上面这段中提及了各种各样的code block，在python中，都有对应的standard type来描述它们；比如module对应的就是python的[standard type hierarchy](https://docs.python.org/3/reference/datamodel.html#the-standard-type-hierarchy)中的Modules

> NOTE: **execution frame**是python的[standard type hierarchy](https://docs.python.org/3/reference/datamodel.html#the-standard-type-hierarchy)中为Internal types中的Frame objects；这让我想起来[call stack](https://en.wikipedia.org/wiki/Call_stack)，[stack frame](https://en.wikipedia.org/wiki/Call_stack#Structure)；显然python中的execution frame是由python解释器来进行维护，对于编译型语言，则有OS来进行维护；

> NOTE: 需要区分**code block**和**callable**（在[standard type hierarchy](https://docs.python.org/3/reference/datamodel.html#the-standard-type-hierarchy)给出的定义）；callable的定义是指： types to which the function call operation (see section [Calls](https://docs.python.org/3.7/reference/expressions.html#calls)) can be applied；显然module不是callable，所以code block不是callable；这里指出code block是指python program的structure，也就是我们在编写python program的时候，往往会创建一个module，然后在module中定义function，定义class；然后我们使用python interpreter来execute module，这才是最后一段话中A **code block** is executed in an *execution frame*的含义；与此类似的是，python interpreter还可以execute class，function，显然python interpreter execute class的结果是定义了一个class；对一个class执行call operation的结果是创建一个instance，对一个function执行call operation的结果是execute这个function的function body；



TODO: 

see also :

- https://docs.python.org/2.0/ref/execframes.html



## 4.2. Naming and binding

### 4.2.1. Binding of names

*Names* refer to objects. Names are introduced by **name binding operations**.

> NOTE：python中一切皆对象，并且python是通过name来access对象的。那这段中的name的含义到底是什么呢？name就是identifier吗？根据Wikipedia上的关于name binding的解释，可以推测name可以理解为identifier；在youdao 20181030中收录了name binding。并且在6.2.1. Identifiers (Names)[¶](https://docs.python.org/3/reference/expressions.html#atom-identifiers)中也将name和identifier视为同义词；

> NOTE：显然name-binding是发生在runtime，只有当program runtime，才能够知道name到底bind到哪个object。

> NOTE : 不同的programming language中对name的处理方式是不同的，显然python中name和`c++`中的name是截然不同的；

The following constructs bind names: 

- formal parameters to functions（函数形参）
- [`import`](https://docs.python.org/3/reference/simple_stmts.html#import) statements
- class and function definitions (these bind the class or function **name** in the defining block)
- targets that are identifiers if occurring in an **assignment**
- [`for`](https://docs.python.org/3/reference/compound_stmts.html#for) loop header
- after [`as`](https://docs.python.org/3/reference/compound_stmts.html#as) in a [`with`](https://docs.python.org/3/reference/compound_stmts.html#with) statement or [`except`](https://docs.python.org/3/reference/compound_stmts.html#except) clause. 

The [`import`](https://docs.python.org/3/reference/simple_stmts.html#import)statement of the form `from ... import *` binds all names defined in the imported module, except those beginning with an underscore. This form may only be used at the module level（关于这一点，参考Reserved classes of identifiers[¶](https://docs.python.org/3/reference/lexical_analysis.html#reserved-classes-of-identifiers)）.

> NOTE：因为python中一切皆对象，所以programmer所定义的类，也是用一个对象来表示该类的。显然在python中，只有通过上面这段中所描述的方式后，programmer才能够通过类名来reference表示该类的object。显然这和编译型语言是不同的。

> NOTE: 理解python的binding of names是理解python的function pass by sharing的前提，这个问题在function章节有专门的介绍；显然它属于上面所列的第一种情况：formal parameters to functions。

A target occurring in a [`del`](https://docs.python.org/3/reference/simple_stmts.html#del) statement is also considered bound for this purpose (though the actual semantics are to unbind the name).

Each **assignment** or **import** statement occurs within a **block** defined by a class or function definition or at the module level (the top-level **code block**).

***SUMMARY*** :其实主要也就这三种code block；

If a name is bound in a block, it is a **local variable** of that block, unless declared as [`nonlocal`](https://docs.python.org/3/reference/simple_stmts.html#nonlocal) or [`global`](https://docs.python.org/3/reference/simple_stmts.html#global). If a name is bound at the **module** level, it is a **global variable**. (The variables of the module code block are local and global.) If a variable is used in a code block but not defined there, it is a ***free variable***.

***SUMMARY***：上面这段中的**bound**的含义是该name所对应的object在该block中被创建的。

***SUMMARY*** : free variable在Wikipedia的[Closure (computer programming)](https://en.wikipedia.org/wiki/Closure_(computer_programming))中有介绍，python中出现**free variable**的场景decorator，嵌套函数定义；

***SUMMARY***：上面这段关于free variable的描述是非常模糊的，下面这段是摘自Wikipedia中[Free variables and bound variables](https://en.wikipedia.org/wiki/Free_variables_and_bound_variables)中关于Free variables的解释：

>In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), the term **free variable** refers to [variables](https://en.wikipedia.org/wiki/Variable_(programming)) used in a [function](https://en.wikipedia.org/wiki/Function_(computer_science)) that are neither [local variables](https://en.wikipedia.org/wiki/Local_variable) nor [parameters](https://en.wikipedia.org/wiki/Parameter_(computer_programming)) of that function. The term [non-local variable](https://en.wikipedia.org/wiki/Non-local_variable) is often a synonym in this context.
>
>A **bound variable** is a variable that was previously *free*, but has been *bound* to a specific value or set of values called [domain of discourse](https://en.wikipedia.org/wiki/Domain_of_discourse) or [universe](https://en.wikipedia.org/wiki/Universe_(mathematics)). 

***SUMMARY*** : 上面这段介绍了python中的三种variable；其实上面这段话也体现了python中的name和variable之间的关系：name refer to object，那么对应的object就是variable；

Each occurrence of a **name** in the program text refers to the *binding* of that name established by the following name resolution rules(程序文本中每次出现的名称都是指由以下名称解析规则建立的名称的绑定).



***SUMMARY*** : 逆向思维：name如果没有bound，那会发生什么？与此相关的是[`UnboundLocalError`](https://docs.python.org/3/library/exceptions.html#UnboundLocalError) 。

### 4.2.2. Resolution of names

A *scope* defines the **visibility** of a name within a **block**(scope定义了在一个block中，name的visibility). If a **local variable** is defined in a block, its scope includes that block（也就是该block中的其他地方是可以看到该variable的）. If the definition occurs in a **function block**, the scope extends to any blocks contained within the defining one, unless a contained block introduces a different binding for the name.

***SUMMARY***：关于scope，在compiler principle这本书中进行了非常好的描述；

***SUMMARY***：上面仅仅给出了*scope*的含义，但是并没有给出*scope*的具体的界定方法。python中的*scope*的界定方法是什么呢？参见[Python Scopes and Namespaces](https://docs.python.org/3/tutorial/classes.html#python-scopes-and-namespaces)

***SUMMARY*** : 注意，function的scope规则，即the scope extends to any blocks contained within the defining one，非常有利于python中嵌套函数的使用；

When a name is used in a code block, it is resolved using the **nearest enclosing scope**. The set of all such scopes visible to a code block is called the block’s *environment*.

***SUMMARY*** : every block 有对应的environment，namespace（对应的就是`__dict__`）；

When a name is not found at all, a [`NameError`](https://docs.python.org/3/library/exceptions.html#NameError) exception is raised. If the current scope is a function scope, and the name refers to a **local variable** that has not yet been bound to a **value** at the point where the name is used, an [`UnboundLocalError`](https://docs.python.org/3/library/exceptions.html#UnboundLocalError) exception is raised. [`UnboundLocalError`](https://docs.python.org/3/library/exceptions.html#UnboundLocalError) is a subclass of [`NameError`](https://docs.python.org/3/library/exceptions.html#NameError).

***THINKING*** : 何时会发生 [`UnboundLocalError`](https://docs.python.org/3/library/exceptions.html#UnboundLocalError) ？在build-in exception章节有专门总结；

If a **name binding operation** occurs anywhere within a code block, all uses of the name within the block are treated as **references** to the **current block** （这是非常符合常识的）. This can lead to errors when a name is used within a block before it is bound. This rule is subtle. **Python lacks declarations** and allows **name binding operations** to occur anywhere within a code block. The **local variables** of a code block can be determined by scanning the entire text of the block for **name binding operations**.

***SUMMARY*** : 上面这段话中，提及到了python的一个显著特征：python lacks declaration，这是python和`c++`的一个显著差异；

If the [`global`](https://docs.python.org/3/reference/simple_stmts.html#global) statement occurs within a block, all uses of the name specified in the statement refer to the binding of that name in the **top-level namespace**. Names are resolved in the **top-level namespace** by searching the **global namespace**, i.e. the namespace of the module containing the code block, and the builtins namespace, the namespace of the module [`builtins`](https://docs.python.org/3/library/builtins.html#module-builtins). The global namespace is searched first. If the name is not found there, the builtins namespace is searched. The [`global`](https://docs.python.org/3/reference/simple_stmts.html#global) statement must precede all uses of the name.

The [`global`](https://docs.python.org/3/reference/simple_stmts.html#global) statement has the same scope as a name binding operation in the same block. If the **nearest enclosing scope** for a **free variable** contains a **global statement**, the **free variable** is treated as a global.



The [`nonlocal`](https://docs.python.org/3/reference/simple_stmts.html#nonlocal) statement causes corresponding names to refer to previously bound variables in the nearest enclosing function scope. [`SyntaxError`](https://docs.python.org/3/library/exceptions.html#SyntaxError) is raised at compile time if the given name does not exist in any enclosing function scope.

The **namespace** for a module is automatically created the first time a module is imported. The main module for a script is always called [`__main__`](https://docs.python.org/3/library/__main__.html#module-__main__).

Class definition blocks and arguments to [`exec()`](https://docs.python.org/3/library/functions.html#exec) and [`eval()`](https://docs.python.org/3/library/functions.html#eval) are special in the context of name resolution. A class definition is an **executable statement** that may use and define names. These references follow the normal rules for name resolution with an exception that **unbound local variables** are looked up in the global namespace（与function block有异）. The namespace of the class definition becomes the attribute dictionary of the class. The scope of names defined in a class block is limited to the **class block**; it does not extend to the code blocks of methods – this includes **comprehensions** and **generator expressions** since they are implemented using a **function scope**（与function block有异）. This means that the following will fail:

```python
class A:
    a = 42
    b = list(a + i for i in range(10))
```

***SUMMARY*** : 注意class block和function block之间的差异所在：

***SUMMARY*** : comprehension和generator在6. Expressions[¶](https://docs.python.org/3/reference/expressions.html#expressions)中有专门介绍；

***SUMMARY*** : resolution of name主要描述的是如何找到name所refer的object；对于一个已经bind的name，那么在后续使用中python解释器就需要考虑的一个问题是如何对这个name进行resolve，即找到这个name所refer的object；显然这就涉及到了scope，涉及到了namespace，为此python interpreter定义了一系列的规则；

### 4.2.3. Builtins and restricted execution

**CPython implementation detail:** Users should not touch `__builtins__`; it is strictly an implementation detail. Users wanting to override values in the builtins namespace should [`import`](https://docs.python.org/3/reference/simple_stmts.html#import) the [`builtins`](https://docs.python.org/3/library/builtins.html#module-builtins) module and modify its attributes appropriately.

The builtins namespace associated with the execution of a code block is actually found by looking up the name `__builtins__` in its **global namespace**; this should be a dictionary or a module (in the latter case the module’s dictionary is used). By default, when in the [`__main__`](https://docs.python.org/3/library/__main__.html#module-__main__) module, `__builtins__` is the built-in module [`builtins`](https://docs.python.org/3/library/builtins.html#module-builtins); when in any other module, `__builtins__` is an alias for the dictionary of the [`builtins`](https://docs.python.org/3/library/builtins.html#module-builtins) module itself.



### 4.2.4. Interaction with dynamic features

Name resolution of free variables occurs at runtime, not at compile time. This means that the following code will print 42:

```python
i = 10
def f():
    print(i)
i = 42
f()
```

***SUMMARY*** : 上面这段代码的着重点是表达当调用`f()`的时候，打印的是最后一次`i`的值；

***SUMMARY*** : 如果将上述代码修改为如下：

```python
def f():
    i+=1
    print(i)
```

则会抛出如下异常：

```
UnboundLocalError: local variable 'i' referenced before assignment
```

为什么第一个没有报错？

这种方式是正常的：

```python
def f():
	global i
    i+=1
    print(i)
```



显然，这个问题涉及到了python中的free variable，还涉及到了python的runtime和compile time；

The [`eval()`](https://docs.python.org/3/library/functions.html#eval) and [`exec()`](https://docs.python.org/3/library/functions.html#exec) functions do not have access to the full environment for resolving names. Names may be resolved in the local and global namespaces of the caller. Free variables are not resolved in the nearest enclosing namespace, but in the global namespace. [[1\]](https://docs.python.org/3/reference/executionmodel.html#id3) The [`exec()`](https://docs.python.org/3/library/functions.html#exec) and [`eval()`](https://docs.python.org/3/library/functions.html#eval) functions have optional arguments to override the global and local namespace. If only one namespace is specified, it is used for both.



## 4.3. Exceptions



Exceptions are a means of breaking out of the normal flow of control of a code block in order to handle errors or other exceptional conditions. An exception is *raised* at the point where the error is detected; it may be *handled* by the surrounding code block or by any code block that directly or indirectly invoked the code block where the error occurred.

The Python interpreter raises an exception when it detects a run-time error (such as division by zero). A Python program can also explicitly raise an exception with the [`raise`](https://docs.python.org/3/reference/simple_stmts.html#raise) statement. Exception handlers are specified with the [`try`](https://docs.python.org/3/reference/compound_stmts.html#try) … [`except`](https://docs.python.org/3/reference/compound_stmts.html#except) statement. The [`finally`](https://docs.python.org/3/reference/compound_stmts.html#finally) clause of such a statement can be used to specify cleanup code which does not handle the exception, but is executed whether an exception occurred or not in the preceding code.

Python uses the **“termination” model** of error handling: an exception handler can find out what happened and continue execution at an outer level, but it cannot repair the cause of the error and retry the failing operation (except by re-entering the offending piece of code from the top).

When an exception is not handled at all, the interpreter **terminates** execution of the program, or returns to its interactive main loop. In either case, it prints a **stack backtrace**, except when the exception is [`SystemExit`](https://docs.python.org/3/library/exceptions.html#SystemExit).

***SUMMARY*** : 关于stack backtrace，在python的[standard type hierarchy](https://docs.python.org/3/reference/datamodel.html#the-standard-type-hierarchy)中为Internal types中的Traceback objects；

Exceptions are identified by class instances. The [`except`](https://docs.python.org/3/reference/compound_stmts.html#except) clause is selected depending on the class of the instance: it must reference the class of the instance or a base class thereof. The **instance** can be received by the handler and can carry additional information about the exceptional condition.

> Note :Exception messages are not part of the Python API. Their contents may change from one version of Python to the next without warning and should not be relied on by code which will run under multiple versions of the interpreter.

See also the description of the [`try`](https://docs.python.org/3/reference/compound_stmts.html#try) statement in section [The try statement](https://docs.python.org/3/reference/compound_stmts.html#try) and [`raise`](https://docs.python.org/3/reference/simple_stmts.html#raise) statement in section [The raise statement](https://docs.python.org/3/reference/simple_stmts.html#raise).

Footnotes

| [[1\]](https://docs.python.org/3/reference/executionmodel.html#id1) | This limitation occurs because the code that is executed by these operations is not available at the time the module is compiled. |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
|                                                              |                                                              |

