# foonathan [Guidelines For Rvalue References In APIs](https://foonathan.net/blog/2018/03/26/rvalue-references-api-guidelines.html)

I’ll be giving a talk at [ACCU](https://conference.accu.org/2018/sessions.html#XIjustwantedtopointtosomething) about when to use which pointer types and why.

While working on that I made some guidelines for rvalue references in interfaces which didn’t quite fit the talk, so I’m writing about them here.

When should you use **rvalue references** as function parameters?

When as return types?

What are ref-qualified member functions and when and how should you use them?

Let’s tackle it one by one.


