# dabeaz [Swig Master Class](https://www.dabeaz.com/SwigMaster/SWIGMaster.pdf)

## Part 7 Decorating the Parse Tree

### Giving Swig Hints

Swig only looks at the contents of headers. There are a lot of things that can be determined automatically. Especially certain semantics of classes. However, there are other aspects of making an extension module where user input is required



#### `%newobject` Directive

`%newobject` marks a declaration as returning newly allocated memory

```C++
%newobject strdup;
...
char *strdup(const char *s);
```

Maybe you want to know this so it can be cleaned up properly



### Declaration Annotation