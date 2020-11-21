non-type template parameter的computation，重要以template parameter pack的computation、recursion为基础来进行总结。





#### Template metaprogramming and functional programming and non-type template parameter

上面这段话对于理解template metaprogramming是非常重要的：

template metaprogramming can be seen as a form of functional programming

在后面，我们会学习到template parameter和template argument，如果将template metaprogramming看做是一种functional programming的话，这个概念是非常容易理解的。

上面这段话中中特别提及了recursion，这就是典型的functional programming。

需要注意的是，一般使用non-type template parameter来实现compile-time execution，它可以看做是一种典型的functional programming，而其他的template parameter，如type template parameter、template template parameter，其实就不属于functional programming的概念了。

下面的`factorial`就是一个典型的例子；`factorial`的实现是典型的functional programming，使用`c++`的template metaprogramming可以实现compiler-time的functional programming。





## template recursion function