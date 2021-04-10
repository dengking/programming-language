# C++ Design by contract

## CppCoreGuidelines 中涉及design by contract的内容

### [I.6: Prefer `Expects()` for expressing preconditions](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Ri-expects)

### [I.8: Prefer `Ensures()` for expressing postconditions](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Ri-ensures)



### [ES.65: Don't dereference an invalid pointer](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es65-dont-dereference-an-invalid-pointer)

This would carry a cost only when the assertion checking was enabled and would give a compiler/analyzer useful information. This would work even better if/when C++ gets direct support for contracts:

```C++
void f3(int* p) // state that p is not supposed to be nullptr
    [[expects: p]]
{
    int x = *p;
}
```

### [NR.3: Don't avoid exceptions](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#nr3-dont-avoid-exceptions)

Contracts/assertions: Use GSL's `Expects` and `Ensures` (until we get language support for contracts)



### [GSL.assert: Assertions](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#gslassert-assertions)



These assertions are currently macros (yuck!) and must appear in function definitions (only) pending standard committee decisions on contracts and assertion syntax. See [the contract proposal](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0380r1.pdf); using the attribute syntax, for example, `Expects(p)` will become `[[expects: p]]`.





## p0380r1 [the contract proposal](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0380r1.pdf)

Preconditions("expects")、postconditions("ensures") and assertions("assert") are collectively called contract. A contract has no observable effect in a correct program.



## TODO

### drdobbs [Programming with Contracts in C++](https://www.drdobbs.com/cpp/programming-with-contracts-in-c/184405997)



### codeproject [Design by Contract in C++](https://www.codeproject.com/Articles/8293/Design-by-Contract-in-C)



### eventhelix [Design by Contract Programming in C++](https://www.eventhelix.com/RealtimeMantra/Object_Oriented/design_by_contract.htm)

