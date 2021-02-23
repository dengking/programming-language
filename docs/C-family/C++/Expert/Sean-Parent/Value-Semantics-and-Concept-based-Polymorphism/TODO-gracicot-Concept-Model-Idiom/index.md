# gracicot [Concept-Model Idiom Part One: A new look at polymorphism](https://gracicot.github.io/conceptmodel/2017/09/13/concept-model-part1.html)

Ah, good old Oriented Object Programming. Have a problem? Just make an interface, and implement it! Simple as that heh? Well no. Polymorphism as done today with classic OOP is intrusive, forces polymorphic behavior even in place where it’s not really needed, almost always imply heap allocation, needs to carry the vtable pointer and the list goes on. Fortunatly, other pattern and idiom exists. Here’s an approach that might change how you see polymophism, and the problem it tries to solve. Let me introduce the Concept-Model idiom, also called runtime concept, or virtual concept.

> NOTE: 
>
> 1、"Polymorphism as done today with classic OOP is intrusive" 要如何理解？在 mropert [Polymorphic ducks](https://mropert.github.io/2017/11/30/polymorphic_ducks/) 中有一个回答。

I’m not the inventor of this idiom. I’m sure everyone doing OOP ended up doing something that look like it in some way (hint: Adapter pattern). The first time I saw the idea in a concrete way was in the Sean Parent’s talk [Inheritance Is The Base Class of Evil](https://channel9.msdn.com/Events/GoingNative/2013/Inheritance-Is-The-Base-Class-of-Evil). Thanks to him, I manage to use it and practice it a lot. I want to share what I’ve learned by playing around with this pattern.