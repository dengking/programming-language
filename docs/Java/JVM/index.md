# JVM



## wikipedia [Java virtual machine](https://en.wikipedia.org/wiki/Java_virtual_machine) 

A **Java virtual machine** (**JVM**) is a [virtual machine](https://en.wikipedia.org/wiki/Virtual_machine) that enables a computer to run [Java](https://en.wikipedia.org/wiki/Java_(software_platform)) programs as well as programs written in [other languages](https://en.wikipedia.org/wiki/List_of_JVM_languages) that are also compiled to [Java bytecode](https://en.wikipedia.org/wiki/Java_bytecode). The JVM is detailed by a [specification](https://en.wikipedia.org/wiki/Specification_(technical_standard)) ((technical standard)) that formally describes what is required in a JVM implementation. Having a specification ensures interoperability of Java programs across different implementations so that program authors using the [Java Development Kit](https://en.wikipedia.org/wiki/Java_Development_Kit) (JDK) need not worry about idiosyncrasies(个性) of the underlying hardware platform.



### JVM specification



The main reason for this omission is to not unnecessarily constrain implementers.

> NOTE:
>
> 一、"省略的主要原因是避免不必要地约束实现者。"



The data types can be divided[[6\]](https://en.wikipedia.org/wiki/Java_virtual_machine#cite_note-6) into:

- primitive types ([integers](https://en.wikipedia.org/wiki/Integer), Floating-point, long etc.) 
- Reference types.



The earlier JVM were only [32-bit](https://en.wikipedia.org/wiki/32-bit_computing) machines. `long` and `double` types, which are [64-bits](https://en.wikipedia.org/wiki/64-bit_computing), are supported natively, but consume two units of storage in a frame's local variables or **operand stack**, since each unit is 32 bits.

> NOTE:
>
> 一、**operand stack**的单位是 32 bits



Each frame provides an "operand stack" and an array of "local variables".  The **operand stack** is used for operands to run computations and for receiving the return value of a called method, while local variables serve the same purpose as [registers](https://en.wikipedia.org/wiki/Processor_register) and are also used to pass method arguments. Thus, the JVM is both a [stack machine](https://en.wikipedia.org/wiki/Stack_machine) and a [register machine](https://en.wikipedia.org/wiki/Register_machine). In practice, HotSpot eliminates every stack besides the native thread/call stack even when running in Interpreted mode, as its Templating Interpreter technically functions as a compiler.





## JVM gpt-4-vision

The Java Virtual Machine (JVM) is an abstract computing machine that enables a computer to run Java programs, as well as programs written in other languages that are also compiled to Java bytecode. The JVM is a crucial component of the Java Runtime Environment (JRE), which provides a platform-independent execution environment for Java applications.

Here are some key aspects of the JVM:

1. **Platform Independence**: One of the main advantages of the JVM is that it allows Java programs to be executed on any device or operating system that has a compatible JVM implementation. This is encapsulated in the popular Java slogan "Write Once, Run Anywhere" (WORA).

2. **Execution of Bytecode**: Java programs are compiled by the Java compiler (javac) into bytecode, which is a set of instructions that the JVM can understand and execute. The bytecode is platform-independent, which means it doesn't depend on the underlying hardware and operating system.

3. **Memory Management**: The JVM manages system memory and provides automatic garbage collection, which helps in managing the lifecycle of objects. It frees up memory by collecting and removing objects that are no longer in use.

4. **Class Loader**: The JVM includes a class loader, which dynamically loads Java classes into the runtime environment. It separates classes into namespaces and ensures that Java applications are securely isolated from each other.

5. **Execution Engine**: The execution engine is responsible for executing the instructions contained in the bytecode. It includes:
   - **Interpreter**: Reads and executes bytecode instructions one at a time.
   - **Just-In-Time (JIT) Compiler**: Improves performance by compiling bytecode into native machine code at runtime, which can be directly executed by the host CPU.

6. **Security**: The JVM enforces a set of strict security rules and safeguards to protect against malicious(恶意的) code. It includes a security manager that defines a **sandbox** in which Java applications run, limiting their abilities to perform potentially unsafe operations.

7. **Runtime Data Areas**: The JVM defines various runtime data areas that are used during the execution of a program, such as:
   - **Method Area**: Stores class structure information, including the runtime constant pool, field and method data, and the code for methods and constructors.
   - **Heap**: The runtime data area from which memory for all class instances and arrays is allocated.
   - **Java Stacks**: Each Java thread has a private JVM stack created at the same time as the thread. A stack stores frames, which contain local variables and partial results, and plays a part in method invocation and return.
   - **PC Registers**: Each JVM thread has a program counter (PC) register that indicates the current instruction being executed.

8. **Native Method Interface (JNI)**: The JNI allows Java code running in the JVM to call and be called by native applications and libraries written in other languages, such as C, C++, and assembly.

The JVM is an integral part of the Java ecosystem and continues to evolve with new features and performance enhancements in each release. It's not only used for running Java programs but also serves as a platform for other languages like Scala, Kotlin, and Clojure, which are also compiled to Java bytecode.

## JVM is both a [stack machine](https://en.wikipedia.org/wiki/Stack_machine) and a [register machine](https://en.wikipedia.org/wiki/Register_machine) 

wikipedia [Java virtual machine](https://en.wikipedia.org/wiki/Java_virtual_machine) : 

> Each frame provides an "operand stack" and an array of "local variables". The operand stack is used for operands to run computations and for receiving the return value of a called method, while local variables serve the same purpose as [registers](https://en.wikipedia.org/wiki/Processor_register) and are also used to pass method arguments. Thus, the JVM is both a [stack machine](https://en.wikipedia.org/wiki/Stack_machine) and a [register machine](https://en.wikipedia.org/wiki/Register_machine). 



### JVM [General-purpose](https://en.wikipedia.org/wiki/General-purpose_register) [Registers](https://en.wikipedia.org/wiki/Processor_register): 

Per-method operand stack (up to 65535 operands) plus per-method local variables (up to 65535)

#### gpt-4-vision JVM register

The Java Virtual Machine (JVM) does not use registers in the same way that physical CPUs do. Instead, the JVM is a stack-based virtual machine, which means that it primarily uses an operand stack for its operations rather than registers.

Here are some key points to understand about the JVM and its lack of traditional registers:

1. **Operand Stack**: The JVM uses an operand stack in each frame to perform operations. Bytecode instructions typically involve pushing operands onto the stack, performing operations, and then popping the results off the stack.

2. **Local Variables Array**: Each frame also contains an array of local variables, which acts somewhat like registers in that they provide indexed storage locations for method parameters and temporary variables. However, these are not registers in the traditional CPU sense; they are part of the memory allocated for the frame on the JVM stack.

3. **No General-Purpose Registers**: Unlike physical CPUs, which have a set of general-purpose registers for holding intermediate values during computation, the JVM does not expose any concept of registers to the bytecode level. All intermediate values are handled on the operand stack or in the local variables array.

4. **High-Level Abstraction**: The JVM is a high-level abstraction that is designed to be platform-independent. It does not expose low-level details like CPU registers to the programmer or the bytecode. This allows Java programs to be written once and run anywhere, regardless of the underlying hardware architecture.

5. **JIT Compilation and Registers**: While the JVM itself does not use registers in its bytecode execution, Just-In-Time (JIT) compilers, which are part of modern JVM implementations, do translate bytecode into native machine code that uses the physical CPU's registers. This translation is done at runtime and is transparent to the Java application.

6. **Performance**: The JVM's stack-based architecture is simpler and more portable than a register-based architecture, but it can be less efficient in terms of raw execution speed. JIT compilers help mitigate this by optimizing the translated machine code to make effective use of the CPU's registers and other features.

In summary, the JVM operates at a higher level of abstraction than physical CPUs and does not use registers in the way that machine code does. Instead, it relies on an operand stack and a local variables array within each method's frame to perform operations. The translation to machine code that uses actual CPU registers is handled by the **JIT compiler** at runtime.



#### JVM local variables 

In the Java Virtual Machine (JVM), local variables are stored in a local variable array for each method invocation. When a method is called, a new frame is created on the JVM stack, and this frame contains an array of local variables. These local variables are used to store parameters passed to the method, as well as internally declared variables within the method.

Here are some key points about JVM local variables:

1. **Indexing**: Local variables are indexed, starting from zero. The first few slots are usually reserved for the method's parameters. For instance, in an instance method, the `this` reference is stored at index 0, and the actual method parameters start from index 1. In static methods, there is no `this`, so parameters start from index 0.

2. **Types**: The local variable array can hold different types of variables, including `boolean`, `byte`, `char`, `short`, `int`, `float`, `long`, `double`, and references to objects or arrays. Note that `long` and `double` types occupy two consecutive slots in the array because they are 64-bit values.

3. **Scope**: The scope of a local variable is limited to the method in which it is declared. Once the method invocation is complete and its frame is popped off the stack, the local variables are no longer accessible and are subject to garbage collection if there are no other references to the objects they may have been pointing to.

4. **Access**: Local variables are accessed using `load` and `store` instructions in the bytecode. For example, `iload_0` will load the `int` value from the first local variable slot onto the operand stack, and `istore_1` will store the top `int` value from the operand stack into the second local variable slot.

5. **Initialization**: Unlike class fields, local variables are not automatically initialized to any default value. They must be explicitly initialized in the code before use.

6. **Visibility**: Local variables are not visible outside the method, which means they are inherently thread-safe since they are not shared between threads. Each thread has its own stack, and thus its own set of local variable arrays for each method invocation.

7. **Efficiency**: Access to local variables is very fast because they are indexed and stored in an array that is part of the method's stack frame, which is a highly efficient data structure.

Understanding how local variables work in the JVM is important for Java developers, especially when it comes to understanding method invocation, stack management, and variable scoping. It also plays a role in understanding performance aspects and how the JVM executes Java bytecode.

### JVM stack machine

#### JVM operand stack

[stackoverflow-What is an operand stack?](https://stackoverflow.com/questions/24427056/what-is-an-operand-stack)



## Java bytecode

https://en.wikipedia.org/wiki/List_of_Java_bytecode_instructions

https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.aload

https://en.wikipedia.org/wiki/Java_bytecode



### JVM load bytecodes

In the Java Virtual Machine (JVM), load bytecodes are a category of instructions used to load values from local variables or arrays onto the operand stack. The operand stack is where the JVM performs operations, so before any operation can be done on a variable, it must be loaded onto the stack.

Here are some of the common load bytecodes in the JVM:

1. **Local Variable Load Instructions**:
   - `iload`: Load an `int` value from a local variable onto the stack.
   - `lload`: Load a `long` value from a local variable onto the stack.
   - `fload`: Load a `float` value from a local variable onto the stack.
   - `dload`: Load a `double` value from a local variable onto the stack.
   - `aload`: Load a reference (could be an object, array, or returnAddress) from a local variable onto the stack.

   Each of these instructions has a variant that includes the index of the local variable as part of the instruction (e.g., `iload_0` to load from the first local variable slot).

2. **Array Load Instructions**:
   - `iaload`: Load an `int` from an array.
   - `laload`: Load a `long` from an array.
   - `faload`: Load a `float` from an array.
   - `daload`: Load a `double` from an array.
   - `aaload`: Load a reference from an array.
   - `baload`: Load a `byte` or `boolean` from an array.
   - `caload`: Load a `char` from an array.
   - `saload`: Load a `short` from an array.

These instructions are used in conjunction with other instructions that manipulate the stack, perform operations, and store results back into local variables or arrays.

To see these bytecodes in action, you can compile a simple Java program and then use the `javap` tool with the `-c` option to disassemble the `.class` file and view the bytecode instructions. For example:

```java
public class LoadExample {
    public int add(int a, int b) {
        return a + b;
    }
}
```

After compiling this class, you can run `javap -c LoadExample` to see the bytecode, which will include `iload` instructions to load the method parameters onto the stack before performing the addition.

Understanding these load instructions is important for anyone interested in Java bytecode, JVM internals, or performance optimization, as they are fundamental to how the JVM executes Java code.



### JVM store bytecodes

In the Java Virtual Machine (JVM), store bytecodes are a category of instructions used to store values from the operand stack into local variables or arrays. After performing operations on the operand stack, the results are often stored back into local variables or array elements for later use.

Here are some of the common store bytecodes in the JVM:

1. **Local Variable Store Instructions**:
   - `istore`: Store `int` value into a local variable.
   - `lstore`: Store `long` value into a local variable.
   - `fstore`: Store `float` value into a local variable.
   - `dstore`: Store `double` value into a local variable.
   - `astore`: Store reference (`object`, `array`, or `returnAddress`) into a local variable.

   Similar to load instructions, each of these has a variant that includes the index of the local variable as part of the instruction (e.g., `istore_0` to store into the first local variable slot).

2. **Array Store Instructions**:
   - `iastore`: Store `int` into an array.
   - `lastore`: Store `long` into an array.
   - `fastore`: Store `float` into an array.
   - `dastore`: Store `double` into an array.
   - `aastore`: Store reference into an array.
   - `bastore`: Store `byte` or `boolean` into an array.
   - `castore`: Store `char` into an array.
   - `sastore`: Store `short` into an array.

These instructions are used after computations have been done and the results need to be stored for later use. The store instructions take the value from the top of the operand stack and place it into the specified local variable or array element.

To see these bytecodes in action, you can compile a Java program and then use the `javap` tool with the `-c` option to disassemble the `.class` file and view the bytecode instructions. For example:

```java
public class StoreExample {
    public void setValues(int[] array, int index, int value) {
        array[index] = value;
    }
}
```

After compiling this class, you can run `javap -c StoreExample` to see the bytecode, which will include `iastore` instruction to store the `int` value into the array at the specified index.

Understanding store instructions is crucial for anyone delving into Java bytecode, JVM internals, or performance optimization, as they are key to how the JVM handles variable assignment and manipulation.