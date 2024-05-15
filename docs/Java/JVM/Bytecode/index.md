# Java bytecode



https://en.wikipedia.org/wiki/List_of_Java_bytecode_instructions

https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.aload

https://en.wikipedia.org/wiki/Java_bytecode



Each bytecode operation in the JVM is represented by a single byte, hence the name "[bytecode](https://en.wikipedia.org/wiki/Bytecode)", making it a compact form of instruction.

> NOTE:
>
> 一、



## JVM load bytecodes

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



## JVM store bytecodes

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

