# Programming language

我的工程中，与programming language相关的有：

1、工程[compiler-principle](https://dengking.github.io/compiler-principle/)

2、工程[automata-and-formal-language](https://dengking.github.io/automata-and-formal-language)

3、文章[Abstraction](https://dengking.github.io/Post/Abstraction/Abstraction/)的[Abstraction in programming language](https://dengking.github.io/Post/Abstraction/Abstraction/#abstraction-in-programming-language)小节



## wikipedia [Programming language](https://en.wikipedia.org/wiki/Programming_language)





## Abstraction in programming language history

> NOTE: 本节从programming language的发展史来思考abstraction。

纵观 [programming language的发展史](https://en.wikipedia.org/wiki/Programming_language#History)，我们发现：语言越来越高级，越来越抽象，越来越易用。最底层的语言：[machine language](https://en.wikipedia.org/wiki/Machine_code) 晦涩难懂，计算机科学的先驱在[machine language](https://en.wikipedia.org/wiki/Machine_code) 的基础上抽象出了 [function](https://en.wikipedia.org/wiki/Subroutine) 概念，又抽象出了 [class](https://en.wikipedia.org/wiki/Class-based_programming) 的概念，创建了非常多 [高级的语言](https://en.wikipedia.org/wiki/High-level_programming_language)，比如C。不管多么高级的语言最终都需要被翻译（compiler或interpreter）为[machine language](https://en.wikipedia.org/wiki/Machine_code)才能够被机器执行，显然这个翻译的过程是与前面的抽象的过程相逆的。

与machine language相比，我们发现高级语言往往伴随着非常多的概念，而且越高级的语言貌似概念越多（这是我的直觉，可能并非如此），当然也越容易使用。与此类似的是，各种library为了简化某个领域的问题， 也创造了各种直观易懂的概念，使得software engineer在解决这类问题时，非常轻松容易，因此获得了software engineer的喜爱。

> TODO: 底层的programming language，比如C，由于它们是面向hardware，无法描述抽象的概念，这就限制了language的express power

从这个发展史我们能够看到抽象所带来的巨大优势：

- 隐藏了底层的细节
- 越抽象，越易用，概念也就越多
- machine-independent，通用性更加，开发效率更高



参见: [Abstraction(computer science)#Rationale](https://en.wikipedia.org/wiki/Abstraction_(computer_science)#Rationale)

> A central form of abstraction in computing is language abstraction: new artificial languages are developed to express specific aspects of a system. *[Modeling languages](https://en.wikipedia.org/wiki/Modeling_languages)* help in planning. *[Computer languages](https://en.wikipedia.org/wiki/Computer_language)* can be processed with a computer. An example of this abstraction process is the generational development of [programming languages](https://en.wikipedia.org/wiki/Programming_language) from the [machine language](https://en.wikipedia.org/wiki/First-generation_programming_language) to the [assembly language](https://en.wikipedia.org/wiki/Second-generation_programming_language) and the [high-level language](https://en.wikipedia.org/wiki/Third-generation_programming_language). Each stage can be used as a stepping stone for the next stage. The language abstraction continues for example in [scripting languages](https://en.wikipedia.org/wiki/Scripting_language) and [domain-specific programming languages](https://en.wikipedia.org/wiki/Domain-specific_programming_language).

参见：龙书 [Chapter 7 Run-Time Environments](https://dengking.github.io/compiler-principle/Chapter-7-Run-Time-Environments/)。



### TODO

关于programming language的发展史，需要将thinking in Java的内容添加到其中。

### 优势

1) more expressive

programming language变得越来越expressive、表现力更强；

2) more powerful

programming language变得越来越powerful，在一定程度上降低了开发成本

