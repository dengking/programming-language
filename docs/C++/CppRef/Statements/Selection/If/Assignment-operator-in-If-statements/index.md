# Assignment operator in If statements

一、其实这种用法和  `for(int i=0;i<count;++i)`  中定义`i`是一样的，这是我在阅读 stackoverflow [Variable assignment in an "if" condition](https://stackoverflow.com/questions/17681535/variable-assignment-in-an-if-condition) # [A](https://stackoverflow.com/a/17681782/10173843) 中的如下comment发现的:

> Putting the definition in the condition tightens the scope so that you don't accidentally refer to `derived` on the next line after the if-statement. It's the same reason we use `for (int i = 0...` instead of `int i; for (i = 0; ...`. 
>
> – [Adrian McCarthy](https://stackoverflow.com/users/1386054/adrian-mccarthy)  [Jul 16, 2013 at 16:36](https://stackoverflow.com/questions/17681535/variable-assignment-in-an-if-condition#comment25760207_17681782)

二、它的scope还包括else分支，这是在 stackoverflow [Variable assignment in an "if" condition](https://stackoverflow.com/questions/17681535/variable-assignment-in-an-if-condition) # [A](https://stackoverflow.com/a/17681782/10173843) 的如下comment中提及的:

> Funny thing about the scope of the variable when putting a definition in the condition is that the variable is also accessible in the `else`-clause as well. `if (T t = foo()) { dosomething(t); } else { dosomethingelse(t); }` – [dalle](https://stackoverflow.com/users/19100/dalle)  [Jul 18, 2013 at 8:57](https://stackoverflow.com/questions/17681535/variable-assignment-in-an-if-condition#comment25825749_17681782)

完整测试程序如下:

```C++
int foo()
{
    return 0;
}
void dosomething(int t) {}
void dosomethingelse(int t) {}
int main()
{
    if (int t = foo())
    {
        dosomething(t);
    }
    else
    {
        dosomethingelse(t);
    }
}

```

三、遵循 "minimize-scope-of-variable-principle" 

四、quora [**Why do C and C++ allow assignment in conditionals when doing this is pure failure?**](https://www.quora.com/Why-do-C-and-C++-allow-assignment-in-conditionals-when-doing-this-is-pure-failure)

A

To read a file it’s a little less clunky(笨重的) to do this

```C++
while ( (int c = file.get()) != EOF) { 
	//stuff 
} 
```

then something like:

```C++
int c = file.get(); 
while (c != EOF) { 
	//stuff 
	c=file.get(); 
} 
```

So you don’t have to repeat yourself, nor does the type definition of ‘`c`’ escape the loop. Note that this is also allowed in Java, although they force you to put the assignment in brackets (as I did).

This is pretty idiomatic for reading file-based data. Disallowing assignment in conditions would lead to awkward code for this common pattern.



五、stackoverflow [Variable assignment in an "if" condition](https://stackoverflow.com/questions/17681535/variable-assignment-in-an-if-condition) 



[A](https://stackoverflow.com/a/17681782/10173843)

```cpp
if (Derived* derived = dynamic_cast<Derived*>(base)) {
   // do stuff with `derived`
}
```

Though this is oft cited as an anti-pattern ("use virtual dispatch!"), sometimes the `Derived` type has functionality that the `Base` simply does not (and, consequently, distinct functions), and this is a good way to switch on that semantic difference.

comments: 

Putting the definition in the condition tightens the scope so that you don't accidentally refer to `derived` on the next line after the if-statement. It's the same reason we use `for (int i = 0...` instead of `int i; for (i = 0; ...`. – [Adrian McCarthy](https://stackoverflow.com/users/1386054/adrian-mccarthy)  [Jul 16, 2013 at 16:36](https://stackoverflow.com/questions/17681535/variable-assignment-in-an-if-condition#comment25760207_17681782)

@JamesKanze - Others find it increases readability and maintainability, partly by minimizing the scope of the variable. "To avoid accidental misuse of a variable, it is usually a good idea to introduce the variable into the smallest scope possible. In particular, it is usually best to delay the definition of a variable until one can give it an initial value ... One of the most elegant applications of these two principles is to declare a variable in a conditional." -- Stroustrup, "The C++ Programming Language." – [Andy Thomas](https://stackoverflow.com/users/202009/andy-thomas)  [Jul 16, 2013 at 16:40](https://stackoverflow.com/questions/17681535/variable-assignment-in-an-if-condition#comment25760354_17681782)

> NOTE:
>
> 一、Stroustrup的原话中的 "declare a variable in a conditional" 其实就是上述用法

Funny thing about the scope of the variable when putting a definition in the condition is that the variable is also accessible in the `else`-clause as well. `if (T t = foo()) { dosomething(t); } else { dosomethingelse(t); }` – [dalle](https://stackoverflow.com/users/19100/dalle)  [Jul 18, 2013 at 8:57](https://stackoverflow.com/questions/17681535/variable-assignment-in-an-if-condition#comment25825749_17681782)

> NOTE:
>
> 一、在上面给出了example code

Fixed in C++17 `if (Derived* derived = dynamic_cast<Derived*>(base); derived != nullptr) {` - it's now explicitly supported to do an assignment and a conditional in a single step. The difference is that the compiler knows that if you've done it in this form, you explicitly mean to. Where as it can't know `if (a = b)` is an error or something the user meant. – [gremwell](https://stackoverflow.com/users/1284610/gremwell)  [Sep 28, 2017 at 5:51](https://stackoverflow.com/questions/17681535/variable-assignment-in-an-if-condition#comment79878387_17681782)

> NOTE:
>
> 一、这就是"If Statements with Initializer (since C++17)"



[A](https://stackoverflow.com/a/18450261/10173843)

