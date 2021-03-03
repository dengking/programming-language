# 关于本章

在阅读 infoq [Lock-free Programming in C++ with Herb Sutter](https://www.infoq.com/news/2014/10/cpp-lock-free-programming/) 时，其中给出的lock free queue的例子，于是我想找source code: Google 

Herb Sutter lock free queue

找到了如下内容，都是出自Herb Sutter:

1、drdobbs [Writing Lock-Free Code: A Corrected Queue](https://www.drdobbs.com/parallel/writing-lock-free-code-a-corrected-queue/210604448?pgno=1)

2、PPT [Lock-Free Programming or, How to Juggle Razor Blades](http://www.alfasoft.com/files/herb/40-LockFree.pdf)

粗略的浏览了一下，发现上述三篇文章的内容是存在一定重叠的，其中[Lock-Free Programming or, How to Juggle Razor Blades](http://www.alfasoft.com/files/herb/40-LockFree.pdf)是Herb Sutter的一个PPT ，包含了另外两篇的内容，因此本章以它为标题，另外两篇文章可以作为对它的补充、说明。