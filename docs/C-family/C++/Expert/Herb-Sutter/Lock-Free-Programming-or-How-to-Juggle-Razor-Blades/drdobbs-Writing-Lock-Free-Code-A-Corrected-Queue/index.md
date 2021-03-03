# [Writing Lock-Free Code: A Corrected Queue](https://www.drdobbs.com/parallel/writing-lock-free-code-a-corrected-queue/210604448?pgno=1)

Herb continues his exploration of lock-free code--this time focusing on creating a lock-free queue.

As we saw last month [1], lock-free coding is hard even for experts. There, I dissected(剖析) a published lock-free queue implementation [2] and examined why the code was quite broken. This month, let's see how to do it right.