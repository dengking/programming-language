# PImpl

非常重要的一种idiom。



## stackoverflow [Is the pImpl idiom really used in practice?](https://stackoverflow.com/questions/8972588/is-the-pimpl-idiom-really-used-in-practice)



[A](https://stackoverflow.com/a/8972757)

> So, I am wondering it this technique is really used in practice? Should I use it everywhere, or with caution?

Of course it is used. I use it in my project, in almost every class.

------

### Reasons for using the PIMPL idiom:

#### Binary compatibility

When you're developing a library, you can add/modify fields to `XImpl` without breaking the binary compatibility with your client (which would mean crashes!). Since the binary layout of `X` class doesn't change when you add new fields to `Ximpl` class, it is safe to add new functionality to the library in minor versions updates.

Of course, you can also add new public/private non-virtual methods to `X`/`XImpl` without breaking the binary compatibility, but that's on par with the standard header/implementation technique.

#### Data hiding

If you're developing a library, especially a proprietary one, it might be desirable not to disclose what other libraries / implementation techniques were used to implement the public interface of your library. Either because of Intellectual Property issues, or because you believe that users might be tempted to take dangerous assumptions about the implementation or just break the encapsulation by using terrible casting tricks. PIMPL solves/mitigates that.

#### Compilation time

Compilation time is decreased, since only the source (implementation) file of `X` needs to be rebuilt when you add/remove fields and/or methods to the `XImpl` class (which maps to adding private fields/methods in the standard technique). In practice, it's a common operation.

With the standard header/implementation technique (without PIMPL), when you add a new field to `X`, every client that ever allocates `X` (either on stack, or on heap) needs to be recompiled, because it must adjust the size of the allocation. Well, every client that doesn't ever allocate X *also* need to be recompiled, but it's just overhead (the resulting code on the client side will be the same).

What is more, with the standard header/implementation separation `XClient1.cpp` needs to be recompiled even when a private method `X::foo()` was added to `X` and `X.h` changed, even though `XClient1.cpp` can't possibly call this method for encapsulation reasons! Like above, it's pure overhead and is related with how real-life C++ build systems work.

Of course, recompilation is not needed when you just modify the implementation of the methods (because you don't touch the header), but that's on par with the standard header/implementation technique.

------

> Is this technique recommended to be used in embedded systems (where the performance is very important)?

That depends on how powerful your target is. However the only answer to this question is: measure and evaluate what you gain and lose. Also, take into consideration that if you're not publishing a library meant to be used in embedded systems by your clients, only the compilation time advantage applies!