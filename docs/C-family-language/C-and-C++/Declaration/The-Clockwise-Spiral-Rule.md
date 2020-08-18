# c-faq [The Clockwise/Spiral Rule](http://c-faq.com/decl/spiral.anderson.html)

> Clockwise  顺时针
Spiral 螺旋

This was posted to comp.lang.c by its author, David Anderson, on 1994-05-06.



## By David Anderson

There is a technique known as the "Clockwise/Spiral Rule'' which enables any C programmer to parse in their head any C **declaration**!

There are three simple steps to follow:

**1:**

Starting with the unknown element, move in a spiral/clockwise direction（顺时针、旋转方向）; when ecountering the following elements replace them with the corresponding english statements:

- `[ ] or []`

  => Array `X` size of`...` or Array undefined size of`...`

- `(type1, type2)`

  => function passing `type1` and `type2` returning`...`

- `*`

  => pointer(s) to`...`

**2:**

Keep doing this in a spiral/clockwise direction until all tokens have been covered.

**3:**

Always resolve anything in parenthesis first!

> NOTE: 上述`...`表示需要根据declaration替换的

### Example #1: Simple declaration

```c
                     +-------+
                     | +-+   |
                     | ^ |   |
                char *str[10];
                 ^   ^   |   |
                 |   +---+   |
                 +-----------+
```

Question we ask ourselves: What is `str`?

> str is an...

- We move in a spiral clockwise direction starting with `str` and the first character we see is a `[` so, that means we have an array, so...

  > `str` is an array 10 of...

- Continue in a spiral clockwise direction, and the next thing we encounter is the `*` so, that means we have pointers, so...

  > `str` is an array 10 of pointers to...

- Continue in a spiral direction and we see the end of the line (the `;`), so keep going and we get to the type `char`, so...

  > `str` is an array 10 of pointers to char''

- We have now **visited**' every token; therefore we are done!

### Example #2: Pointer to Function declaration

```c
                     +--------------------+
                     | +---+              |
                     | |+-+|              |
                     | |^ ||              |
                char *(*fp)( int, float *);
                 ^   ^ ^  ||              |
                 |   | +--+|              |
                 |   +-----+              |
                 +------------------------+
```

Question we ask ourselves: What is `fp`?

> `fp` is a...

- Moving in a spiral clockwise direction, the first thing we see is a `)`; therefore, `fp` is inside parenthesis, so we continue the spiral inside the parenthesis and the next character seen is the `*`, so...

  > `fp` is a pointer to...

- We are now out of the parenthesis and continuing in a spiral clockwise direction, we see the `(`; therefore, we have a function, so...

  > `fp` is a pointer to a function passing an `int` and a pointer to `float` returning...

- Continuing in a spiral fashion, we then see the `*` character, so...

  > `fp` is a pointer to a function passing an `int` and a pointer to `float` returning a pointer to...

- Continuing in a spiral fashion we see the `;`, but we haven't visited all tokens, so we continue and finally get to the type `char`, so...

  > `fp` is a pointer to a function passing an `int` and a pointer to `float` returning a pointer to a `char`

### Example #3: The "Ultimate''

```c
                      +-----------------------------+
                      |                  +---+      |
                      |  +---+           |+-+|      |
                      |  ^   |           |^ ||      |
                void (*signal(int, void (*fp)(int)))(int);
                 ^    ^      |      ^    ^  ||      |
                 |    +------+      |    +--+|      |
                 |                  +--------+      |
                 +----------------------------------+
```

Question we ask ourselves: What is `signal`?

Notice that `signal` is *inside* parenthesis, so we must resolve this first!

- Moving in a clockwise direction we see `(`  so we have...

  > `signal` is a function passing an `int` and a...

- Hmmm, we can use this same rule on `fp`, so... What is `fp`? `fp` is also inside parenthesis so continuing we see an `*`, so...

  > fp is a pointer to...

- Continue in a spiral clockwise direction and we get to `(`, so...

  > `fp` is a pointer to a function passing `int` returning...''

