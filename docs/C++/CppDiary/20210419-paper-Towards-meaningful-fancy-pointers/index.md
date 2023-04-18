# open-std [Towards meaningful fancy pointers](http://open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0773r0.html)

> NOTE: 
>
> 在 [P1144R2 Object relocation in terms of move plus destroy](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html) 
>
> > But this merely encourages the programmer to add the warrant and continue. An incorrect warrant will be discovered only at runtime, via undefined behavior. (See [Allocated memory contains pointer to self](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#non-trivial-sample-list) and [[FollyIssue889\]](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#biblio-follyissue889).)
>
> 在 github [Traits.h marks std::list as trivially relocatable, but in fact it is not #889](https://github.com/facebook/folly/issues/889) # **[Quuxplusone](https://github.com/Quuxplusone)** commented [on 31 Jul 2018](https://github.com/facebook/folly/issues/889#issuecomment-409019772): 
>
> >  Contrast these two diagrams from [P0773R0](http://open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0773r0.html#B):
>
> 上述 [P0773R0](http://open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0773r0.html#B) 即链接到了本文

