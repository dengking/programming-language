# [Static variable](https://en.wikipedia.org/wiki/Static_variable)

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), a **static variable** is a [variable](https://en.wikipedia.org/wiki/Variable_(programming)) that has been [allocated](https://en.wikipedia.org/wiki/Memory_allocation) "statically", meaning that its [lifetime](https://en.wikipedia.org/wiki/Variable_lifetime) (or "extent") is the entire run of the program. This is in contrast to shorter-lived [automatic variables](https://en.wikipedia.org/wiki/Automatic_variable), whose storage is [stack allocated](https://en.wikipedia.org/wiki/Stack_allocation)and deallocated on the [call stack](https://en.wikipedia.org/wiki/Call_stack); and in contrast to [objects](https://en.wikipedia.org/wiki/Object_(computer_science)), whose storage is [dynamically allocated](https://en.wikipedia.org/wiki/Dynamic_memory_allocation) and deallocated in [heap memory](https://en.wikipedia.org/wiki/Heap_memory).

[Variable lifetime](https://en.wikipedia.org/wiki/Variable_lifetime) is contrasted with [scope](https://en.wikipedia.org/wiki/Scope_(computer_science)) (where a variable can be used): "global" and "local" refer to scope, not lifetime, but scope often implies lifetime. In many languages, [global variables](https://en.wikipedia.org/wiki/Global_variable) are always static, but in some languages they are dynamic, while [local variables](https://en.wikipedia.org/wiki/Local_variable) are generally automatic, but may be static.

In general, **static memory allocation** is the allocation of memory at [compile time](https://en.wikipedia.org/wiki/Compile_time), before the associated program is executed, unlike [dynamic memory allocation](https://en.wikipedia.org/wiki/Dynamic_memory_allocation) or [automatic memory allocation](https://en.wikipedia.org/wiki/Automatic_memory_allocation) where memory is allocated as required at [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)).[[1\]](https://en.wikipedia.org/wiki/Static_variable#cite_note-1)

## Addressing

The [absolute address](https://en.wikipedia.org/wiki/Absolute_address) [addressing mode](https://en.wikipedia.org/wiki/Addressing_mode) can only be used with static variables, because those are the only kinds of variables whose location is known by the compiler at compile time. When the program (executable or library) is [loaded](https://en.wikipedia.org/wiki/Loader_(computing)) into memory, static variables are stored in the [data segment](https://en.wikipedia.org/wiki/Data_segment) of the program's [address space](https://en.wikipedia.org/wiki/Memory_address#Address_space_in_application_programming) (if initialized), or the [BSS segment](https://en.wikipedia.org/wiki/BSS_segment) (if uninitialized), and are stored in corresponding sections of [object files](https://en.wikipedia.org/wiki/Object_file) prior to loading.



## Scope

See also: [Variable (computer science) § Scope and extent](https://en.wikipedia.org/wiki/Variable_(computer_science)#Scope_and_extent)

In terms of [scope and extent](https://en.wikipedia.org/wiki/Variable_(computer_science)#Scope_and_extent), static variables have extent the entire run of the program, but may have more limited [scope](https://en.wikipedia.org/wiki/Scope_(computer_science)). A basic distinction is between a *static global variable*, which has global scope and thus is in context throughout the program, and a *static local variable,* which has local scope. A static local variable is different from a local variable as a static local variable is initialized only once no matter how many times the function in which it resides is called and its value is retained and accessible through many calls to the function in which it is declared, e.g. to be used as a count variable. A static variable may also have [module scope](https://en.wikipedia.org/wiki/Module_scope) or some variant, such as [internal linkage](https://en.wikipedia.org/wiki/Internal_linkage) in [C](https://en.wikipedia.org/wiki/C_programming_language), which is a form of file scope or module scope.



### Example



An example of static local variable in C:

```C
#include <stdio.h>

void func() {
	static int x = 0; 
	/* x is initialized only once across five calls of func() and
	  the variable will get incremented five 
	  times after these calls. The final value of x will be 5. */
	x++;
	printf("%d\n", x); // outputs the value of x
}

int main() { //int argc, char *argv[] inside the main is optional in the particular program
	func(); // prints 1
	func(); // prints 2
	func(); // prints 3
	func(); // prints 4
	func(); // prints 5
	return 0;
}
```



## Object-oriented programming



In [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), there is also the concept of a *static member variable*, which is a "[class variable](https://en.wikipedia.org/wiki/Class_variable)" of a statically defined class, i.e., a [member variable](https://en.wikipedia.org/wiki/Member_variable) of a given class which is shared across all [instances](https://en.wikipedia.org/wiki/Class_instance) (objects), and is accessible as a member variable of these objects. A class variable of a dynamically defined class, in languages where classes can be defined at run time, is allocated when the class is defined and is not static.

Object constants known at compile-time, such as [string literals](https://en.wikipedia.org/wiki/String_literal), are usually allocated statically. In object-oriented programming, the [virtual method tables](https://en.wikipedia.org/wiki/Virtual_method_table) of classes are usually allocated statically. A statically defined value can also be [global](https://en.wikipedia.org/wiki/Global_variable) in its scope ensuring the same [immutable](https://en.wikipedia.org/wiki/Immutable_object) value is used throughout a run for consistency.