

## Part 3: The CPython Compiler and Execution Loop

In Part 2, you saw how the CPython interpreter takes an input, such as a file or string, and converts it into a logical **Abstract Syntax Tree**. We’re still not at the stage where this code can be executed. Next, we have to go deeper to convert the **Abstract Syntax Tree** into a set of sequential commands that the CPU can understand.



### Compiling

Now the interpreter has an AST with the properties required for each of the operations, functions, classes, and namespaces. It is the job of the compiler to turn the **AST** into something the CPU can understand.

This compilation task is split into 2 parts:

1. Traverse the **tree** and create a **control-flow-graph**, which represents the logical sequence for execution
2. Convert the **nodes** in the CFG to smaller, executable statements, known as **byte-code**



Earlier, we were looking at how files are executed, and the `PyRun_FileExFlags()` function in `Python/pythonrun.c`. Inside this function, we converted the `FILE` handle into a `mod`, of type `mod_ty`. This task was completed by [`PyParser_ASTFromFileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1369), which in turns calls the `tokenizer`, `parser-tokenizer` and then the AST:

```c
PyObject *
PyRun_FileExFlags(FILE *fp, const char *filename_str, int start, PyObject *globals,
                  PyObject *locals, int closeit, PyCompilerFlags *flags)
{
 ...
    mod = PyParser_ASTFromFileObject(fp, filename, NULL, start, 0, 0,
 ...
    ret = run_mod(mod, filename, globals, locals, flags, arena);
}
```



The resulting module from the call to is sent to [`run_mod()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1125) still in `Python/pythonrun.c`. This is a small function that gets a [`PyCodeObject`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Include/code.h#L69) from [`PyAST_CompileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L312) and sends it on to [`run_eval_code_obj()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1094). You will tackle [`run_eval_code_obj()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1094) in the next section:

```c
static PyObject *
run_mod(mod_ty mod, PyObject *filename, PyObject *globals, PyObject *locals,
            PyCompilerFlags *flags, PyArena *arena)
{
    PyCodeObject *co;
    PyObject *v;
    co = PyAST_CompileObject(mod, filename, flags, -1, arena);
    if (co == NULL)
        return NULL;

    if (PySys_Audit("exec", "O", co) < 0) {
        Py_DECREF(co);
        return NULL;
    }

    v = run_eval_code_obj(co, globals, locals);
    Py_DECREF(co);
    return v;
}
```



The [`PyAST_CompileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L312) function is the main **entry point** to the **CPython compiler**. It takes a **Python module** as its primary argument, along with the name of the file, the globals, locals, and the `PyArena` all created earlier in the interpreter process.

We’re starting to get into the guts of the CPython compiler now, with decades of development and Computer Science theory behind it. Don’t be put off by the language. Once we break down the compiler into logical steps, it’ll make sense.

Before the compiler starts, a **global compiler state** is created. This type, `compiler` is defined in `Python/compile.c` and contains properties used by the compiler to remember the compiler flags, the stack, and the `PyArena`:

```c
struct compiler {
    PyObject *c_filename;
    struct symtable *c_st;
    PyFutureFeatures *c_future; /* pointer to module's __future__ */
    PyCompilerFlags *c_flags;

    int c_optimize;              /* optimization level */
    int c_interactive;           /* true if in interactive mode */
    int c_nestlevel;
    int c_do_not_emit_bytecode;  /* The compiler won't emit any bytecode
                                    if this value is different from zero.
                                    This can be used to temporarily visit
                                    nodes without emitting bytecode to
                                    check only errors. */

    PyObject *c_const_cache;     /* Python dict holding all constants,
                                    including names tuple */
    struct compiler_unit *u; /* compiler state for current block */
    PyObject *c_stack;           /* Python list holding compiler_unit ptrs */
    PyArena *c_arena;            /* pointer to memory allocation arena */
};
```



Inside [`PyAST_CompileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L312), there are 11 main steps happening:

1. Create an empty `__doc__` property to the module if it doesn’t exist.
2. Create an empty `__annotations__` property to the module if it doesn’t exist.
3. Set the filename of the **global compiler state** to the filename argument.
4. Set the memory allocation arena for the compiler to the one used by the interpreter.
5. Copy any `__future__` flags in the module to the **future flags** in the compiler.
6. Merge **runtime flags** provided by the command-line or environment variables.
7. Enable any `__future__` features in the compiler.
8. Set the **optimization level** to the provided argument, or default.
9. Build a **symbol table** from the module object.
10. Run the compiler with the **compiler state** and return the **code object**.
11. Free any allocated memory by the compiler.



```c
PyCodeObject *
PyAST_CompileObject(mod_ty mod, PyObject *filename, PyCompilerFlags *flags,
                   int optimize, PyArena *arena)
{
    struct compiler c;
    PyCodeObject *co = NULL;
    PyCompilerFlags local_flags = _PyCompilerFlags_INIT;
    int merged;
    PyConfig *config = &_PyInterpreterState_GET_UNSAFE()->config;

    if (!__doc__) {
        __doc__ = PyUnicode_InternFromString("__doc__");
        if (!__doc__)
            return NULL;
    }
    if (!__annotations__) {
        __annotations__ = PyUnicode_InternFromString("__annotations__");
        if (!__annotations__)
            return NULL;
    }
    if (!compiler_init(&c))
        return NULL;
    Py_INCREF(filename);
    c.c_filename = filename;
    c.c_arena = arena;
    c.c_future = PyFuture_FromASTObject(mod, filename);
    if (c.c_future == NULL)
        goto finally;
    if (!flags) {
        flags = &local_flags;
    }
    merged = c.c_future->ff_features | flags->cf_flags;
    c.c_future->ff_features = merged;
    flags->cf_flags = merged;
    c.c_flags = flags;
    c.c_optimize = (optimize == -1) ? config->optimization_level : optimize;
    c.c_nestlevel = 0;
    c.c_do_not_emit_bytecode = 0;

    if (!_PyAST_Optimize(mod, arena, c.c_optimize)) {
        goto finally;
    }

    c.c_st = PySymtable_BuildObject(mod, filename, c.c_future);
    if (c.c_st == NULL) {
        if (!PyErr_Occurred())
            PyErr_SetString(PyExc_SystemError, "no symtable");
        goto finally;
    }

    co = compiler_mod(&c, mod);

 finally:
    compiler_free(&c);
    assert(co || PyErr_Occurred());
    return co;
}
```



#### Future Flags and Compiler Flags

Before the compiler runs, there are two types of flags to toggle the **features** inside the compiler. These come from two places:

1. The **interpreter state**, which may have been command-line options, set in `pyconfig.h` or via **environment variables**
2. The use of `__future__` statements inside the actual source code of the module

To distinguish the two types of flags, think that the `__future__` flags are required because of the syntax or features in that specific module. For example, Python 3.7 introduced delayed evaluation of type hints through the `annotations` future flag:

```python
from __future__ import annotations
```

The code after this statement might use unresolved type hints, so the `__future__` statement is required. Otherwise, the module wouldn’t import. It would be unmaintainable to manually request that the person importing the module enable this specific compiler flag.

The other compiler flags are specific to the environment, so they might change the way the code executes or the way the compiler runs, but they shouldn’t link to the source in the same way that `__future__` statements do.

One example of a **compiler flag** would be the [`-O` flag for optimizing the use of `assert` statements](https://docs.python.org/3/using/cmdline.html#cmdoption-o). This flag disables any `assert` statements, which may have been put in the code for [debugging purposes](https://realpython.com/python-debugging-pdb/). It can also be enabled with the `PYTHONOPTIMIZE=1` environment variable setting.





#### Symbol Tables

In [`PyAST_CompileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L312) there was a reference to a `symtable` and a call to [`PySymtable_BuildObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/symtable.c#L262) with the module to be executed.

The purpose of the **symbol table** is to provide a list of namespaces, globals, and locals for the compiler to use for referencing and resolving scopes.

> NOTE: scope在这里的含义是？

The `symtable` structure in `Include/symtable.h` is well documented, so it’s clear what each of the fields is for. There should be one **symtable instance** for the compiler, so **namespacing** becomes essential.

> NOTE: 如何实现namespace？

If you create a function called `resolve_names()` in one module and declare another function with the same name in another module, you want to be sure which one is called. The symtable serves this purpose, as well as ensuring that variables declared within a narrow scope don’t automatically become globals (after all, this isn’t JavaScript):

```c
struct symtable {
    PyObject *st_filename;          /* name of file being compiled,
                                       decoded from the filesystem encoding */
    struct _symtable_entry *st_cur; /* current symbol table entry */
    struct _symtable_entry *st_top; /* symbol table entry for module */
    PyObject *st_blocks;            /* dict: map AST node addresses
                                     *       to symbol table entries */
    PyObject *st_stack;             /* list: stack of namespace info */
    PyObject *st_global;            /* borrowed ref to st_top->ste_symbols */
    int st_nblocks;                 /* number of blocks used. kept for
                                       consistency with the corresponding
                                       compiler structure */
    PyObject *st_private;           /* name of current class or NULL */
    PyFutureFeatures *st_future;    /* module's future features that affect
                                       the symbol table */
    int recursion_depth;            /* current recursion depth */
    int recursion_limit;            /* recursion limit */
};
```

Some of the symbol table API is exposed via [the `symtable` module](https://docs.python.org/3/library/symtable.html) in the standard library. You can provide an expression or a module and receive a `symtable.SymbolTable` instance.

You can provide a string with a Python expression and the `compile_type` of `"eval"`, or a module, function or class, and the `compile_mode` of `"exec"` to get a **symbol table**.

Looping over the elements in the table we can see some of the public and private fields and their types:

```python
>>> import symtable
>>> s = symtable.symtable('b + 1', filename='test.py', compile_type='eval')
>>> [symbol.__dict__ for symbol in s.get_symbols()]
[{'_Symbol__name': 'b', '_Symbol__flags': 6160, '_Symbol__scope': 3, '_Symbol__namespaces': ()}]
```

The C code behind this is all within `Python/symtable.c` and the primary interface is the [`PySymtable_BuildObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/symtable.c#L262) function.

Similar to the top-level AST function we covered earlier, the [`PySymtable_BuildObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/symtable.c#L262) function switches between the `mod_ty` possible types (Module, Expression, Interactive, Suite, FunctionType), and visits each of the statements inside them.



Remember, `mod_ty` is an **AST instance**, so the will now recursively explore the nodes and branches of the tree and add entries to the symtable:

```c
struct symtable *
PySymtable_BuildObject(mod_ty mod, PyObject *filename, PyFutureFeatures *future)
{
    struct symtable *st = symtable_new();
    asdl_seq *seq;
    int i;
    PyThreadState *tstate;
    int recursion_limit = Py_GetRecursionLimit();
...
    st->st_top = st->st_cur;
    switch (mod->kind) {
    case Module_kind:
        seq = mod->v.Module.body;
        for (i = 0; i < asdl_seq_LEN(seq); i++)
            if (!symtable_visit_stmt(st,
                        (stmt_ty)asdl_seq_GET(seq, i)))
                goto error;
        break;
    case Expression_kind:
        ...
    case Interactive_kind:
        ...
    case Suite_kind:
        ...
    case FunctionType_kind:
        ...
    }
    ...
}
```



So for a module, [`PySymtable_BuildObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/symtable.c#L262) will loop through each statement in the module and call [`symtable_visit_stmt()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/symtable.c#L1176). The [`symtable_visit_stmt()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/symtable.c#L1176) is a huge `switch` statement with a case for each statement type (defined in `Parser/Python.asdl`).



For each statement type, there is specific logic to that statement type. For example, a function definition has particular logic for:

1. If the recursion depth is beyond the limit, raise a recursion depth error
2. The name of the function to be added as a **local variable**
3. The **default values** for sequential arguments to be resolved
4. The **default values** for keyword arguments to be resolved
5. Any annotations for the arguments or the return type are resolved
6. Any **function decorators** are resolved
7. The code block with the contents of the function is visited in [`symtable_enter_block()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/symtable.c#L973)
8. The arguments are visited
9. The body of the function is visited

> **Note:** If you’ve ever wondered why Python’s default arguments are mutable, the reason is in this function. You can see they are a **pointer** to the variable in the symtable. No extra work is done to copy any values to an immutable type.



```c
static int
symtable_visit_stmt(struct symtable *st, stmt_ty s)
{
    if (++st->recursion_depth > st->recursion_limit) {                          // 1.
        PyErr_SetString(PyExc_RecursionError,
                        "maximum recursion depth exceeded during compilation");
        VISIT_QUIT(st, 0);
    }
    switch (s->kind) {
    case FunctionDef_kind:
        if (!symtable_add_def(st, s->v.FunctionDef.name, DEF_LOCAL))            // 2.
            VISIT_QUIT(st, 0);
        if (s->v.FunctionDef.args->defaults)                                    // 3.
            VISIT_SEQ(st, expr, s->v.FunctionDef.args->defaults);
        if (s->v.FunctionDef.args->kw_defaults)                                 // 4.
            VISIT_SEQ_WITH_NULL(st, expr, s->v.FunctionDef.args->kw_defaults);
        if (!symtable_visit_annotations(st, s, s->v.FunctionDef.args,           // 5.
                                        s->v.FunctionDef.returns))
            VISIT_QUIT(st, 0);
        if (s->v.FunctionDef.decorator_list)                                    // 6.
            VISIT_SEQ(st, expr, s->v.FunctionDef.decorator_list);
        if (!symtable_enter_block(st, s->v.FunctionDef.name,                    // 7.
                                  FunctionBlock, (void *)s, s->lineno,
                                  s->col_offset))
            VISIT_QUIT(st, 0);
        VISIT(st, arguments, s->v.FunctionDef.args);                            // 8.
        VISIT_SEQ(st, stmt, s->v.FunctionDef.body);                             // 9.
        if (!symtable_exit_block(st, s))
            VISIT_QUIT(st, 0);
        break;
    case ClassDef_kind: {
        ...
    }
    case Return_kind:
        ...
    case Delete_kind:
        ...
    case Assign_kind:
        ...
    case AnnAssign_kind:
        ...
```

Once the resulting symtable has been created, it is sent back to be used for the compiler.

#### Core Compilation Process

Now that the [`PyAST_CompileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L312) has a **compiler state**, a **symtable**, and a **module** in the form of the AST, the actual compilation can begin.



The purpose of the **core compiler** is to:

- Convert the state, symtable, and AST into a [Control-Flow-Graph (CFG)](https://en.wikipedia.org/wiki/Control-flow_graph)
- Protect the execution stage from runtime exceptions by catching any logic and code errors and raising them here

You can call the CPython compiler in Python code by calling the built-in function `compile()`. It returns a `code object` instance:

```python
>>> co=compile('b+1', 'test.py', mode='eval')
<code object <module> at 0x10f222780, file "test.py", line 1>
```

The same as with the `symtable()` function, a simple expression should have a mode of `'eval'` and a module, function, or class should have a mode of `'exec'`.

The compiled code can be found in the `co_code` property of the code object:

```
>>> co.co_code
b'e\x00d\x00\x17\x00S\x00'
```

There is also a `dis` module in the standard library, which disassembles the bytecode instructions and can print them on the screen or give you a list of `Instruction` instances.

If you import `dis` and give the `dis()` function the code object’s `co_code` property it disassembles it and prints the instructions on the REPL:

```python
>>> import dis
>>> dis.dis(co.co_code)
          0 LOAD_NAME                0 (0)
          2 LOAD_CONST               0 (0)
          4 BINARY_ADD
          6 RETURN_VALUE
```

`LOAD_NAME`, `LOAD_CONST`, `BINARY_ADD`, and `RETURN_VALUE` are all bytecode instructions. They’re called bytecode because, in binary form, they were a byte long. However, since Python 3.6 the storage format was changed to a `word`, so now they’re technically wordcode, not bytecode.

The [full list of bytecode instructions](https://docs.python.org/3/library/dis.html#python-bytecode-instructions) is available for each version of Python, and it does change between versions. For example, in Python 3.7, some new bytecode instructions were introduced to speed up execution of specific method calls.

In an earlier section, we explored the `instaviz` package. This included a visualization of the code object type by running the compiler. It also displays the Bytecode operations inside the code objects.

Execute instaviz again to see the code object and bytecode for a function defined on the REPL:

```python
>>> import instaviz
>>> def example():
       a = 1
       b = a + 1
       return b
>>> instaviz.show(example)
```

If we now jump into [`compiler_mod()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L1782), a function used to switch to different compiler functions depending on the **module type**. We’ll assume that `mod` is a `Module`. The module is compiled into the **compiler state** and then [`assemble()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L5971) is run to create a [`PyCodeObject`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Include/code.h#L69).

The new code object is returned back to [`PyAST_CompileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L312) and sent on for execution:

```c
static PyCodeObject *
compiler_mod(struct compiler *c, mod_ty mod)
{
    PyCodeObject *co;
    int addNone = 1;
    static PyObject *module;
    ...
    switch (mod->kind) {
    case Module_kind:
        if (!compiler_body(c, mod->v.Module.body)) {
            compiler_exit_scope(c);
            return 0;
        }
        break;
    case Interactive_kind:
        ...
    case Expression_kind:
        ...
    case Suite_kind:
        ...
    ...
    co = assemble(c, addNone);
    compiler_exit_scope(c);
    return co;
}
```



The [`compiler_body()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L1743) function has some optimization flags and then loops over each statement in the module and visits it, similar to how the `symtable` functions worked:

```c
static int
compiler_body(struct compiler *c, asdl_seq *stmts)
{
    int i = 0;
    stmt_ty st;
    PyObject *docstring;
    ...
    for (; i < asdl_seq_LEN(stmts); i++)
        VISIT(c, stmt, (stmt_ty)asdl_seq_GET(stmts, i));
    return 1;
}
```

The statement type is determined through a call to the [`asdl_seq_GET()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Include/asdl.h#L32) function, which looks at the AST node’s type.

Through some smart macros, `VISIT` calls a function in `Python/compile.c` for each statement type:

```c
#define VISIT(C, TYPE, V) {\
    if (!compiler_visit_ ## TYPE((C), (V))) \
        return 0; \
}
```

For a `stmt` (the category for a statement) the **compiler** will then drop into [`compiler_visit_stmt()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L3310) and switch through all of the potential statement types found in `Parser/Python.asdl`:

```c
static int
compiler_visit_stmt(struct compiler *c, stmt_ty s)
{
    Py_ssize_t i, n;

    /* Always assign a lineno to the next instruction for a stmt. */
    c->u->u_lineno = s->lineno;
    c->u->u_col_offset = s->col_offset;
    c->u->u_lineno_set = 0;

    switch (s->kind) {
    case FunctionDef_kind:
        return compiler_function(c, s, 0);
    case ClassDef_kind:
        return compiler_class(c, s);
    ...
    case For_kind:
        return compiler_for(c, s);
    ...
    }

    return 1;
}
```

As an example, let’s focus on the `For` statement, in Python is the:

```python
for i in iterable:
    # block
else:  # optional if iterable is False
    # block
```

If the statement is a `For` type, it calls [`compiler_for()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L2651). There is an equivalent `compiler_*()` function for all of the statement and expression types. The more straightforward types create the bytecode instructions inline, some of the more complex statement types call other functions.

Many of the statements can have **sub-statements**. A `for` loop has a body, but you can also have complex expressions in the assignment and the iterator.

The compiler’s `compiler_` statements sends **blocks** to the **compiler state**. These blocks contain instructions, the instruction data structure in `Python/compile.c` has the opcode, any arguments, and the target block (if this is a jump instruction), it also contains the line number.

For **jump statements**, they can either be absolute or relative jump statements. **Jump statements** are used to “jump” from one operation to another. **Absolute jump statements** specify the exact operation number in the compiled code object, whereas **relative jump statements** specify the jump target relative to another operation:

```c
struct instr {
    unsigned i_jabs : 1;
    unsigned i_jrel : 1;
    unsigned char i_opcode;
    int i_oparg;
    struct basicblock_ *i_target; /* target block (if jump instruction) */
    int i_lineno;
};
```

So a frame block (of type `basicblock`), contains the following fields:

- A `b_list` pointer, the link to a list of blocks for the **compiler state**
- A list of instructions `b_instr`, with both the allocated list size `b_ialloc`, and the number used `b_iused`
- The next block after this one `b_next`
- Whether the block has been “seen” by the assembler when traversing depth-first
- If this block has a `RETURN_VALUE` opcode (`b_return`)
- The depth of the stack when this block was entered (`b_startdepth`)
- The instruction offset for the assembler

```c
typedef struct basicblock_ {
    /* Each basicblock in a compilation unit is linked via b_list in the
       reverse order that the block are allocated.  b_list points to the next
       block, not to be confused with b_next, which is next by control flow. */
    struct basicblock_ *b_list;
    /* number of instructions used */
    int b_iused;
    /* length of instruction array (b_instr) */
    int b_ialloc;
    /* pointer to an array of instructions, initially NULL */
    struct instr *b_instr;
    /* If b_next is non-NULL, it is a pointer to the next
       block reached by normal control flow. */
    struct basicblock_ *b_next;
    /* b_seen is used to perform a DFS of basicblocks. */
    unsigned b_seen : 1;
    /* b_return is true if a RETURN_VALUE opcode is inserted. */
    unsigned b_return : 1;
    /* depth of stack upon entry of block, computed by stackdepth() */
    int b_startdepth;
    /* instruction offset for block, computed by assemble_jump_offsets() */
    int b_offset;
} basicblock;
```

The `For` statement is somewhere in the middle in terms of complexity. There are 15 steps in the compilation of a `For` statement with the `for  in :` syntax:

1. Create a new code block called `start`, this allocates memory and creates a `basicblock` pointer
2. Create a new code block called `cleanup`
3. Create a new code block called `end`
4. Push a frame block of type `FOR_LOOP` to the stack with `start` as the entry block and `end` as the exit block
5. Visit the iterator expression, which adds any operations for the iterator
6. Add the `GET_ITER` operation to the compiler state
7. Switch to the `start` block
8. Call `ADDOP_JREL` which calls [`compiler_addop_j()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L1413) to add the `FOR_ITER` operation with an argument of the `cleanup` block
9. Visit the `target` and add any special code, like tuple unpacking, to the `start` block
10. Visit each statement in the body of the for loop
11. Call `ADDOP_JABS` which calls [`compiler_addop_j()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L1413) to add the `JUMP_ABSOLUTE` operation which indicates after the body is executed, jumps back to the start of the loop
12. Move to the `cleanup` block
13. Pop the `FOR_LOOP` frame block off the stack
14. Visit the statements inside the `else` section of the for loop
15. Use the `end` block

Referring back to the `basicblock` structure. You can see how in the compilation of the for statement, the various blocks are created and pushed into the compiler’s frame block and stack:

```c
static int
compiler_for(struct compiler *c, stmt_ty s)
{
    basicblock *start, *cleanup, *end;

    start = compiler_new_block(c);                       // 1.
    cleanup = compiler_new_block(c);                     // 2.
    end = compiler_new_block(c);                         // 3.
    if (start == NULL || end == NULL || cleanup == NULL)
        return 0;

    if (!compiler_push_fblock(c, FOR_LOOP, start, end))  // 4.
        return 0;

    VISIT(c, expr, s->v.For.iter);                       // 5.
    ADDOP(c, GET_ITER);                                  // 6.
    compiler_use_next_block(c, start);                   // 7.
    ADDOP_JREL(c, FOR_ITER, cleanup);                    // 8.
    VISIT(c, expr, s->v.For.target);                     // 9.
    VISIT_SEQ(c, stmt, s->v.For.body);                   // 10.
    ADDOP_JABS(c, JUMP_ABSOLUTE, start);                 // 11.
    compiler_use_next_block(c, cleanup);                 // 12.

    compiler_pop_fblock(c, FOR_LOOP, start);             // 13.

    VISIT_SEQ(c, stmt, s->v.For.orelse);                 // 14.
    compiler_use_next_block(c, end);                     // 15.
    return 1;
}
```

Depending on the type of operation, there are different arguments required. For example, we used `ADDOP_JABS` and `ADDOP_JREL` here, which refer to “**ADD** **O**peration with **J**ump to a **REL**ative position” and “**ADD** **O**peration with **J**ump to an **ABS**olute position”. This is referring to the `APPOP_JREL` and `ADDOP_JABS` macros which call `compiler_addop_j(struct compiler *c, int opcode, basicblock *b, int absolute)` and set the `absolute` argument to 0 and 1 respectively.

There are some other macros, like `ADDOP_I` calls [`compiler_addop_i()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L1383) which add an operation with an integer argument, or `ADDOP_O` calls [`compiler_addop_o()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L1345) which adds an operation with a `PyObject` argument.

Once these stages have completed, the compiler has a list of frame blocks, each containing a list of instructions and a pointer to the next block.

#### Assembly

With the compiler state, the assembler performs a “depth-first-search” of the blocks and merge the instructions into a single bytecode sequence. The assembler state is declared in `Python/compile.c`:

```c
struct assembler {
    PyObject *a_bytecode;  /* string containing bytecode */
    int a_offset;              /* offset into bytecode */
    int a_nblocks;             /* number of reachable blocks */
    basicblock **a_postorder; /* list of blocks in dfs postorder */
    PyObject *a_lnotab;    /* string containing lnotab */
    int a_lnotab_off;      /* offset into lnotab */
    int a_lineno;              /* last lineno of emitted instruction */
    int a_lineno_off;      /* bytecode offset of last lineno */
};
```

The `assemble()` function has a few tasks:

- Calculate the number of blocks for memory allocation
- Ensure that every block that falls off the end returns `None`, this is why every function returns `None`, whether or not a `return` statement exists
- Resolve any jump statements offsets that were marked as relative
- Call `dfs()` to perform a depth-first-search of the blocks
- Emit all the instructions to the compiler
- Call [`makecode()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L5854) with the compiler state to generate the `PyCodeObject`

```c
static PyCodeObject *
assemble(struct compiler *c, int addNone)
{
    basicblock *b, *entryblock;
    struct assembler a;
    int i, j, nblocks;
    PyCodeObject *co = NULL;

    /* Make sure every block that falls off the end returns None.
       XXX NEXT_BLOCK() isn't quite right, because if the last
       block ends with a jump or return b_next shouldn't set.
     */
    if (!c->u->u_curblock->b_return) {
        NEXT_BLOCK(c);
        if (addNone)
            ADDOP_LOAD_CONST(c, Py_None);
        ADDOP(c, RETURN_VALUE);
    }
    ...
    dfs(c, entryblock, &a, nblocks);

    /* Can't modify the bytecode after computing jump offsets. */
    assemble_jump_offsets(&a, c);

    /* Emit code in reverse postorder from dfs. */
    for (i = a.a_nblocks - 1; i >= 0; i--) {
        b = a.a_postorder[i];
        for (j = 0; j < b->b_iused; j++)
            if (!assemble_emit(&a, &b->b_instr[j]))
                goto error;
    }
    ...

    co = makecode(c, &a);
 error:
    assemble_free(&a);
    return co;
}
```

The depth-first-search is performed by the [`dfs()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L5397) function in `Python/compile.c`, which follows the the `b_next` pointers in each of the blocks, marks them as seen by toggling `b_seen` and then adds them to the assemblers `**a_postorder` list in reverse order.

The function loops back over the assembler’s post-order list and for each block, if it has a jump operation, recursively call [`dfs()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L5397) for that jump:

```c
static void
dfs(struct compiler *c, basicblock *b, struct assembler *a, int end)
{
    int i, j;

    /* Get rid of recursion for normal control flow.
       Since the number of blocks is limited, unused space in a_postorder
       (from a_nblocks to end) can be used as a stack for still not ordered
       blocks. */
    for (j = end; b && !b->b_seen; b = b->b_next) {
        b->b_seen = 1;
        assert(a->a_nblocks < j);
        a->a_postorder[--j] = b;
    }
    while (j < end) {
        b = a->a_postorder[j++];
        for (i = 0; i < b->b_iused; i++) {
            struct instr *instr = &b->b_instr[i];
            if (instr->i_jrel || instr->i_jabs)
                dfs(c, instr->i_target, a, j);
        }
        assert(a->a_nblocks < j);
        a->a_postorder[a->a_nblocks++] = b;
    }
}
```

#### Creating a Code Object

The task of [`makecode()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L5854) is to go through the compiler state, some of the assembler’s properties and to put these into a `PyCodeObject` by calling [`PyCode_New()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/codeobject.c#L246):





### Execution

In `Python/pythonrun.c` we broke out just before the call to [`run_eval_code_obj()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1094).

This call takes a code object, either fetched from the marshaled `.pyc` file, or compiled through the AST and compiler stages.

[`run_eval_code_obj()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1094) will pass the globals, locals, `PyArena`, and compiled `PyCodeObject` to [`PyEval_EvalCode()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/ceval.c#L716) in `Python/ceval.c`.

This stage forms the execution component of CPython. Each of the bytecode operations is taken and executed using a [“Stack Frame” based system](http://www.cs.uwm.edu/classes/cs315/Bacon/Lecture/HTML/ch10s07.html).

> **What is a Stack Frame?**
>
> Stack Frames are a data type used by many runtimes, not just Python, that allows functions to be called and variables to be returned between functions. Stack Frames also contain arguments, local variables, and other state information.
>
> Typically, a Stack Frame exists for every function call, and they are stacked in sequence. You can see CPython’s frame stack anytime an exception is unhandled and the stack is printed on the screen.



