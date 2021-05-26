# stackoverflow [What is the strict aliasing rule?](https://stackoverflow.com/questions/98650/what-is-the-strict-aliasing-rule)

When asking about [common undefined behavior in C](https://stackoverflow.com/questions/98340/what-are-the-common-undefinedunspecified-behavior-for-c-that-you-run-into), souls more enlightened than I referred to the strict aliasing rule.
What are they talking about?



## [A](https://stackoverflow.com/a/99010)

A typical situation you encounter strict aliasing problems is when overlaying a `struct` (like a device/network msg) onto a buffer of the word size of your system (like a pointer to `uint32_t`s or `uint16_t`s). When you overlay a `struct` onto such a buffer, or a buffer onto such a `struct` through pointer casting you can easily violate strict aliasing rules.

So in this kind of setup, if I want to send a message to something I'd have to have two incompatible pointers pointing to the same chunk of memory. I might then naively code something like this:

```c
typedef struct Msg
{
    unsigned int a;
    unsigned int b;
} Msg;

void SendWord(uint32_t);

int main(void)
{
    // Get a 32-bit buffer from the system
    uint32_t* buff = malloc(sizeof(Msg));

    // Alias that buffer through message
    Msg* msg = (Msg*)(buff);

    // Send a bunch of messages    
    for (int i =0; i < 10; ++i)
    {
        msg->a = i;
        msg->b = i+1;
        SendWord(buff[0]);
        SendWord(buff[1]);   
    }
}
```

The strict aliasing rule makes this setup illegal: dereferencing a pointer that aliases an object that is not of a [compatible type](http://en.cppreference.com/w/c/language/type) or one of the other types allowed by C 2011 6.5 paragraph 71 is undefined behavior. Unfortunately, you can still code this way, *maybe* get some warnings, have it compile fine, only to have weird unexpected behavior when you run the code.

(GCC appears somewhat inconsistent in its ability to give aliasing warnings, sometimes giving us a friendly warning and sometimes not.)

To see why this behavior is undefined, we have to think about what the strict aliasing rule buys the compiler. Basically, with this rule, it doesn't have to think about inserting instructions to refresh the contents of `buff` every run of the loop. Instead, when optimizing, with some annoyingly unenforced assumptions about aliasing, it can omit those instructions, load `buff[0]` and `buff[1`] into CPU registers once before the loop is run, and speed up the body of the loop. Before strict aliasing was introduced, the compiler had to live in a state of paranoia that the contents of `buff` could change at anytime from anywhere by anybody. So to get an extra performance edge, and assuming most people don't type-pun pointers, the strict aliasing rule was introduced.

Keep in mind, if you think the example is contrived, this might even happen if you're passing a buffer to another function doing the sending for you, if instead you have.

```c
void SendMessage(uint32_t* buff, size_t size32)
{
    for (int i = 0; i < size32; ++i) 
    {
        SendWord(buff[i]);
    }
}
```

And rewrote our earlier loop to take advantage of this convenient function

```c
for (int i = 0; i < 10; ++i)
{
    msg->a = i;
    msg->b = i+1;
    SendMessage(buff, 2);
}
```

The compiler may or may not be able to or smart enough to try to inline SendMessage and it may or may not decide to load or not load buff again. If `SendMessage` is part of another API that's compiled separately, it probably has instructions to load buff's contents. Then again, maybe you're in C++ and this is some templated header only implementation that the compiler thinks it can inline. Or maybe it's just something you wrote in your .c file for your own convenience. Anyway undefined behavior might still ensue. Even when we know some of what's happening under the hood, it's still a violation of the rule so no well defined behavior is guaranteed. So just by wrapping in a function that takes our word delimited buffer doesn't necessarily help.

**So how do I get around this?**

- Use a union. Most compilers support this without complaining about strict aliasing. This is allowed in C99 and explicitly allowed in C11.

  ```c
  union {
      Msg msg;
      unsigned int asBuffer[sizeof(Msg)/sizeof(unsigned int)];
  };
  ```

- You can disable strict aliasing in your compiler ([f[no-\]strict-aliasing](http://gcc.gnu.org/onlinedocs/gcc-4.6.1/gcc/Optimize-Options.html#index-fstrict_002daliasing-825) in gcc))

- You can use `char*` for aliasing instead of your system's word. The rules allow an exception for `char*` (including `signed char` and `unsigned char`). It's always assumed that `char*` aliases other types. However this won't work the other way: there's no assumption that your struct aliases a buffer of chars.

**Beginner beware**

This is only one potential minefield when overlaying two types onto each other. You should also learn about [endianness](http://en.wikipedia.org/wiki/Endianness), [word alignment](http://web.archive.org/web/20170708093042/http://www.cs.umd.edu:80/class/sum2003/cmsc311/Notes/Data/aligned.html), and how to deal with alignment issues through [packing structs](http://grok2.com/structure_packing.html) correctly.

### Footnote

1 The types that C 2011 6.5 7 allows an lvalue to access are:

- a type compatible with the effective type of the object,
- a qualified version of a type compatible with the effective type of the object,
- a type that is the signed or unsigned type corresponding to the effective type of the object,
- a type that is the signed or unsigned type corresponding to a qualified version of the effective type of the object,
- an aggregate or union type that includes one of the aforementioned types among its members (including, recursively, a member of a subaggregate or contained union), or
- a character type.