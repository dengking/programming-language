# Examples

## stackoverflow [Which constructor will trigger the move semantics?](https://stackoverflow.com/questions/47490610/which-constructor-will-trigger-the-move-semantics)

[A](https://stackoverflow.com/a/47490667)

The correct way to do this is:

```cpp
Xyt::ByteArray::ByteArray(std::vector<char>&& Buf)
  : Buffer(std::move(Buf))
{}
```