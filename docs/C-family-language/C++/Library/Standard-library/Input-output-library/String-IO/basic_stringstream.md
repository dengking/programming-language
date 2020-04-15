# `basic_stringstream`



## cppreference [`basic_stringstream`](https://en.cppreference.com/w/cpp/io/basic_stringstream) 



## Application



### Decimal to hex

https://www.geeksforgeeks.org/stringstream-c-decimal-hexadecimal-back/

https://stackoverflow.com/questions/11181251/saving-hex-values-to-a-c-string

http://www.cplusplus.com/reference/ios/hex/

https://www.tutorialspoint.com/convert-an-integer-to-a-hex-string-in-cplusplus



```C++
#include <iostream>
#include <sstream>
#include <ios>
#include <iomanip>
#include <string>

int main()  
{
    unsigned char buf[] = { 0xAA, 0xD1, 0x09, 0x01, 0x10, 0xF1 };

    std::ostringstream s;
    s << std::hex << std::setfill('0') << std::uppercase
      << std::setw(2) << static_cast<int>(buf[0]) << ':'
      << std::setw(2) << static_cast<int>(buf[1]) << ':'
      << std::setw(2) << static_cast<int>(buf[2]) << ':'
      << std::setw(2) << static_cast<int>(buf[3]) << ':'
      << std::setw(2) << static_cast<int>(buf[4]) << ':'
      << std::setw(2) << static_cast<int>(buf[5]);

    std::cout << "[" << s.str() << "]\n";

    return 0;
}
```

