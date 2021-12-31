# [Chapter 2: Data Layout](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#layout)



## [2.5 Virtual Table Layout](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable)

### [2.5.1 General](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable-general)

A *virtual table* (*vtable*) is a table of information used 

1、to dispatch virtual functions, 

2、to access virtual base class subobjects, and 

3、to access information for runtime type identification (RTTI). 

> NOTE: 
>
> 显然，virtual table比我们平时所认知的，包含的内容是更多的

Each class that has **virtual member functions** or **virtual bases** has an associated set of virtual tables. There may be multiple virtual tables for a particular class, if it is used as a base class for other classes. However, the virtual table pointers within all the objects (instances) of a particular **most-derived class** point to the same set of virtual tables.

> NOTE: 
>
> 上面这段话要如何理解？

A virtual table consists of a sequence of offsets, data pointers, and function pointers, as well as structures composed of such items. We will describe below the sequence of such items. Their offsets within the virtual table are determined by that allocation sequence and the natural ABI size and alignment, just as a data struct would be. In particular:

1、Offsets are of type `ptrdiff_t` unless otherwise stated.

2、Data pointers have normal pointer size and alignment.

3、On Itanium, function pointers are pairs: the function address followed by the global pointer value that should be used when calling the function, aligned as for a pointer. On other platforms, function pointers are represented as they would be in any other context.

In general, what we consider the address of a virtual table (i.e. the address contained in objects pointing to a virtual table) may not be the beginning of the virtual table. We call it the *address point* of the virtual table. The virtual table may therefore contain components at either positive or negative offsets from its address point.

### [2.5.2 Virtual Table Components and Order](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable-components)

This section describes the usage and relative order of various components that may appear in virtual tables. Precisely which components are present in various possible virtual tables is specified in the next section. If present, components are present in the order described, except for the exceptions specified.

#### *Virtual call (vcall) offsets*

*Virtual call (vcall) offsets* are used to perform **pointer adjustment for virtual functions** that are declared in a **virtual base clas**s or its **subobjects** and overridden in a class derived from it. These entries are allocated in the virtual table for the virtual base class that is most immediately derived from the base class containing the overridden virtual function declaration. **They are used to find the necessary adjustment from the virtual base to the derived class containing the overrider, if any.** When a virtual function is invoked via a virtual base, but has been overridden in a derived class, the overriding function first adds a fixed offset to adjust the `this` pointer to the virtual base, and then adds the value contained at the `vcall` offset in the virtual base to its `this` pointer to get the address of the derived object where the function was overridden. These values may be positive or negative. These are first in the virtual table if present, ordered as specified in categories 3 and 4 of [Section 2.5.3](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable-construction) below.

> NOTE: 
>
> 一、"**They are used to find the necessary adjustment from the virtual base to the derived class containing the overrider, if any.**" 是主要用途，需要注意的是: 在使用的时候，是存在一对相反的过程。
>
> 

#### *Virtual Base (vbase) offsets* 

*Virtual Base (vbase) offsets* are used to access the virtual bases of an object. Such an entry is added to the derived class object address (i.e. the address of its virtual table pointer) to get the address of a virtual base class subobject. Such an entry is required for each virtual base class. The values can be positive or negative.

####  *offset to top*

The *offset to top* holds the displacement(位移) to the top of the object from the location within the object of the virtual table pointer that addresses this virtual table, as a `ptrdiff_t`. It is always present. The offset provides a way to find the top of the object from any base subobject with a virtual table pointer. This is necessary for `dynamic_cast<void*>` in particular.

#### typeinfo pointer

The *typeinfo pointer* points to the typeinfo object used for RTTI. It is always present. All entries in each of the virtual tables for a given class must point to the same typeinfo object. A correct implementation of typeinfo equality is to check pointer equality, except for pointers (directly or indirectly) to incomplete types. The typeinfo pointer is a valid pointer for polymorphic classes, i.e. those with virtual functions, and is zero for non-polymorphic classes.

#### Virtual function pointers

*Virtual function pointers* are used for virtual function dispatch. Each pointer holds either the address of a virtual function of the class, or the address of a **secondary entry point** that performs certain adjustments before transferring control to a virtual function.
