# Examples

## example1

```c++
#include <iostream>
#include <vector>
#include <climits> // INT_MAXÒ
using namespace std;

struct Snapshot
{
  double x = 0.0;
  int y = 1;
};

struct Snapshot2
{
  double x;
  int y;
};
int main()
{
  Snapshot s;
  cout << s.x << endl;
  cout << s.y << endl;
  Snapshot2 s2;
  cout << s2.x << endl;
  cout << s2.y << endl;
  Snapshot2 s3{}; // value initialization
  cout << s3.x << endl;
  cout << s3.y << endl;
}

```

上述例子来自于stackoverflow [Are members of a C++ struct initialized to 0 by default?](https://stackoverflow.com/questions/1069621/are-members-of-a-c-struct-initialized-to-0-by-default) # [A](https://stackoverflow.com/a/1069634)