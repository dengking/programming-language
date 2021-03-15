# Returning `unique_ptr` from functions

1、resource return idiom是典型的"Returning `unique_ptr` from functions"，它涉及optimization in function return，下面文章对"Returning `unique_ptr` from functions"进行了说明。

2、resource ownership transfer

## stackoverflow [Returning unique_ptr from functions](https://stackoverflow.com/questions/4316727/returning-unique-ptr-from-functions)

```C++
#include <iostream>
#include <memory>

using namespace std;

unique_ptr<int> foo()
{
  unique_ptr<int> p( new int(10) );

  return p;                   // 1
  //return move( p );         // 2
}

int main()
{
  unique_ptr<int> p = foo();

  cout << *p << endl;
  return 0;
}
```

