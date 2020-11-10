# Exception



## What is exception?

“exception”是software engineer领域非常常见的一个概念，在programming language、OS层都有exception的概念，我们可以看到，无论是在programming language层，还是在OS层，exception概念的基本含义是相同的，正如维基百科[Exception handling](https://en.wikipedia.org/wiki/Exception_handling)中所总结的：

> an exception breaks the normal flow of execution and executes a pre-registered *exception handler*



### oracle [oracle javase What Is an Exception?](https://docs.oracle.com/javase/tutorial/essential/exceptions/definition.html)

> The term *exception* is shorthand for the phrase "exceptional event."
>
> **Definition:** An *exception* is an event, which occurs during the execution of a program, that disrupts the normal flow of the program's instructions.



Exception是一种event，这个认知是非常重要的。

## Exception in programming language

大多数现代programming language(比如Java、C++、Python)都提供的了exception机制。

在阅读github [redis-plus-plus](https://github.com/sewenew/redis-plus-plus)的文档的时候，其中提及了这样的一段话：

> You don't need to check whether Redis object connects to server successfully. If Redis fails to create a connection to Redis server, or the connection is broken at some time, it throws an **exception** of type `Error` when you try to send command with Redis. Even when you get an exception, i.e. the connection is broken, you don't need to create a new Redis object. You can reuse the Redis object to send commands, and the Redis object will try to reconnect to server automatically. If it reconnects successfully, it sends command to server. Otherwise, it throws an exception again.
>
> See the [Exception section](https://github.com/sewenew/redis-plus-plus#exception) for details on exceptions.

显然，在programming language中，上述使用exception的模式是非常典型的，它符合 [Asking for permission instead of forgiveness](https://docs.quantifiedcode.com/python-anti-patterns/readability/asking_for_permission_instead_of_forgiveness_when_working_with_files.html) 的，显然这是一种programming思想，当然我们也可以采用另外的完全不同的思想；

EAFP风格

### wikipedia [Exception handling syntax](https://en.wikipedia.org/wiki/Exception_handling_syntax)

> NOTE: 对主流programming language的exception handling syntax进行了总结，比较好。

**Exception handling syntax** is the set of keywords and/or structures provided by a computer [programming language](https://en.wikipedia.org/wiki/Programming_language) to allow [exception handling](https://en.wikipedia.org/wiki/Exception_handling), which separates the handling of errors that arise during a program's operation from its **ordinary processes**.

> NOTE: 后面一段话说明了exception handling的优势所在。

Most commonly, error handling uses a `try...[catch...][finally...]` block.

#### [C++](https://infogalactic.com/info/C%2B%2B)

```C++
#include <exception>
int main() {
   try {
       // do something (might throw an exception)
   }
   catch (const std::exception& e) {
        // handle exception e
   }
   catch (...) {
        // catches all exceptions, not already caught by a catch block before
        // can be used to catch exception of unknown or irrelevant type
   }
}
```

In C++, a *[resource acquisition is initialization](https://infogalactic.com/info/Resource_Acquisition_Is_Initialization)* technique can be used to clean up resources in exceptional situations. C++ intentionally does not support `finally`.[[1\]](https://infogalactic.com/info/Exception_handling_syntax#cite_note-1) Note that the outer braces for the method are optional.

#### [Java](https://infogalactic.com/info/Java_(programming_language))

```
try {
   // Normal execution path
   throw new EmptyStackException();
} catch (ExampleException ee) {
   //  deal with the ExampleException
} finally {
   // This optional section is executed upon termination of any of the try or catch blocks above, 
   //  except when System.exit() is called in "try" or "catch" blocks;
}
```

#### [JavaScript](https://infogalactic.com/info/JavaScript)

```
try {
  // Statements in which exceptions might be thrown
  throw 'error';
} catch(error) {
  // Statements that execute in the event of an exception
} finally {
  // Statements that execute afterward either way
}

```



#### [Python](https://infogalactic.com/info/Python_(programming_language))

```C++
f = None
try:
   f = file("aFileName")
   f.write(could_make_error())
except IOError:
   print "Unable to open file"
except:   # catch all exceptions
   print "Unexpected error"
else:     # executed if no exceptions are raised
   print "File write completed successfully"
finally:  # clean-up actions, always executed
   if f:
      f.close()
```







## Exception in Linux OS

在工程[Linux-OS](https://dengking.github.io/Linux-OS/)的`Kernel\Book-Understanding-the-Linux-Kernel\Chapter-4-Interrupts-and-Exceptions\Chapter-4-Interrupts-and-Exceptions`中讨论OS层的exception概念。

工程[Linux-OS](https://dengking.github.io/Linux-OS/)的`Kernel\Book-Understanding-the-Linux-Kernel\Chapter-4-Interrupts-and-Exceptions\Chapter-4-Interrupts-and-Exceptions`中详细讨论了exception的具体分类、exception的实现机制等。







## wikipedia [Exception safety](https://en.wikipedia.org/wiki/Exception_safety)

> NOTE: 在c++语言中，exception safety的概念非常重要，并且c++语言对它进行了formal definition。这个话题放到了c++中进行了具体讨论。



