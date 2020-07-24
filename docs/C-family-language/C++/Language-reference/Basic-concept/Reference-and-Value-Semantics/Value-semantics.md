## akrzemi1 [Value semantics](https://akrzemi1.wordpress.com/2012/02/03/value-semantics/)



### What is a value? What is an object?





### Why we use value semantics



### Why we do not use value semantics





### Performance of value semantics





### Comments



Hi Gunnar, it looks we are getting somewhere. I am trying to identify at which point we really disagree (because we seem to agree on many things). It looks like you claim that “value semantics is completely orthogonal to polymorphism” and I claim that there is a tension between value semantics and run-time polymorphism (based on virtual table look-up). Agreed?

Now, let me go through your post:

> Polymorphism comes from the fact that each object has a hidden piece of data which points to the Virtual Table that is created for each class. This is the key to polymorphic behavior.

I agree so far, although “this is the key” seems a bit imprecise: having a virtual table pointer is a way of implementing run-time polymorphism but it is not a polymorphism per se. C++ does not require that compilers provide virtual tables if they can implement the same polymorphic behaviour by other means. (but in practise it is always virtual tables, I guess).

> It’s there regardless of whether you access the object with value semantics, reference semantics or pointer semantics.

If by “it” you mean “a hidden piece of data which points to the Virtual Table” than, this I agree that we would find “it” in a newly created object, or in an object referred to by a named reference, or in an object pointed to by a pointer.

> That’s why I say that value semantics is completely orthogonal to polymorphism and therefore, OO.

This is where I disagree with you. Only having “a hidden piece of data which points to the Virtual Table” does not mean using polymorphism. In fact, you have shown a good example the last time how one can create a polymorphic type and then choose not to use this polymorphism. Of course the example was very scarce, so I can only guess how you defined your types (sorry if I guessed wrong) but here is how I imagine you define your classes: