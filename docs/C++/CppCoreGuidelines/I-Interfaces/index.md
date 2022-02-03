# CppCoreGuidelines [I: Interfaces](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-interfaces)



## [I.22: Avoid complex initialization of global objects](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i22-avoid-complex-initialization-of-global-objects)

> NOTE: 
>
> 一、原因: 
>
> 1、Static-Initialization-Order-Fiasco，参见 `Static-storage-duration` 章节
>
> 2、static initialization可能slowdown startup，关于此在chromium的doc中进行了非常详细的说明



## [I.4: Make interfaces precisely and strongly typed](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i4-make-interfaces-precisely-and-strongly-typed)

> NOTE: 
>
> 1、参见 `Strong-type` 章节



## [T.83: Do not declare a member function template virtual](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t83-do-not-declare-a-member-function-template-virtual)

#### Reason

C++ does not support that. If it did, vtbls could not be generated until link time. And in general, implementations must deal with dynamic linking.

> NOTE: 
>
> 1、当前，virtual table是compile time生成的；如果支持template virtual function，那么virtual table需要直到link time才能够生成，并且还需要支持dynamic linking。
>
> 2、参见 `Template-and-virtual` 章节

#### Alternative

Double dispatch, visitors, calculate which function to call