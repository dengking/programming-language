# docs [Don't write a clang plugin](https://chromium.googlesource.com/chromium/src/+/main/docs/writing_clang_plugins.md)

> NOTE: 
>
> 这篇文章的观点是: 不到万不得已不要写plugin。

## Downcast early, Downcast often

The clang library will give you the most general types possible. For example `TagDecl` has comparably minimal interface. The library is designed so you will be downcasting all the time, and you won‘t use the standard `dynamic_cast<>()` builtin to do it. Instead, you’ll use llvm/clang's home built RTTI system:

```C++
  virtual void HandleTagDeclDefinition(TagDecl* tag) {
    if (CXXRecordDecl* record = dyn_cast<CXXRecordDecl>(tag)) {
      // Do stuff with |record|.
    }
  }
```

> NOTE: 
>
> 一、这是典型的在interface中使用abstract type，然后在implementation中downcast到具体的type