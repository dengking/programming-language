object的linkage

function的linkage

首先需要搞清楚的是，linkage涉及的是translation unit



definition和declaration

definition是ORD，declaration是可以多个的。



对这些问题的理解，需要综合所有，需要结合compile的整个过程来进行分析。



`const`、`static`都是internal linkage

`extern` variable的storage duration是static的，这是因为显然它需要compiler在

