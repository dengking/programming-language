# Safe navigation operator

是在阅读wikipedia [Pyramid of doom (programming)](https://en.wikipedia.org/wiki/Pyramid_of_doom_(programming))时，其中提及的此。

## wikipedia [Safe navigation operator](https://en.wikipedia.org/wiki/Safe_navigation_operator)

In [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), the **safe navigation operator** (also known as **optional chaining operator**, **safe call operator**, **null-conditional operator**) is a binary operator that returns [null](https://en.wikipedia.org/wiki/Null_pointer) if its first argument is null; otherwise it performs a dereferencing operation as specified by the second argument (typically an object member access, array index, or lambda invocation).

### Examples

#### Swift

Optional chaining operator[[12\]](https://en.wikipedia.org/wiki/Safe_navigation_operator#cite_note-12), subscript operator, and call:

```swift
let name = article?.authors?[0].name
let result = protocolVar?.optionalRequirement?()
```