# Polymorphic containers with value semantic



## stackoverflow [Can I have polymorphic containers with value semantics in C++?](https://stackoverflow.com/questions/41045/can-i-have-polymorphic-containers-with-value-semantics-in-c)



[A](https://stackoverflow.com/a/41059)

Since the objects of different classes will have different sizes, you would end up running into the slicing problem if you store them as values.

One reasonable solution is to store container safe smart pointers. I normally use `boost::shared_ptr` which is safe to store in a container. Note that std::auto_ptr is not.

```c++
vector<shared_ptr<Parent>> vec;
vec.push_back(shared_ptr<Parent>(new Child()));
```

`shared_ptr` uses reference counting so it will not delete the underlying instance until all references are removed.

[A](https://stackoverflow.com/a/71264)

I just wanted to point out that `vector<Foo`> is usually more efficient than `vector<Foo*>`. In a `vector<Foo>`, all the Foos will be adjacent to each other in memory. Assuming a cold TLB and cache, the first read will add the page to the TLB and pull a chunk of the vector into the L# caches; subsequent reads will use the warm cache and loaded TLB, with occasional cache misses and less frequent TLB faults.

Contrast this with a `vector<Foo*>`: As you fill the vector, you obtain `Foo*`'s from your memory allocator. Assuming your allocator is not extremely smart, (tcmalloc?) or you fill the vector slowly over time, the location of each `Foo` is likely to be far apart from the other Foos: maybe just by hundreds of bytes, maybe megabytes apart.

In the worst case, as you scan through a `vector<Foo*>` and dereferencing each pointer you will incur a TLB fault and cache miss -- this will end up being a *lot* slower than if you had a `vector<Foo>`. (Well, in the really worst case, each Foo has been paged out to disk, and every read incurs a disk `seek()` and `read()` to move the page back into RAM.)

So, keep on using `vector<Foo>` whenever appropriate. :-)

> NOTE: tag-value semantic VS reference semantic performance，上述是非常好的素材

## Sean Parent: Value-Semantics-and-Concept-based-Polymorphism

其中探讨了这个问题。



## Summary

总的来说，对于polymorphic object，只能够使用reference value semantic。Sean Parent的Concept-based-Polymorphism，只是在外层包了一层value semantic的壳子。