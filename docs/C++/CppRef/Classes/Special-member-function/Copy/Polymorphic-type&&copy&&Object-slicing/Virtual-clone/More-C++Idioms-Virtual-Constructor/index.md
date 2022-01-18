# More C++ Idioms/Virtual Constructor

## Intent 

To create a copy of an object or a new object without knowing its concrete type.

## Motivation

Uses of calling member functions in a class hierarchy polymorphically are well known in the object-oriented programming community. It is a way of implementing the **is-a** (or more practically, **behaves-as-a**) relationship. Sometimes it is useful to call life-cycle management (creation, copy, and destruction) functions of a class hierarchy polymorphically.

C++ natively supports polymorphic destruction of objects using a virtual destructor. An equivalent support for creation and copying of objects is missing. In C++, creation of object(s) always requires its type to be known at compile-time. The Virtual Constructor idiom allows polymorphic creation of and copying of objects in C++.

### Solution and Sample Code

The effect of a virtual constructor by a create() member function for creation and a clone() member function for copy construction as shown below.

```c++
class Employee 
{
  public:
    virtual ~Employee () {}                 // Native support for polymorphic destruction.
    virtual Employee * create () const = 0; // Virtual constructor (creation) 
    virtual Employee * clone () const = 0;  // Virtual constructor (copying) 
};
class Manager : public Employee     // "is-a" relationship
{
  public:
    Manager ();                     // Default constructor
    Manager (Manager const &);      // Copy constructor
    virtual ~Manager () {}                  // Destructor
    Manager * create () const       // Virtual constructor (creation) 
    {
      return new Manager();
    }
    Manager * clone () const        // Virtual constructor (copying) 
    {
      return new Manager (*this);
    }
};
class Programmer : public Employee { /* Very similar to the Manager class */ };
Employee * duplicate (Employee const & e)
{
   return e.clone();  // Using virtual constructor idiom.
}
```

The Manager class implements the two pure virtual functions and uses the type name (Manager) to create them. The function duplicate shows how virtual constructor idiom is used. It does not really know what it is duplicating. It only knows that it is cloning an Employee. The responsibility of creating the right instance is delegated to the derived classes. The duplicate function is therefore closed for modifications even though the class hierarchy rooted at Employee gets more sub-classes added in the future.

The return type of the clone and create member functions of the Manager class is not Employee but the class itself. C++ allows this flexibility in types where the return type of the over-ridden function can be a derived type of that of the function in the base class. This language feature is known as **co-variant return types.**

To handle resource ownership properly, the [Resource Return](https://en.m.wikibooks.org/wiki/More_C%2B%2B_Idioms/Resource_Return) idiom should be employed for the return types of clone() and create() functions as they are factory functions. If used, the return types (`shared_ptr<Employee>` and `shared_ptr<Manager>`) are no longer covariant return types and program should fail to compile. In such a case, the virtual constructor functions in the derived class should return the exact type as in the parent class.

```c++
#include <tr1/memory>

class Employee
{
  public:
    typedef std::tr1::shared_ptr<Employee> Ptr;
    virtual ~Employee () {}                    // Native support for polymorphic destruction.
    virtual Ptr create () const = 0; // Virtual constructor (creation)
    virtual Ptr clone () const = 0;  // Virtual constructor (copying)
};
class Manager : public Employee     // "is-a" relationship
{
  public:
    Manager () {}                     // Default constructor
    Manager (Manager const &) {}      // Copy constructor
    virtual ~Manager () {}
    Ptr create () const       // Virtual constructor (creation)
    {
      return Ptr(new Manager());
    }
    Ptr clone () const        // Virtual constructor (copying)
    {
      return Ptr(new Manager (*this));
    }
};
```