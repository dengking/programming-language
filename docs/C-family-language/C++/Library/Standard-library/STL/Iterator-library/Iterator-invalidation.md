# Iterator invalidation

使用iterator需要格外关注的一件事情就是：iterator invalidation。

可以将iterator看做是pointer， 显然一个iterator所指向的memory如果被修改了，那么再次使用这个iterator去取这个memory的内容，那么就可能取到的是invalid的内容。从这个角度来出发，再结合各种container的实现细节，就可以方便的判断出是否发生了invalidation。

## cppreference [Containers library#Iterator invalidation](https://en.cppreference.com/w/cpp/container)



