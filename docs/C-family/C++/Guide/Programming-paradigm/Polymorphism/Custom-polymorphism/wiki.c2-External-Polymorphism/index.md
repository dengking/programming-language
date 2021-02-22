# wiki.c2 [External Polymorphism](https://proxy.c2.com/cgi/fullSearch?search=ExternalPolymorphism)

Any mechanism for implementing **polymorphism** that is external to the definition of the object displaying polymorphic behavior. Switch statements based on a type code are probably the simplest of such mechanisms.

> NOTE: 
>
> 1、关于 "Switch statements based on a type code are probably the simplest of such mechanisms" 的最最典型的案例就是在 gieseanw [Stop reimplementing the virtual table and start using double dispatch](https://gieseanw.wordpress.com/2018/12/29/stop-reimplementing-the-virtual-table-and-start-using-double-dispatch/) 中给出的如下示例:
>
> ![img](https://gieseanw.files.wordpress.com/2018/12/doubledispatchheader.png?w=660)