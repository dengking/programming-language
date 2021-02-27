

# isocpp.faq: Inheritance — Multiple and Virtual Inheritance [¶](https://isocpp.org/wiki/faq/multiple-inheritance) [Δ](https://isocpp.org/wiki/faq/multiple-inheritance#)

> NOTE: 这篇文章讲解地还不错

## Do we really need multiple inheritance? [¶](https://isocpp.org/wiki/faq/multiple-inheritance#mi-needed) [Δ](https://isocpp.org/wiki/faq/multiple-inheritance#)

In C++, abstract classes often serve as interfaces and a class can have many interfaces. Other languages – often deemed “not MI” – simply have a separate name for their equivalent to a pure abstract class: an interface. 

> NOTE: 
>
> 1、可以使用multiple inheritance的场景: "implement multiple interface"

The reason languages provide inheritance (both single and multiple) is that language-supported inheritance is typically superior to workarounds (e.g. use of forwarding functions to sub-objects or separately allocated objects) for ease of programming, for detecting logical problems, for maintainability, and often for performance.

> NOTE: 这段话所要表明的是: 虽然可以使用walkround来实现和multiple inheritance类似的效果，但是如果programming language提供了multiple inheritance，那么直接使用这个特性能够带来诸多优势。

## I’ve been told that I should never use multiple inheritance. Is that right? [¶](https://isocpp.org/wiki/faq/multiple-inheritance#mi-not-evil) [Δ](https://isocpp.org/wiki/faq/multiple-inheritance#)

It *really* bothers me when people think they know what’s best for *your* problem even though they’ve never seen *your* problem!! How can anybody possibly know that multiple inheritance won’t help *you* accomplish *your* goals without knowing *your* goals?!?!?!?!!!

> NOTE: 表明意思是: 当人们认为他们知道如何解决你的问题时，即使他们从未见过你的问题，这真的让我很困扰!!
>
> 其实说白了就是：千万不要迷信，要具体问题具体讨论。

 “One size does not fit all.”

## So there are times when multiple inheritance isn’t bad?!?? [¶](https://isocpp.org/wiki/faq/multiple-inheritance#mi-not-evil-2) [Δ](https://isocpp.org/wiki/faq/multiple-inheritance#)

If MI (multiple inheritance) helps, use it; if not, don’t. 

## What are some disciplines for using multiple inheritance? [¶](https://isocpp.org/wiki/faq/multiple-inheritance#mi-disciplines) [Δ](https://isocpp.org/wiki/faq/multiple-inheritance#)

