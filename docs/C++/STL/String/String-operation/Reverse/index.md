# Reverse string



## geeksforgeeks [Different Methods to Reverse a String in C++](https://www.geeksforgeeks.org/reverse-a-string-in-c-cpp-different-methods/)

### 一、for-loop

1、Making a Custom Reverse Function For Swapping Characters

```c++
// C++ program to reverse a string
// using first to last approach
// 'for' loop
#include <bits/stdc++.h>
using namespace std;

// Function to reverse a string
void reverseStr(string &str)
{
    int n = str.length();

    // Swap character starting from two
    // corners
    for (int i = 0; i < n / 2; i++)
        swap(str[i], str[n - i - 1]);
}

// Driver program
int main()
{
    string str = "geeksforgeeks";
    reverseStr(str);
    cout << str;
    return 0;
}
```



### 二、Recursion 

1、Using recursion Function with two pointer approach

```c++
// C++ program to reverse a string
// using recursion
#include <bits/stdc++.h>
using namespace std;

// Function to reverse a string
void reverseStr(string &str, int n, int i)
{

    if (n <= i)
    {
        return;
    }
    // Swaping the character
    swap(str[i], str[n]);
    reverseStr(str, n - 1, i + 1);
}

// Driver program
int main()
{
    string str = "geeksforgeeks";
    reverseStr(str, str.length() - 1, 0);
    cout << str;
    return 0;
}

```



2、Using one pointer approach in recursion

```c++
// C++ program to reverse a string using recursion
#include <iostream>
using namespace std;
void getreverse(string &str, int i)
{
    if (i > (str.length() - 1 - i))
    {
        return;
    }
    swap(str[i], str[str.length() - i - 1]);
    i++;
    getreverse(str, i);
}
int main()
{
    string name = "geeksforgeeks";

    getreverse(name, 0);
    cout << name << endl;
    return 0;
}
// code contributed by pragatikohli

```



### 三、Using the inbuilt “reverse” Function

```c++
// C++ program to illustrate the
// reversing of a string using
// reverse() function
#include <bits/stdc++.h>
using namespace std;
int main()
{
    string str = "geeksforgeeks";

    // Reverse str[begin..end]
    reverse(str.begin(), str.end());

    cout << str;
    return 0;
}

```



### 四、Reverse a String Using the Constructor 

```c++
// C++ program to reverse
// string using constructor
#include <bits/stdc++.h>
using namespace std;
int main()
{
    string str = "GeeksforGeeks";

    // Use of reverse iterators
    string rev = string(str.rbegin(), str.rend());

    cout << rev << endl;
    return 0;
}
```



### 五、Using a Temporary String

```c++
// C++ program to demonstrate
// reversing of string
// using temporary string
#include <bits/stdc++.h>
using namespace std;
int main()
{

    string str = "GeeksforGeeks";
    int n = str.length();

    // Temporary string to store the reverse
    string rev;

    for (int i = n - 1; i >= 0; i--)
        rev.push_back(str[i]);

    cout << rev << endl;
    return 0;
}
```



### 六、Using Stack Data Structure



```c++
// C++ Program to reverse a string
#include <bits/stdc++.h>
using namespace std;
int main()
{
    string s = "GeeksforGeeks";
    stack<char> st;
    for (char x : s)
        st.push(x);
    while (!st.empty())
    {
        cout << st.top();
        st.pop();
    }
    return 0;
}

```



## stackoverflow [How do you reverse a string in place in C or C++?](https://stackoverflow.com/questions/198199/how-do-you-reverse-a-string-in-place-in-c-or-c)



### [A](https://stackoverflow.com/a/198210/10173843)

```c++
#include <algorithm>
std::reverse(str.begin(), str.end());
```

This is the simplest way in C++.



### [A](https://stackoverflow.com/a/198264/10173843)

The standard algorithm is to use pointers to the start / end, and walk them inward until they meet or cross in the middle. Swap as you go.

------

