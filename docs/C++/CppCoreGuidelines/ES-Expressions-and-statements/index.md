# CppCoreGuidelines [ES: Expressions and statements](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es-expressions-and-statements)

## Declaration rules:

### [ES.20: Always initialize an object](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es20-always-initialize-an-object)

> NOTE: 
>
> 1、`Default-initialization`:
>
> > 对于built-in type，如果不对它进行initialization的话，那么它就是Indeterminate value，显然这是比较危险的；我们最好遵循
> >
> > CppCoreGuidelines [ES.20: Always initialize an object](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es20-always-initialize-an-object)



## Arithmetic rules:

### [ES.100: Don't mix signed and unsigned arithmetic](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es100-dont-mix-signed-and-unsigned-arithmetic)

> NOTE: 
>
> 1、在 pvs-studio [About size_t and ptrdiff_t](https://pvs-studio.com/en/a/0050/) 中，有非常好的例子



### [ES.107: Don't use `unsigned` for subscripts, prefer `gsl::index`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es107-dont-use-unsigned-for-subscripts-prefer-gslindex)

> NOTE: 
>
> 1、参见 `Array-index` 章节

