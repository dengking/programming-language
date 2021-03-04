# Temporary materialization



## What is materialization？

在描述"temporary materialization"之前，我们首先搞清楚"materialization"的含义，它的中文意思是: "物化"，下面是维基百科总结的它在computer science中的一些用法:

TODO: 需要添加维基百科的词条。



在《Book-Designing-Data-Intensive-Applications》中，也有materialization词条:

`Part III. Derived Data#10. Batch Processing#Materialization of Intermediate State`:

The process of writing out this intermediate state to files is called ***materialization***. (We came across the term previously in the context of materialized views, in “Aggregation: Data Cubes and Materialized Views” on page 101. It means to eagerly compute the result of some operation and write it out, rather than computing it on demand when requested.)

By contrast, the log analysis example at the beginning of the chapter used Unix pipes to connect the output of one command with the input of another. Pipes do not fully **materialize** the **intermediate state**, but instead stream the output to the input incrementally, using only a small in-memory buffer.



## cppreference [Implicit conversions#Temporary materialization](https://en.cppreference.com/w/cpp/language/implicit_conversion#Temporary_materialization)

