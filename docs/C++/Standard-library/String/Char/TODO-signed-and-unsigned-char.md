# signed and unsigned

在 stackoverflow [What does the “::” mean in “::tolower”?](https://stackoverflow.com/questions/5270780/what-does-the-mean-in-tolower)  # [A](https://stackoverflow.com/a/5270970) 中对 signed 和 unsigned char 进行了讨论，我是阅读它时才发现的这个问题。

需要使用interpretation model、type determine everything的观点来进行理解，同一个memory representation，不同的type，则最终的value是不同的。下面的signed char和unsigned char之间的差异能够非常好的体现出这个观点。

## stackoverflow [What does the “::” mean in “::tolower”?](https://stackoverflow.com/questions/5270780/what-does-the-mean-in-tolower)  # [A](https://stackoverflow.com/a/5270970):

While I'm at it, I might mention that using `::tolower` like this is **undefined behavior**, at least if plain char is **signed**. 

> NOTE: 这段话的意思是如果plain char is signed，则using `::tolower` like this is **undefined behavior**。因此理解这段话的一个前提是知道: plain char可能是signed也可能是unsigned，关于这一点，在 `C-and-C++\String\Char` 章节中进行了描述。

The input to `::tolower` is an int, and must be in the range `0`...`UCHAR_MAX`, or `EOF`. If plain char is signed, some of the characters may have negative encodings, which results in **undefined behavior**. 

> NOTE: 为什么会导致**undefined behavior**？

In practice, most implementations make this work. For all characters except `0xFF` (ÿ in Latin 1). If you're not concerned with portability, some compilers have a switch to make char unsigned---use it. Otherwise, write a small functional object to handle it correctly, either:

```C++
struct ToLower
{
    char operator()( char ch ) const
    {
        return ::tolower( static_cast<unsigned char>(ch) );
    }
};
```

or (better, but significantly more work---only worth it if your using it a lot), a functional object whose constructor  takes a locale (defaulting to the global locale) and contains a reference to an `std::ctype`, which it uses for the `tolower` function. (Of course, if you're really internationalized, `tolower` probably doesn't have any meaning. And you'll be using UTF-8, which is a multi-byte encoding, and doesn't work with any of the available possibilities.)



## linuxtopia [Portability of signed and unsigned types](https://www.linuxtopia.org/online_books/an_introduction_to_gcc/gccintro_71.html)

The C and C++ standards allows the character type `char` to be signed or unsigned, depending on the platform and compiler. Most systems, including x86 GNU/Linux and Microsoft Windows, use **signed** `char`, but those based on PowerPC and ARM processors typically use **unsigned** `char`.([29](https://www.linuxtopia.org/online_books/an_introduction_to_gcc/gccintro_foot.html#FOOT29)) This can lead to unexpected results when porting programs between platforms which have different defaults for the type of `char`.

The following code demonstrates the difference between platforms with signed and unsigned `char` types:

```C++
#include <stdio.h>

int 
main (void)
{
  char c = 255;// 0XFF
  if (c > 128) {
    printf ("char is unsigned (c = %d)\n", c);
  } else {
    printf ("char is signed (c = %d)\n", c);
  }
  return 0;
}
```

With an unsigned `char`, the variable `c` takes the value 255, but with a signed `char` it becomes -1.

The correct way to manipulate `char` variables in C is through the portable functions declared in `'ctype.h'`, such as `isalpha`, `isdigit` and `isblank`, rather than by their numerical values. The behavior of non-portable conditional expressions such as `c > 'a'` depends on the signedness of the `char` type. If the signed or unsigned version of `char` is explicitly required at certain points in a program, it can be specified using the declarations `signed char` or `unsigned char`.

For existing programs which assume that `char` is signed or unsigned, GCC provides the options `-fsigned-char` and `-funsigned-char` to set the default type of `char`. Using these options, the example code above compiles cleanly when `char` is unsigned:

```
$ gcc -Wall -funsigned-char signed.c 
$ ./a.out 
char is unsigned (c = 255)
```

However, when `char` is signed the value 255 wraps around to -1, giving a warning:

```
$ gcc -Wall -fsigned-char signed.c 
signed.c: In function `main':
signed.c:7: warning: comparison is always false due to 
  limited range of data type
$ ./a.out 
char is signed (c = -1)
```

The warning message "comparison is always true/false due to limited range of data type" is one symptom of code which assumes a definition of `char` which is different from the actual type.

The most common problem with code written assuming signed `char` types occurs with the functions `getc`, `fgetc` and `getchar` (which read a character from a file). They have a return type of `int`, not `char`, and this allows them to use the special value -1 (defined as `EOF`) to indicate an end-of-file error. Unfortunately, many programs have been written which incorrectly store this return value straight into a `char` variable. Here is a typical example:

```C++
#include <stdio.h>

int
main (void)
{
  char c;
  while ((c = getchar()) != EOF) /* not portable */
    {
      printf ("read c = '%c'\n", c);
    }
  return 0;
}
```

This only works on platforms which default to a signed `char` type.([30](https://www.linuxtopia.org/online_books/an_introduction_to_gcc/gccintro_foot.html#FOOT30)) On platforms which use an unsigned `char` the same code will fail, because the value -1 becomes 255 when stored in an `unsigned char`. This usually causes an infinite loop because the end of the file cannot be recognized.([31](https://www.linuxtopia.org/online_books/an_introduction_to_gcc/gccintro_foot.html#FOOT31)) To be portable, the program should test the return value as an integer before coercing it to a `char`, as follows:

```C++
#include <stdio.h>

int
main (void)
{
  int i;
  while ((i = getchar()) != EOF)
    {
      unsigned char c = i;
      printf ("read c = '%c'\n", c);
    }
  return 0;
}
```



The same considerations described in this section apply to the definitions of bitfields in structs, which can be signed or unsigned by default. In GCC, the default type of bitfields can be controlled using the options `-fsigned-bitfields` and `-funsigned-bitfields`.

## stackoverflow [Is char signed or unsigned by default?](https://stackoverflow.com/questions/2054939/is-char-signed-or-unsigned-by-default)

[A](https://stackoverflow.com/a/2055015)

> NOTE: 这个回答是最容易理解的

[C99 N1256 draft](http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf) 6.2.5/15 "Types" has this to say about the signed-ness of type `char`:

> The implementation shall define char to have the same range, representation, and behavior as either signed char or unsigned char.

and in a footnote:

> `CHAR_MIN`, defined in `<limits.h>`, will have one of the values `0` or `SCHAR_MIN`, and this can be used to distinguish the two options. Irrespective(不管) of the choice made, `char` is a separate type from the other two and is not compatible with either.

> NOTE: 不管选择哪种类型，char都是与其他两种类型分开的，而且都不兼容。

[A](https://stackoverflow.com/a/2054941)

The book is wrong. The standard does not specify if plain `char` is signed or unsigned.

In fact, the standard defines three distinct types: `char`, `signed char`, and `unsigned char`. If you `#include <limits.h>` and then look at `CHAR_MIN`, you can find out if plain `char` is `signed` or `unsigned` (if `CHAR_MIN` is less than 0 or equal to 0), but even then, the *three types are distinct* as far as the standard is concerned.

Do note that `char` is special in this way. If you declare a variable as `int` it is 100% equivalent to declaring it as `signed int`. This is always true for all compilers and architectures.