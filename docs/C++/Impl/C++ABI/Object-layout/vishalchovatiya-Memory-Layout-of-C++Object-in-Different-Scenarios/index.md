# vishalchovatiya [Memory Layout of C++ Object in Different Scenarios](http://www.vishalchovatiya.com/memory-layout-of-cpp-object/)



## Memory Layout of Simple & Non-Polymorphic Object in C++

```c++
class X {
    int     x;
    float   xx;

public:
    X() {}
    ~X() {}

    void printInt() {}
    void printFloat() {}
};
```

### Memory layout

```c++
      |                        |          
      |------------------------| <------ X class object memory layout
      |        int X::x        |
      |------------------------|  stack segment
      |       float X::xx      |       |   
      |------------------------|       |
      |                        |      \|/
      |                        |    
      |                        |
------|------------------------|----------------
      |         X::X()         | 
      |------------------------|       |   
      |        X::~X()         |       |
      |------------------------|      \|/
      |      X::printInt()     |  text segment
      |------------------------|
      |     X::printFloat()    |
      |------------------------|
      |                        |            
```

> NOTE: 
>
> `X::x` 先入栈，`X::xx` 后入栈

## Layout of an Object With Virtual Function & Static Data Member

```c++
class X {
    int         x;
    float       xx;
    static int  count;

public:
    X() {}
    virtual ~X() {}

    virtual void printAll() {}
    void printInt() {}
    void printFloat() {}
    static void printCount() {}
};
```

### Memory layout

```c++
      |                        |          
      |------------------------| <------ X class object memory layout
      |        int X::x        |
stack |------------------------|
  |   |       float X::xx      |                      
  |   |------------------------|      |-------|--------------------------|
  |   |         X::_vptr       |------|       |       type_info X        |
 \|/  |------------------------|              |--------------------------|
      |           o            |              |    address of X::~X()    |
      |           o            |              |--------------------------|
      |           o            |              | address of X::printAll() |
      |                        |              |--------------------------|
      |                        |
------|------------------------|------------
      |  static int X::count   |      /|\
      |------------------------|       |
      |           o            |  data segment           
      |           o            |       |
      |                        |      \|/
------|------------------------|------------
      |        X::X()          | 
      |------------------------|       |   
      |        X::~X()         |       |
      |------------------------|       | 
      |      X::printAll()     |      \|/ 
      |------------------------|  text segment
      |      X::printInt()     |
      |------------------------|
      |     X::printFloat()    |
      |------------------------|
      | static X::printCount() |
      |------------------------|
      |                        |
```

## Layout of C++ Object With Inheritance

```c++
class X {
    int     x;
    string str;

public:
    X() {}
    virtual ~X() {}

    virtual void printAll() {}
};

class Y : public X {
    int     y;

public:
    Y() {}
    ~Y() {}

    void printAll() {}
};
```

### Memory layout

```c++
      |                              |          
      |------------------------------| <------ Y class object memory layout
      |          int X::x            |
stack |------------------------------|
  |   |              int string::len |
  |   |string X::str ----------------|
  |   |            char* string::str |         
 \|/  |------------------------------|      |-------|--------------------------|
      |           X::_vptr           |------|       |       type_info Y        |
      |------------------------------|              |--------------------------|
      |          int Y::y            |              |    address of Y::~Y()    |
      |------------------------------|              |--------------------------|
      |               o              |              | address of Y::printAll() |
      |               o              |              |--------------------------|
      |               o              |              
------|------------------------------|--------
      |           X::X()             | 
      |------------------------------|       |   
      |           X::~X()            |       |
      |------------------------------|       | 
      |         X::printAll()        |      \|/ 
      |------------------------------|  text segment
      |           Y::Y()             |
      |------------------------------|
      |           Y::~Y()            |
      |------------------------------|
      |         Y::printAll()        |
      |------------------------------|
      |      string::string()        |
      |------------------------------|
      |      string::~string()       |
      |------------------------------|
      |      string::length()        |
      |------------------------------|
      |               o              |
      |               o              |
      |               o              |
      |                              |
```

## Memory Layout of an Object With Multiple Inheritances & Virtual Function

```c++
class X {
public:
    int     x;
    
    virtual ~X() {}
    virtual void printX() {}
};

class Y {
public:
    int     y;
    
    virtual ~Y() {}
    virtual void printY() {}
};

class Z : public X, public Y {
public:
    int     z;
    
    ~Z() {}
    void printX() {}
    void printY() {}
    void printZ() {}
};
```

### Memory layout

```c++
      |                              |          
      |------------------------------| <------ Z class object memory layout
stack |          int X::x            |         
  |   |------------------------------|                  |--------------------------|      
  |   |          X:: _vptr           |----------------->|       type_info Z        |
  |   |------------------------------|                  |--------------------------|
 \|/  |          int Y::y            |                  |    address of Z::~Z()    |
      |------------------------------|                  |--------------------------|
      |          Y:: _vptr           |------|           |   address of Z::printX() |
      |------------------------------|      |           |--------------------------|
      |          int Z::z            |      |           |--------GUARD_AREA--------|    
      |------------------------------|      |           |--------------------------|
      |              o               |      |---------->|       type_info Z        |
      |              o               |                  |--------------------------|
      |              o               |                  |    address of Z::~Z()    |
      |                              |                  |--------------------------|
------|------------------------------|---------         |   address of Z::printY() |
      |           X::~X()            |       |          |--------------------------|  
      |------------------------------|       |          
      |          X::printX()         |       |        
      |------------------------------|       |         
      |           Y::~Y()            |      \|/        
      |------------------------------|  text segment
      |          Y::printY()         |                
      |------------------------------|                
      |           Z::~Z()            |                
      |------------------------------|                
      |          Z::printX()         |                
      |------------------------------|                
      |          Z::printY()         |                
      |------------------------------|                
      |          Z::printZ()         |                
      |------------------------------|                
      |               o              |                
      |               o              |                
      |                              |             
```

## Layout of Object Having Virtual Inheritance

```c++
class X { int x; };
class Y : public virtual X { int y; };
class Z : public virtual X { int z; };
class A : public Y, public Z { int a; };
```



### Memory layout:

```c++
                  |                |          
 Y class  ------> |----------------| <------ A class object memory layout
sub-object        |   Y::y         |          
                  |----------------|             |------------------| 
                  |   Y::_vptr_Y   |------|      |    offset of X   | // offset(20) starts from Y 
 Z class  ------> |----------------|      |----> |------------------|     
sub-object        |   Z::z         |             |       .....      |     
                  |----------------|             |------------------|  
                  |   Z::_vptr_Z   |------|       
                  |----------------|      |        
 A sub-object --> |   A::a         |      |      |------------------| 
                  |----------------|      |      |    offset of X   | // offset(12) starts from Z
 X class -------> |   X::x         |      |----> |------------------|          
 shared           |----------------|             |       .....      |           
 sub-object       |                |             |------------------|        
```

