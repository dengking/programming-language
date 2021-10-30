# Token



## token

[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[token.h](https://github.com/chromium/chromium/blob/master/base/token.h)**

[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[token.cc](https://github.com/chromium/chromium/blob/master/base/token.cc)**

```C++
  // Constructs a new Token with random |high| and |low| values taken from a
  // cryptographically strong random source.
  static Token CreateRandom();
```



## sequence token



[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[sequence_token.h](https://github.com/chromium/chromium/blob/master/base/sequence_token.h)**

> A token that identifies a series of sequenced tasks (i.e. tasks that run one at a time in posting order).

## unguessable token

其实就是 "unforgeable不可伪造的"，它可以作为resource token、resource ID，比如在 [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/**[platform_shared_memory_region.h](https://github.com/chromium/chromium/blob/master/base/memory/platform_shared_memory_region.h)** 中就使用了它。

[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[unguessable_token.h](https://github.com/chromium/chromium/blob/master/base/unguessable_token.h)**

> An `UnguessableToken` is a strong *bearer token*. Bearer tokens are like HTTP cookies: if a caller has the token, the callee thereby considers the caller authorized to request the operation the callee performs.
>
> `UnguessableToken` can be used when the resource associated with the ID needs to be protected against manipulation by other untrusted agents in the system, and there is no other convenient way to verify the authority of the agent to do so (because the resource is part of a table shared across processes, for instance). In such a scheme, knowledge of the token value in and of itself is sufficient proof of authority to carry out an operation on the associated resource.

一、由于采用virtual address，因此address是不能够作为resource token、resource ID的

二、"*bearer token*"的含义是"不记名的令牌

三、capability-based security



```C++
static const UnguessableToken& Null();
```

这是典型的named constructor idiom。