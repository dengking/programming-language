# String



## Operation

### Upper Case

[Converting to uppercase in C++](https://stackoverflow.com/questions/8693990/converting-to-uppercase-in-c)

[Convert a String In C++ To Upper Case](https://stackoverflow.com/questions/735204/convert-a-string-in-c-to-upper-case)

```c++
#include "ctype.h"
#include <algorithm>
#include <iterator>
#include <string>

namespace StringUtil
{
static void ToUpper(std::string& S)
{
	std::transform(S.begin(), S.end(), S.begin(), ::toupper);
}
}
```

