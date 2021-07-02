# Bound 



## IO bound VS VS CPU bound VS data bound

|            |                   |            |
| ---------- | ----------------- | ---------- |
| IO bound   | IO intensive      | IO密集型   |
| CPU bound  | compute intensive | 计算密集型 |
| Data bound | data intensive    | 数据密集型 |

1、参考: `Book-Designing-Data-Intensive-Applications`

2、data bound 和 memory bound有一点关联

## wikipedia [CPU-bound](https://en.wikipedia.org/wiki/CPU-bound)



## wikipedia [I/O bound](https://en.wikipedia.org/wiki/I/O_bound)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **I/O bound** refers to a **condition** in which the time it takes to complete a [computation](https://en.wikipedia.org/wiki/Computation) is determined principally by the period spent waiting for [input/output](https://en.wikipedia.org/wiki/Input/output) operations to be completed. This is the opposite of a task being [CPU bound](https://en.wikipedia.org/wiki/CPU_bound). This circumstance arises when the rate at which [data](https://en.wikipedia.org/wiki/Data) is requested is slower than the rate it is consumed or, in other words, more time is spent requesting data than processing it.[[1\]](https://en.wikipedia.org/wiki/I/O_bound#cite_note-1)



### I/O bound as an inherent problem in computing

> NOTE: I/O瓶颈是当代computing的一个固有问题，这一段，作者从 [Von Neumann architecture](https://en.wikipedia.org/wiki/Von_Neumann_architecture) 的角度来进行思考，从IO bound追溯到了 [Von Neumann bottleneck](https://en.wikipedia.org/wiki/Von_Neumann_bottleneck) 。这提示了我们，结构决定了...。

The **I/O bound state** has been identified as a problem in computing almost since its inception. The [Von Neumann architecture](https://en.wikipedia.org/wiki/Von_Neumann_architecture), which is employed by many computing devices, this involves multiple possible solutions such as implementing a logically separate [central processor unit](https://en.wikipedia.org/wiki/Central_processor_unit) which along with storing the instructions of the program also retrieves actual data usually from [main memory](https://en.wikipedia.org/wiki/Main_memory) and makes use of this much easily accessible data for working. When the process is terminated it writes back the results to the original storage (usually the [main memory](https://en.wikipedia.org/wiki/Main_memory)).

Since data must be moved between the **CPU** and **memory** along a [bus](https://en.wikipedia.org/wiki/Bus_(computing)) which has a limited [data transfer rate](https://en.wikipedia.org/wiki/Bit_rate), there exists a condition that is known as the [Von Neumann bottleneck](https://en.wikipedia.org/wiki/Von_Neumann_bottleneck). Put simply, this means that the data [bandwidth](https://en.wikipedia.org/wiki/Bandwidth_(computing)) between the CPU and memory tends to limit the overall speed of computation. In terms of the actual technology that makes up a computer, the **Von Neumann Bottleneck** predicts that it is easier to make the CPU perform calculations faster than it is to supply it with data at the necessary rate for this to be possible.

In recent history, the **Von Neumann bottleneck** has become more apparent. The design **philosophy** of modern computers is based upon a physically separate CPU and main memory. It is possible to make the CPU run at a high data transfer rate because data is moved between locations inside them across tiny distances. The physical separation between CPU and main memory, however, requires a data bus to move data across comparatively long distances of centimetres or more. The problem of making this part of the system operate sufficiently fast to keep up with the CPU has been a great challenge to designers.[[2\]](https://en.wikipedia.org/wiki/I/O_bound#cite_note-2)

> NOTE: 这个问题变得越来越明显了



## CPU bound vs IO bound

## yaoyao [CPU bound vs IO bound](http://yaoyao.codes/os/2017/03/20/cpu-bound-vs-io-bound)

From [stackoverflow: What do the terms “CPU bound” and “I/O bound” mean?](http://stackoverflow.com/questions/868568/what-do-the-terms-cpu-bound-and-i-o-bound-mean):

1) **CPU Bound**

means the rate at which process progresses is limited by the speed of the **CPU**.

- A task that performs calculations on a small set of numbers, for example multiplying small matrices, is likely to be CPU bound.
- A program is CPU bound if it would go faster if the CPU were faster.

2) **I/O Bound**

means the rate at which a process progresses is limited by the speed of the I/O subsystem.

- A task that processes data from disk, for example, counting the number of lines in a file is likely to be I/O bound.
- A program is I/O bound if it would go faster if the **I/O subsystem** was faster.



## stackoverflow [What do the terms “CPU bound” and “I/O bound” mean?](https://stackoverflow.com/questions/868568/what-do-the-terms-cpu-bound-and-i-o-bound-mean)

What do the terms "CPU bound" and "I/O bound" mean?



### [A](https://stackoverflow.com/a/868577)

It's pretty intuitive:

A program is CPU bound if it would go faster if the CPU were faster, i.e. it spends the majority of its time simply using the CPU (doing calculations). A program that computes new digits of π will typically be CPU-bound, it's just crunching numbers.

A program is I/O bound if it would go faster if the **I/O subsystem** was faster. Which exact **I/O system** is meant can vary; I typically associate it with **disk**, but of course **networking** or communication in general is common too. A program that looks through a huge file for some data might become I/O bound, since the bottleneck is then the reading of the data from disk (actually, this example is perhaps kind of old-fashioned these days with hundreds of MB/s coming in from SSDs).

> NOTE: file和network都属于IO



## wikipedia [Category:Computer performance](https://en.wikipedia.org/wiki/Category:Computer_performance)