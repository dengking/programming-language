看了《深入理解include预编译原理.md》中对static的解释，它指出来static的作用：
用static修饰函数，则表明该函数只能在本文件中使用，因此，当不同的文件中有相同的函数名被static修饰时，不会产生重复定义的报错。

显然，使用static能够避免在链接阶段可能出现的重复定义的错误，但是我还想进一步地进行深究：为什么使用static就能够避免这个问题？问题的答案肯定在于编译器对于static修饰的函数后变量会进行特殊的处理，那么编译器会进行什么样的特殊处理才能够避免这个问题呢？

# 20180321
原文链接：https://www.cnblogs.com/zplutor/archive/2011/08/06/2129401.html

按照编译器的设计，include编译命令会使编译器将指定的头文件拷贝到本文件中。在一个头文件中定义一个static变量，意味着该变量的**作用域**只限于定义它的源文件中，其它源文件不能访问。既然这种定义方式出现在头文件中，那么可以很自然地推测：包含了该头文件的所有**源文件**中都定义了这些变量，即该头文件被包含了多少次，这些变量就定义了多少次。

看到有一位同学在头文件中这么写：
```
static const wchar_t* g_str1 = …
static const wchar_t* g_str2 = …
```
假如将上面两行代码的static去掉，编译的时候就会出现变量重定义的错误，这进一步证实了上面的推测，因为没有static的话变量的作用域是**全局**的，定义了两个以上的同名变量就会出现该错误。

推测终究是推测，要真正证实这个推测还要通过写代码来验证。验证的方式是：在头文件中使用static定义变量，在多个源文件中包含该头文件，然后在每个源文件中输出变量的地址，同时在一个源文件中改变变量的值并输出，在另一个源文件中也输出。如果每个源文件的输出都不同，则推测得证；否则推测是错误的。

源代码详见e1。

运行编译出来的程序，可以看到，虽然在代码中好像使用了**相同的变量**，但是实际上使用的是**不同的变量**，在每个源文件中都有**单独的变量**。所以，在头文件中定义static变量会造成变量**多次定义**，造成内存空间的浪费，而且也不是真正的全局变量。应该避免使用这种定义方式。

作为对比，下面使用正确的方式来定义全局变量：

源代码详见e2.

可以看到，这次两个源文件中使用的都是同一个变量。要注意的是，使用extern声明变量时不能带有初始值，否则仍然属于变量定义，会出现变量重定义的错误。
