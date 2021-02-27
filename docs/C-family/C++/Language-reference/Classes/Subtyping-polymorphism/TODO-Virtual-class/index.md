
# Virtual class

## wikipedia [Virtual class](https://en.wikipedia.org/wiki/Virtual_class)

In [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), a **virtual class** is a nested [inner class](https://en.wikipedia.org/wiki/Inner_class) whose functions and member variables can be overridden and redefined by [subclasses](https://en.wikipedia.org/wiki/Subclass_(computer_science)) of an outer class.[[1\]](https://en.wikipedia.org/wiki/Virtual_class#cite_note-1) Virtual classes are analogous to [virtual functions](https://en.wikipedia.org/wiki/Virtual_function).

The run time type of a virtual class depends on the run time type of an object of the outer class. (Just like the run time type of an object decides which virtual function should be used.)

> NOTE: 由他的outer class的dynamic type决定

### Example

```C++
#include <iostream>

class Machine {
public:
    void run() { }

    class Parts {
    public:
        virtual int get_wheels() = 0;
        
        virtual std::string get_fuel_type() = 0;
    };
};

// The inner class "Parts" of the class "Machine" may return the number of wheels the machine has.
class Car: Machine {
public:
    void run() { 
        std::cout << "The car is running." << std::endl; 
    }
    
    class Parts: Machine::Parts {
    public:
        int get_wheels() override {
            std::cout << "A car has 4 wheels." << std::endl;
            return 4;
        }
        
        std::string get_fuel_type() override {
            std::cout << "A car uses gasoline for fuel." << std::endl;
            return "gasoline";
        }
    };
};
```

