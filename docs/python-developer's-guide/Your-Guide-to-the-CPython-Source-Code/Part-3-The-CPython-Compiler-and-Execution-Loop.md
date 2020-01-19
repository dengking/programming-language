

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
5. Copy any `__future__` flags in the module to the future flags in the compiler.
6. Merge runtime flags provided by the command-line or environment variables.
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

One example of a compiler flag would be the [`-O` flag for optimizing the use of `assert` statements](https://docs.python.org/3/using/cmdline.html#cmdoption-o). This flag disables any `assert` statements, which may have been put in the code for [debugging purposes](https://realpython.com/python-debugging-pdb/). It can also be enabled with the `PYTHONOPTIMIZE=1` environment variable setting.





#### Symbol Tables

In [`PyAST_CompileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/compile.c#L312) there was a reference to a `symtable` and a call to [`PySymtable_BuildObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/symtable.c#L262) with the module to be executed.

The purpose of the **symbol table** is to provide a list of namespaces, globals, and locals for the compiler to use for referencing and resolving scopes.

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