- Now we continue out of the function parenthesis and we see `void`, so...

  > `fp` is a pointer to a function passing int returning nothing (`void`)''

- We have finished with `fp` so let's catch up with `signal`, we now have...

  > signal is a function passing an int and a pointer to a function passing an int returning nothing (void) returning...

- We are still inside parenthesis so the next character seen is a `*`, so...

  > `signal` is a function passing an `int` and a pointer to a function passing an `int` returning nothing (`void`) returning a pointer to...

- We have now resolved the items within parenthesis, so continuing clockwise, we then see another `(`, so...

  > `signal` is a function passing an `int` and a pointer to a function passing an int returning nothing (void) returning a pointer to a function passing an int returning...

- Finally

   

  we continue and the only thing left is the word `void`, so the final complete definition for signal is:

  > `signal` is a function passing an int and a pointer to a function passing an int returning nothing (void) returning a pointer to a function passing an int returning nothing (void)

The same rule is applied for `const` and `volatile`. For Example:

```c++
	const char *chptr;
```

- Now, what is `chptr`??

  > `chptr` is a pointer to a char constant''

How about this one:

```c++
	char * const chptr;
```

- Now, what is `chptr`??

  > `chptr` is a constant pointer to `char`'

Finally:

```c++
	volatile char * const chptr;
```

- Now, what is `chptr`??

  > `chptr` is a constant pointer to a char volatile.

Practice this rule with the examples found in K&R II on page 122.

## excercise

```c
int (*(*foo)(double))[3] = nullptr;
```

`foo` is a pointer to a function passing `double` returning pointer to an array size of 3 type of int



## Examples

### stackoverflow [What is the difference between const int*, const int * const, and int const *?](https://stackoverflow.com/questions/1143262/what-is-the-difference-between-const-int-const-int-const-and-int-const)

Q:

I always mess up how to use `const int*`, `const int * const`, and `int const *` correctly. Is there a set of rules defining what you can and cannot do?

I want to know all the do's and all don'ts in terms of assignments, passing to the functions, etc.



A:

You can use the ["Clockwise/Spiral Rule"](http://c-faq.com/decl/spiral.anderson.html) to decipher most C and C++ declarations. – [James McNellis](https://stackoverflow.com/users/151292/james-mcnellis) 

A:

[cdecl.org](http://www.cdecl.org/) is a great website which auto-translates C declarations for you. – [Dave Gallagher](https://stackoverflow.com/users/153040/dave-gallagher) 

[A](https://stackoverflow.com/a/1143272):

Read it backwards (as driven by [Clockwise/Spiral Rule](http://c-faq.com/decl/spiral.anderson.html)):

- `int*` - pointer to int
- `int const *` - pointer to const int
- `int * const` - const pointer to int
- `int const * const` - const pointer to const int

Now the first `const` can be on either side of the type so:

- `const int *` == `int const *`
- `const int * const` == `int const * const`

If you want to go really crazy you can do things like this:

- `int **` - pointer to pointer to int
- `int ** const` - a const pointer to a pointer to an int
- `int * const *` - a pointer to a const pointer to an int
- `int const **` - a pointer to a pointer to a const int
- `int * const * const` - a const pointer to a const pointer to an int
- ...

And to make sure we are clear on the meaning of const

```
const int* foo;
int *const bar; //note, you actually need to set the pointer 
                //here because you can't change it later ;)
```

`foo` is a variable pointer to a constant integer. This lets you change what you point to but not the value that you point to. Most often this is seen with C-style strings where you have a pointer to a `const char`. You may change which string you point to but you can't change the content of these strings. This is important when the **string** itself is in the **data segment** of a program and shouldn't be changed.

`bar` is a constant or fixed pointer to a value that can be changed. This is like a **reference** without the extra syntactic sugar. Because of this fact, usually you would use a reference where you would use a `T* const` pointer unless you need to allow `NULL` pointers.

