# medium [How Does Python Handle Multiple Web Requests ?](https://medium.com/@side_swail/how-does-python-handle-multiple-web-requests-29787925775)

Take the case of a python web application running on a server. It gets a request from a client, processes it and returns a response, no issues so far. Next, the server receives multiple requests at the same time. The server can execute only one request at a time. How will the server handle **multiple** requests?

## Use Multiple Servers

One way you can handle multiple requests is to have multiple physical servers. Each request can be given to a server that is free and it can service it. This approach is highly inefficient as we are adding more servers without effectively using resources of the existing servers. A lot of CPU and memory is left under utilised. This approach is taken only when the existing server’s resources are utilised to the max.

## Use Multiple Threads

You can create a new thread to handle each request. Threads are lightweight units that executes parts of code and have the advantage of having shared resources amongst themselves. However, there is a problem with using threads in Python.

In CPython, there is a concept of Global Interpreter Lock which gives exclusive access to data. The GIL is implemented because the memory management of CPython in not thread safe. If two threads access the same data, the GIL will add a lock on the data for one thread and the other thread has to wait for the lock to be released. Two threads calling the same function will be slower than a single thread calling the same function twice because of threaded signalling overhead. Read more about it [here](http://www.dabeaz.com/python/GIL.pdf).

Because of this limitation, Python web applications cannot make use of threads to improve concurrency effectively. Note that this issue is not applicable to other flavours of Python like Jython and IronPython.

However, there are many applications using Python in their backend and are able to scale their servers to handle millions of requests. So how is this possible?

To understand this I looked into Gunicorn, a popular application server. Gunicorn is a WSGI compatible server that executes python applications. In Gunicorn docs, it is mentioned that it uses a pre-fork worker model. What does that mean?



## Use Multiple Processes

In a worker model architecture, a **master process** spawns **worker processes**(fork) and each **worker process** is executed in parallel. Each worker process will have its own instance of python so there is no problem of the GIL coming into play. When one process is waiting for some I/O operation to happen, another process can make use of the CPU to service a request. When lots of requests are received, the master process spawns more worker processes but starting processes can be a slow operation and starting more and more processes can be inefficient.

In order to avoid this, the pre-fork worker model has a master process that starts a fixed number of processes beforehand (pre-fork) and whenever a connection comes from a client, it hands it over to one of the workers from the pool.

The drawback here is that, you only have a handful of processes and if more requests come than the available processes, they will have to be queued.

## Conclusion

These are all synchronous ways to handle requests. There are optimisations that can be done to these approaches as well as use asynchronous ways to handle requests using **non-blocking servers** and these topics require a separate blog post to explain properly.

