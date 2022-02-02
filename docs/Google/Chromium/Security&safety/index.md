# Security and safety

主要在探讨memory safety。

## chromium docs [Memory safety](https://www.chromium.org/Home/chromium-security/memory-safety)

The Chromium project finds that around 70% of our serious security bugs are [memory safety problems](https://alexgaynor.net/2019/aug/12/introduction-to-memory-unsafety-for-vps-of-engineering/). Our next major project is to prevent such bugs at source.

### The problem

Around 70% of our high severity security bugs are memory unsafety problems (that is, mistakes with C/C++ pointers). Half of *those* are use-after-free bugs.

> NOTE: 
>
> 通过下面的文章可知，目前解决use-after-free的方法是: `raw_ptr` 
>
> 1、chromium docs [MiraclePtr aka raw_ptr aka BackupRefPtr](https://chromium.googlesource.com/chromium/src/+/ddc017f9569973a731a574be4199d8400616f5a5/base/memory/raw_ptr.md)
>
> 

[![Pie chart of uses-after-free, other memory safety, other security bug, security asserts](https://www.chromium.org/_/rsrc/1589581287612/Home/chromium-security/memory-safety/piechart.png)](https://www.chromium.org/Home/chromium-security/memory-safety/piechart.png?attredirects=0)

## security.googleblog [An update on Memory Safety in Chrome](https://security.googleblog.com/2021/09/an-update-on-memory-safety-in-chrome.html)

> NOTE: 
>
> 一、zdnet [Chrome willing to take performance hit to prevent use-after-free bugs](https://www.zdnet.com/article/chrome-willing-to-take-performance-hit-to-prevent-use-after-free-bugs/) 是对这篇文章的解读
>
> 二、这篇文章还是值得一读的，其实作为C++ programmer，我们会面对同样的问题，也会有同样的思路:
>
> 1、static compile-time check
>
> 2、dynamic run-time check
>
> 3、使用一门新的语言
>
> 这篇文章，其实就是对这三种思路的探索

Chrome has been exploring three broad avenues to seize this opportunity:

1、Make C++ safer through compile-time checks that pointers are correct.

> NOTE: 
>
> 这其实就是"static memory safety"，rust就提供了这总能力

2、Make C++ safer through runtime checks that pointers are correct.

3、Investigating use of a memory safe language for parts of our codebase.



Runtime checks have a performance cost. Checking the correctness of a pointer is an infinitesimal cost in memory and CPU time. But with millions of pointers, [it adds up](https://www.youtube.com/watch?v=rHIkrotSwcc). And since Chrome performance is important to billions of users, many of whom are using low-power mobile devices without much memory, an increase in these checks would result in a slower web.

Ideally we’d choose option 1 - make C++ safer, at compile time. Unfortunately, the language just isn’t designed that way. You can learn more about the investigation we've done in this area in [Borrowing Trouble: The Difficulties Of A C++ Borrow-Checker](https://docs.google.com/document/d/e/2PACX-1vSt2VB1zQAJ6JDMaIA9PlmEgBxz2K5Tx6w2JqJNeYCy0gU4aoubdTxlENSKNSrQ2TXqPWcuwtXe6PlO/pub) that we're also publishing today.

So, we’re mostly left with options 2 and 3 - [make C++ safer](https://docs.google.com/document/d/e/2PACX-1vRZr-HJcYmf2Y76DhewaiJOhRNpjGHCxliAQTBhFxzv1QTae9o8mhBmDl32CRIuaWZLt5kVeH9e9jXv/pub) (but slower!) or start to use a different language. Chrome Security is experimenting with both of these approaches.

You’ll see major investments in C++ safety solutions - such as [MiraclePtr](https://chromium.googlesource.com/chromium/src/+/ddc017f9569973a731a574be4199d8400616f5a5/base/memory/raw_ptr.md) and [ABSL/STL hardened modes](https://crbug.com/1072380). In each case, we hope to eliminate a sizable fraction of our exploitable security bugs, but we also expect some performance penalty. For example, MiraclePtr prevents use-after-free bugs by quarantining memory that may still be referenced. On many mobile devices, memory is very precious and it’s hard to spare some for a quarantine. Nevertheless, MiraclePtr stands a chance of eliminating over 50% of the use-after-free bugs in the browser process - an *enormous* win for Chrome security, right now.

In parallel, we’ll be exploring whether we can use a memory safe language for parts of Chrome in the future. The leading contender is [Rust](https://www.rust-lang.org/), invented by our friends at Mozilla. This is (largely) compile-time safe; that is, the Rust compiler spots mistakes with pointers before the code even gets to your device, and thus there’s no performance penalty. Yet there are [open questions about whether we can make C++ and Rust work well enough together](https://www.chromium.org/Home/chromium-security/memory-safety/rust-and-c-interoperability). Even if we started writing new large components in Rust tomorrow, we’d be unlikely to eliminate a significant proportion of security vulnerabilities for many years. And can we make the language boundary clean enough that we can write parts of existing components in Rust? We don’t know yet. We’ve started to land limited, non-user-facing Rust experiments in the [Chromium source code tree](https://source.chromium.org/chromium/chromium/src/+/main:build/config/rust.gni), but we’re not yet using it in production versions of Chrome - we remain in an experimental phase.

That’s why we’re pursuing both strategies in parallel. Watch this space for updates on our adventures in making C++ safer, and efforts to experiment with a new language in Chrome.



## google docs [Safer Usage Of C++](https://docs.google.com/document/d/e/2PACX-1vRZr-HJcYmf2Y76DhewaiJOhRNpjGHCxliAQTBhFxzv1QTae9o8mhBmDl32CRIuaWZLt5kVeH9e9jXv/pub#h.eoikp3r0cwlf)

> NOTE: 
>
> 非常值得一读的





## google docs [Borrowing Trouble: The Difficulties Of A C++ Borrow-Checker](https://docs.google.com/document/d/e/2PACX-1vSt2VB1zQAJ6JDMaIA9PlmEgBxz2K5Tx6w2JqJNeYCy0gU4aoubdTxlENSKNSrQ2TXqPWcuwtXe6PlO/pub)

> NOTE: 
>
> 不同于C++，rust语言将resource ownership建立在语言层面了，并且它基于此实现了static memory safety，关于此，可以参见:
>
> 1、rust doc [References and Borrowing](https://doc.rust-lang.org/1.8.0/book/references-and-borrowing.html)