Reverse ASCII string, i.e. a 0-terminated array where every character fits in 1 `char`. (Or other non-multibyte character sets).

```bash

void strrev(char *head)
{
    if (!head)
        return;
    char *tail = head;
    while (*tail)
        ++tail; // find the 0 terminator, like head+strlen
    --tail;     // tail points to the last real char
                // head still points to the first
    for (; head < tail; ++head, --tail)
    {
        // walk pointers inwards until they meet or cross in the middle
        char h = *head, t = *tail;
        *head = t; // swapping as we go
        *tail = h;
    }
}

// test program that reverses its args
#include <stdio.h>

int main(int argc, char **argv)
{
    do
    {
        printf("%s ", argv[argc - 1]);
        strrev(argv[argc - 1]);
        printf("%s\n", argv[argc - 1]);
    } while (--argc);

    return 0;
}

```



The same algorithm works for integer arrays with known length, just use `tail = start + length - 1` instead of the end-finding loop.

(Editor's note: this answer originally used XOR-swap for this simple version, too. Fixed for the benefit of future readers of this popular question. [XOR-swap is *highly* not recommended](https://stackoverflow.com/questions/10549155/why-swap-dont-use-xor-operation-in-c); hard to read and making your code compile less efficiently. You can see [on the Godbolt compiler explorer](https://godbolt.org/z/pPNxJq) how much more complicated the asm loop body is when xor-swap is compiled for x86-64 with gcc -O3.)

------

#### Ok, fine, let's fix the UTF-8 chars...

(This is XOR-swap thing. Take care to note that you *must avoid* swapping with self, because if `*p` and `*q` are the same location you'll zero it with a^a==0. XOR-swap depends on having two distinct locations, using them each as temporary storage.)

Editor's note: you can replace SWP with a safe inline function using a tmp variable.

```cpp
#include <bits/types.h>
#include <stdio.h>

#define SWP(x,y) (x^=y, y^=x, x^=y)

void strrev(char *p)
{
  char *q = p;
  while(q && *q) ++q; /* find eos */
  for(--q; p < q; ++p, --q) SWP(*p, *q);
}

void strrev_utf8(char *p)
{
  char *q = p;
  strrev(p); /* call base case */

  /* Ok, now fix bass-ackwards UTF chars. */
  while(q && *q) ++q; /* find eos */
  while(p < --q)
    switch( (*q & 0xF0) >> 4 ) {
    case 0xF: /* U+010000-U+10FFFF: four bytes. */
      SWP(*(q-0), *(q-3));
      SWP(*(q-1), *(q-2));
      q -= 3;
      break;
    case 0xE: /* U+000800-U+00FFFF: three bytes. */
      SWP(*(q-0), *(q-2));
      q -= 2;
      break;
    case 0xC: /* fall-through */
    case 0xD: /* U+000080-U+0007FF: two bytes. */
      SWP(*(q-0), *(q-1));
      q--;
      break;
    }
}

int main(int argc, char **argv)
{
  do {
    printf("%s ",  argv[argc-1]);
    strrev_utf8(argv[argc-1]);
    printf("%s\n", argv[argc-1]);
  } while(--argc);

  return 0;
}
```

- Why, yes, if the input is borked, this will cheerfully swap outside the place.
- Useful link when vandalising in the UNICODE: http://www.macchiato.com/unicode/chart/
- Also, UTF-8 over 0x10000 is untested (as I don't seem to have any font for it, nor the patience to use a hexeditor)

Examples:

```bash
$ ./strrev Räksmörgås ░▒▓○◔◑◕●

░▒▓○◔◑◕● ●◕◑◔○▓▒░

Räksmörgås sågrömskäR

./strrev verrts/.
```



## stackoverflow [How to reverse an std::string? [duplicate]](https://stackoverflow.com/questions/4951796/how-to-reverse-an-stdstring)



### [A](https://stackoverflow.com/a/4951811/10173843)

```C++
string reversed(temp.rbegin(), temp.rend());
```

