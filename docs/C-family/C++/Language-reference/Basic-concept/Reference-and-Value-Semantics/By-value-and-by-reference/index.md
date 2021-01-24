# By value and by reference

### Pass-by-value and pass-by-reference

这两者是最最能够体现value-semantic和reference-semantic的。



### by value and by reference

显然by value涉及到copy，并且构造的对象在函数调用结束后就会被release掉；但是by reference则不存在。

> This is not just a matter of convenience but in fact an optimization. If the parameter (s) binds to an **lvalue** (another non-const object), a copy of the object is made automatically while creating the parameter (s). However, when s binds to an rvalue (temporary object, literal), the copy is typically elided, which saves a call to a copy constructor and a destructor. In the earlier version of the assignment operator where the parameter is accepted as **const reference**, **copy elision** does not happen when the **reference** binds to an **rvalue**. This results in an additional object being created and destroyed.

