

## Part 2: The Python Interpreter Process

Now that you’ve seen the Python grammar and memory management, you can follow the process from typing `python` to the part where your code is executed.

There are five ways the `python` binary can be called:

1. To run a single command with `-c` and a Python command
2. To start a module with `-m` and the name of a module
3. To run a file with the filename
4. To run the `stdin` input using a shell pipe
5. To start the REPL and execute commands one at a time

> Python has so many ways to execute scripts, it can be a little overwhelming. Darren Jones has put together a [great course on running Python scripts](https://realpython.com/courses/running-python-scripts/) if you want to learn more.

The three source files you need to inspect to see this process are:

1. **`Programs/python.c`** is a simple entry point.
2. **`Modules/main.c`** contains the code to bring together the whole process, loading configuration, executing code and clearing up memory.
3. **`Python/initconfig.c`** loads the configuration from the system environment and merges it with any command-line flags.



This diagram shows how each of those functions is called:

[![Python run swim lane diagram](https://files.realpython.com/media/swim-lanes-chart-1.9fb3000aad85.png)](https://files.realpython.com/media/swim-lanes-chart-1.9fb3000aad85.png)

The execution mode is determined from the configuration.

> **The CPython source code style:**
>
> Similar to the [PEP8 style guide for Python code](https://realpython.com/courses/writing-beautiful-python-code-pep-8/), there is an [official style guide](https://www.python.org/dev/peps/pep-0007/) for the CPython C code, designed originally in 2001 and updated for modern versions.
>
> There are some naming standards which help when navigating the source code:
>
> - Use a `Py` prefix for public functions, never for static functions. The `Py_` prefix is reserved for global service routines like `Py_FatalError`. Specific groups of routines (like specific object type APIs) use a longer prefix, such as `PyString_` for string functions.
> - Public functions and variables use MixedCase with underscores, like this: [`PyObject_GetAttr`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/object.c#L924), [`Py_BuildValue`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Include/modsupport.h#L20), `PyExc_TypeError`.
> - Occasionally an “internal” function has to be visible to the loader. We use the `_Py` prefix for this, for example, [`_PyObject_Dump`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/object.c#L464).
> - Macros should have a MixedCase prefix and then use upper case, for example `PyString_AS_STRING`, `Py_PRINT_RAW`.

### Establishing Runtime Configuration

In the swimlanes, you can see that before any Python code is executed, the runtime first establishes the configuration. The **configuration of the runtime** is a data structure defined in `Include/cpython/initconfig.h` named [`PyConfig`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Include/cpython/initconfig.h#L407).

The configuration data structure includes things like:

- **Runtime flags** for various modes like debug and optimized mode
- The **execution mode**, such as whether a filename was passed, `stdin` was provided or a module name
- **Extended option**, specified by `-X `
- **Environment variables** for runtime settings

The configuration data is primarily used by the CPython runtime to enable and disable various features.

Python also comes with several [Command Line Interface Options](https://docs.python.org/3/using/cmdline.html). In Python you can enable verbose mode with the `-v` flag. In verbose mode, Python will print messages to the screen when modules are loaded:

```
python -v -c "print('hello world')"
```

You will see a hundred lines or more with all the imports of your user site-packages and anything else in the system environment.

You can see the definition of this flag within `Include/cpython/initconfig.h` inside the `struct` for [`PyConfig`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Include/cpython/initconfig.h#L407):

```c
/* --- PyConfig ---------------------------------------------- */

typedef struct {
    int _config_version;  /* Internal configuration version,
                             used for ABI compatibility */
    int _config_init;     /* _PyConfigInitEnum value */

    ...

    /* If greater than 0, enable the verbose mode: print a message each time a
       module is initialized, showing the place (filename or built-in module)
       from which it is loaded.

       If greater or equal to 2, print a message for each file that is checked
       for when searching for a module. Also provides information on module
       cleanup at exit.

       Incremented by the -v option. Set by the PYTHONVERBOSE environment
       variable. If set to -1 (default), inherit Py_VerboseFlag value. */
    int verbose;
```

In `Python/initconfig.c`, the logic for reading settings from environment variables and runtime command-line flags is established.

In the `config_read_env_vars` function, the environment variables are read and used to assign the values for the configuration settings:

```c
static PyStatus
config_read_env_vars(PyConfig *config)
{
    PyStatus status;
    int use_env = config->use_environment;

    /* Get environment variables */
    _Py_get_env_flag(use_env, &config->parser_debug, "PYTHONDEBUG");
    _Py_get_env_flag(use_env, &config->verbose, "PYTHONVERBOSE");
    _Py_get_env_flag(use_env, &config->optimization_level, "PYTHONOPTIMIZE");
    _Py_get_env_flag(use_env, &config->inspect, "PYTHONINSPECT");
```

For the verbose setting, you can see that the value of `PYTHONVERBOSE` is used to set the value of `&config->verbose`, if `PYTHONVERBOSE` is found. If the environment variable does not exist, then the default value of `-1` will remain.

Then in [`config_parse_cmdline`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/initconfig.c#L1828) within `initconfig.c` again, the command-line flag is used to set the value, if provided:

```c
static PyStatus
config_parse_cmdline(PyConfig *config, PyWideStringList *warnoptions,
                     Py_ssize_t *opt_index)
{
...

        switch (c) {
...

        case 'v':
            config->verbose++;
            break;
...
        /* This space reserved for other options */

        default:
            /* unknown argument: parsing failed */
            config_usage(1, program);
            return _PyStatus_EXIT(2);
        }
    } while (1);
```

This value is later copied to a global variable `Py_VerboseFlag` by the [`_Py_GetGlobalVariablesAsDict`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/initconfig.c#L134) function.

Within a Python session, you can access the runtime flags, like verbose mode, quiet mode, using the `sys.flags` named tuple. The `-X` flags are all available inside the `sys._xoptions` dictionary:

```
$ ./python.exe -X dev -q       

>>> import sys
>>> sys.flags
sys.flags(debug=0, inspect=0, interactive=0, optimize=0, dont_write_bytecode=0, 
 no_user_site=0, no_site=0, ignore_environment=0, verbose=0, bytes_warning=0, 
 quiet=1, hash_randomization=1, isolated=0, dev_mode=True, utf8_mode=0)

>>> sys._xoptions
{'dev': True}
```

As well as the **runtime configuration** in `initconfig.h`, there is also the **build configuration**, which is located inside `pyconfig.h` in the root folder. This file is created dynamically in the `configure` step in the build process, or by Visual Studio for Windows systems.

You can see the **build configuration** by running:

```
$ ./python.exe -m sysconfig
```

### Reading Files/Input

Once CPython has the runtime configuration and the command-line arguments, it can establish what it needs to execute.

This task is handled by the [`pymain_main`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Modules/main.c#L665) function inside `Modules/main.c`. Depending on the newly created `config` instance, CPython will now execute code provided via several options.



#### Input via `-c`

The simplest is providing CPython a command with the `-c` option and a Python program inside quotes.

For example:

```
$ ./python.exe -c "print('hi')"
hi
```

Here is the full flowchart of how this happens:

![Flow chart of pymain_run_command](https://files.realpython.com/media/pymain_run_command.f5da561ba7d5.png)



First, the [`pymain_run_command()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Modules/main.c#L240) function is executed inside `Modules/main.c` taking the command passed in `-c` as an argument in the C type `wchar_t*`. The `wchar_t*` type is often used as a low-level storage type for **Unicode data** across CPython as the size of the type can store UTF8 characters.

When converting the `wchar_t*` to a Python string, the `Objects/unicodeobject.c` file has a helper function [`PyUnicode_FromWideChar()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/unicodeobject.c#L2088) that returns a `PyObject`, of type `str`. The encoding to UTF8 is then done by `PyUnicode_AsUTF8String()` on the Python `str` object to convert it to a Python `bytes` object.

Once this is complete, [`pymain_run_command()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Modules/main.c#L240) will then pass the Python bytes object to [`PyRun_SimpleStringFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L453) for execution, but first converting the `bytes` to a `str` type again:

```c
static int
pymain_run_command(wchar_t *command, PyCompilerFlags *cf)
{
    PyObject *unicode, *bytes;
    int ret;

    unicode = PyUnicode_FromWideChar(command, -1);
    if (unicode == NULL) {
        goto error;
    }

    if (PySys_Audit("cpython.run_command", "O", unicode) < 0) {
        return pymain_exit_err_print();
    }

    bytes = PyUnicode_AsUTF8String(unicode);
    Py_DECREF(unicode);
    if (bytes == NULL) {
        goto error;
    }

    ret = PyRun_SimpleStringFlags(PyBytes_AsString(bytes), cf);
    Py_DECREF(bytes);
    return (ret != 0);

error:
    PySys_WriteStderr("Unable to decode the command from the command line:\n");
    return pymain_exit_err_print();
}
```

The conversion of `wchar_t*` to Unicode, bytes, and then a string is roughly equivalent to the following:

```python
unicode = str(command)
bytes_ = bytes(unicode.encode('utf8'))
# call PyRun_SimpleStringFlags with bytes_
```

The [`PyRun_SimpleStringFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L453) function is part of `Python/pythonrun.c`. It’s purpose is to turn this simple command into a **Python module** and then send it on to be executed. Since a **Python module** needs to have `__main__` to be executed as a standalone module, it creates that automatically:

```c
int
PyRun_SimpleStringFlags(const char *command, PyCompilerFlags *flags)
{
    PyObject *m, *d, *v;
    m = PyImport_AddModule("__main__");
    if (m == NULL)
        return -1;
    d = PyModule_GetDict(m);
    v = PyRun_StringFlags(command, Py_file_input, d, d, flags);
    if (v == NULL) {
        PyErr_Print();
        return -1;
    }
    Py_DECREF(v);
    return 0;
}
```

Once [`PyRun_SimpleStringFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L453) has created a module and a dictionary, it calls [`PyRun_StringFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1008), which creates a fake filename and then calls the **Python parser** to create an **AST** from the string and return a **module**, `mod`:

```c
PyObject *
PyRun_StringFlags(const char *str, int start, PyObject *globals,
                  PyObject *locals, PyCompilerFlags *flags)
{
...
    mod = PyParser_ASTFromStringObject(str, filename, start, flags, arena);
    if (mod != NULL)
        ret = run_mod(mod, filename, globals, locals, flags, arena);
    PyArena_Free(arena);
    return ret;
```

You’ll dive into the AST and Parser code in the next section.

> NOTE: command string-->python module-->AST



#### Input via `-m`

Another way to execute Python commands is by using the `-m` option with the name of a module. A typical example is `python -m unittest` to run the unittest module in the standard library.

Being able to execute modules as scripts were initially proposed in [PEP 338](https://www.python.org/dev/peps/pep-0338) and then the standard for explicit relative imports defined in [PEP366](https://www.python.org/dev/peps/pep-0366).

The use of the `-m` flag implies that within the module package, you want to execute whatever is inside [`__main__`](https://realpython.com/python-main-function/). It also implies that you want to search `sys.path` for the named module.

This search mechanism is why you don’t need to remember where the `unittest` module is stored on your filesystem.

Inside `Modules/main.c` there is a function called when the command-line is run with the `-m` flag. The name of the module is passed as the `modname` argument.

CPython will then import a standard library module, `runpy` and execute it using [`PyObject_Call()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/call.c#L214). The import is done using the C API function [`PyImport_ImportModule()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/import.c#L1409), found within the `Python/import.c` file:

```c
static int
pymain_run_module(const wchar_t *modname, int set_argv0)
{
    PyObject *module, *runpy, *runmodule, *runargs, *result;
    runpy = PyImport_ImportModule("runpy");
 ...
    runmodule = PyObject_GetAttrString(runpy, "_run_module_as_main");
 ...
    module = PyUnicode_FromWideChar(modname, wcslen(modname));
 ...
    runargs = Py_BuildValue("(Oi)", module, set_argv0);
 ...
    result = PyObject_Call(runmodule, runargs, NULL);
 ...
    if (result == NULL) {
        return pymain_exit_err_print();
    }
    Py_DECREF(result);
    return 0;
}
```

In this function you’ll also see 2 other C API functions: [`PyObject_Call()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/call.c#L214) and [`PyObject_GetAttrString()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/object.c#L831). Because [`PyImport_ImportModule()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/import.c#L1409) returns a `PyObject*`, the core object type, you need to call special functions to get attributes and to call it.

In Python, if you had an object and wanted to get an attribute, then you could call `getattr()`. In the C API, this call is [`PyObject_GetAttrString()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Objects/object.c#L831), which is found in `Objects/object.c`. If you wanted to run a callable, you would give it parentheses, or you can run the `__call__()` property on any Python object. The `__call__()` method is implemented inside `Objects/object.c`:

```python
hi = "hi!"
hi.upper() == hi.upper.__call__()  # this is the same
```

The `runpy` module is written in pure Python and located in `Lib/runpy.py`.

Executing `python -m ` is equivalent to running `python -m runpy `. The `runpy` module was created to abstract the process of locating and executing modules on an operating system.

`runpy` does a few things to run the target module:

- Calls `__import__()` for the module name you provided
- Sets `__name__` (the module name) to a namespace called `__main__`
- Executes the module within the `__main__` namespace

The `runpy` module also supports executing directories and zip files.



#### Input via Filename

If the first argument to `python` was a filename, such as `python test.py`, then CPython will open a file handle, similar to using `open()` in Python and pass the handle to [`PyRun_SimpleFileExFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L372) inside `Python/pythonrun.c`.

There are 3 paths this function can take:

1. If the file path is a `.pyc` file, it will call [`run_pyc_file()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1145).
2. If the file path is a script file (`.py`) it will run [`PyRun_FileExFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1032).
3. If the filepath is `stdin` because the user ran `command | python` then treat `stdin` as a file handle and run [`PyRun_FileExFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1032).

```c
int
PyRun_SimpleFileExFlags(FILE *fp, const char *filename, int closeit,
                        PyCompilerFlags *flags)
{
 ...
    m = PyImport_AddModule("__main__");
 ...
    if (maybe_pyc_file(fp, filename, ext, closeit)) {
 ...
        v = run_pyc_file(pyc_fp, filename, d, d, flags);
    } else {
        /* When running from stdin, leave __main__.__loader__ alone */
        if (strcmp(filename, "<stdin>") != 0 &&
            set_main_loader(d, filename, "SourceFileLoader") < 0) {
            fprintf(stderr, "python: failed to set __main__.__loader__\n");
            ret = -1;
            goto done;
        }
        v = PyRun_FileExFlags(fp, filename, Py_file_input, d, d,
                              closeit, flags);
    }
 ...
    return ret;
}
```



#### Input via File With `PyRun_FileExFlags()`

For `stdin` and basic script files, CPython will pass the file handle to [`PyRun_FileExFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1032) located in the `pythonrun.c` file.

The purpose of [`PyRun_FileExFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1032) is similar to [`PyRun_SimpleStringFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L453) used for the `-c` input. CPython will load the file handle into [`PyParser_ASTFromFileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1369). We’ll cover the Parser and AST modules in the next section. Because this is a full script, it doesn’t need the `PyImport_AddModule("__main__");` step used by `-c`:

```c
PyObject *
PyRun_FileExFlags(FILE *fp, const char *filename_str, int start, PyObject *globals,
                  PyObject *locals, int closeit, PyCompilerFlags *flags)
{
 ...
    mod = PyParser_ASTFromFileObject(fp, filename, NULL, start, 0, 0,
                                     flags, NULL, arena);
 ...
    ret = run_mod(mod, filename, globals, locals, flags, arena);
}
```

Identical to [`PyRun_SimpleStringFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L453), once [`PyRun_FileExFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1032) has created a Python module from the file, it sent it to [`run_mod()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1125) to be executed.

[`run_mod()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1125) is found within `Python/pythonrun.c`, and sends the **module** to the AST to be compiled into a **code object**. **Code objects** are a format used to store the **bytecode** operations and the format kept in `.pyc` files:

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

We will cover the CPython compiler and bytecodes in the next section. The call to [`run_eval_code_obj()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1094) is a simple wrapper function that calls [`PyEval_EvalCode()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/ceval.c#L716) in the `Python/eval.c` file. The [`PyEval_EvalCode()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/ceval.c#L716) function is the **main evaluation loop** for CPython, it iterates over each bytecode statement and executes it on your local machine.



#### Input via Compiled Bytecode With `run_pyc_file()`

In the [`PyRun_SimpleFileExFlags()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L372) there was a clause for the user providing a file path to a `.pyc` file. If the file path ended in `.pyc` then instead of loading the file as a plain text file and parsing it, it will assume that the `.pyc` file contains a code object written to disk.

The [`run_pyc_file()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1145) function inside `Python/pythonrun.c` then marshals the code object from the `.pyc` file by using the file handle. **Marshaling** is a technical term for copying the contents of a file into memory and converting them to a specific data structure. The code object data structure on the disk is the CPython compiler’s way to caching compiled code so that it doesn’t need to parse it every time the script is called:

```c
static PyObject *
run_pyc_file(FILE *fp, const char *filename, PyObject *globals,
             PyObject *locals, PyCompilerFlags *flags)
{
    PyCodeObject *co;
    PyObject *v;
  ...
    v = PyMarshal_ReadLastObjectFromFile(fp);
  ...
    if (v == NULL || !PyCode_Check(v)) {
        Py_XDECREF(v);
        PyErr_SetString(PyExc_RuntimeError,
                   "Bad code object in .pyc file");
        goto error;
    }
    fclose(fp);
    co = (PyCodeObject *)v;
    v = run_eval_code_obj(co, globals, locals);
    if (v && flags)
        flags->cf_flags |= (co->co_flags & PyCF_MASK);
    Py_DECREF(co);
    return v;
}
```

Once the code object has been marshaled to memory, it is sent to [`run_eval_code_obj()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1094), which calls `Python/ceval.c` to execute the code.



> NOTE: [Marshalling (computer science)](https://en.wikipedia.org/wiki/Marshalling_(computer_science))



### Lexing and Parsing

In the exploration of reading and executing Python files, we dived as deep as the parser and AST modules, with function calls to [`PyParser_ASTFromFileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1369).

Sticking within `Python/pythonrun.c`, the [`PyParser_ASTFromFileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/pythonrun.c#L1369) function will take a file handle, compiler flags and a `PyArena` instance and convert the file object into a **node object** using [`PyParser_ParseFileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Parser/parsetok.c#L163).

With the **node object**, it will then convert that into a module using the AST function [`PyAST_FromNodeObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/ast.c#L772):

```c
mod_ty
PyParser_ASTFromFileObject(FILE *fp, PyObject *filename, const char* enc,
                           int start, const char *ps1,
                           const char *ps2, PyCompilerFlags *flags, int *errcode,
                           PyArena *arena)
{
    ...
    node *n = PyParser_ParseFileObject(fp, filename, enc,
                                       &_PyParser_Grammar,
                                       start, ps1, ps2, &err, &iflags);
    ...
    if (n) {
        flags->cf_flags |= iflags & PyCF_MASK;
        mod = PyAST_FromNodeObject(n, flags, filename, arena);
        PyNode_Free(n);
    ...
    return mod;
}
```

For [`PyParser_ParseFileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Parser/parsetok.c#L163) we switch to `Parser/parsetok.c` and the parser-tokenizer stage of the CPython interpreter. This function has two important tasks:

1. Instantiate a tokenizer state [`tok_state`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Parser/tokenizer.h#L23) using [`PyTokenizer_FromFile()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Parser/tokenizer.h#L78) in `Parser/tokenizer.c`
2. Convert the tokens into a **concrete parse tree** (a list of `node`) using [`parsetok()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Parser/parsetok.c#L232) in `Parser/parsetok.c`

```c
node *
PyParser_ParseFileObject(FILE *fp, PyObject *filename,
                         const char *enc, grammar *g, int start,
                         const char *ps1, const char *ps2,
                         perrdetail *err_ret, int *flags)
{
    struct tok_state *tok;
...
    if ((tok = PyTokenizer_FromFile(fp, enc, ps1, ps2)) == NULL) {
        err_ret->error = E_NOMEM;
        return NULL;
    }
...
    return parsetok(tok, g, start, err_ret, flags);
}
```

`tok_state` (defined in `Parser/tokenizer.h`) is the data structure to store all temporary data generated by the tokenizer. It is returned to the parser-tokenizer as the data structure is required by [`parsetok()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Parser/parsetok.c#L232) to develop the **concrete syntax tree**.

Inside [`parsetok()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Parser/parsetok.c#L232), it will use the `tok_state` structure and make calls to [`tok_get()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Parser/tokenizer.c#L1110) in a loop until the file is exhausted and no more tokens can be found.

[`tok_get()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Parser/tokenizer.c#L1110), defined in `Parser/tokenizer.c` behaves like an iterator. It will keep returning the next token in the parse tree.

[`tok_get()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Parser/tokenizer.c#L1110) is one of the most complex functions in the whole CPython codebase. It has over 640 lines and includes decades of heritage with edge cases, new language features, and syntax.

One of the simpler examples would be the part that converts a newline break into a NEWLINE token:

```c
static int
tok_get(struct tok_state *tok, char **p_start, char **p_end)
{
...
    /* Newline */
    if (c == '\n') {
        tok->atbol = 1;
        if (blankline || tok->level > 0) {
            goto nextline;
        }
        *p_start = tok->start;
        *p_end = tok->cur - 1; /* Leave '\n' out of the string */
        tok->cont_line = 0;
        if (tok->async_def) {
            /* We're somewhere inside an 'async def' function, and
               we've encountered a NEWLINE after its signature. */
            tok->async_def_nl = 1;
        }
        return NEWLINE;
    }
...
}
```

In this case, `NEWLINE` is a token, with a value defined in `Include/token.h`. All tokens are constant `int` values, and the `Include/token.h` file was generated earlier when we ran `make regen-grammar`.

The `node` type returned by [`PyParser_ParseFileObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Parser/parsetok.c#L163) is going to be essential for the next stage, converting a **parse tree** into an **Abstract-Syntax-Tree** (AST):

```c
typedef struct _node {
    short               n_type;
    char                *n_str;
    int                 n_lineno;
    int                 n_col_offset;
    int                 n_nchildren;
    struct _node        *n_child;
    int                 n_end_lineno;
    int                 n_end_col_offset;
} node;
```

Since the CST is a tree of syntax, token IDs, and symbols, it would be difficult for the compiler to make quick decisions based on the Python language.

That is why the next stage is to convert the **CST** into an **AST**, a much higher-level structure. This task is performed by the `Python/ast.c` module, which has both a C and Python API.

Before you jump into the AST, there is a way to access the output from the parser stage. CPython has a standard library module `parser`, which exposes the C functions with a Python API.

The module is documented as an implementation detail of CPython so that you won’t see it in other Python interpreters. Also the output from the functions is not that easy to read.

The output will be in the numeric form, using the token and symbol numbers generated by the `make regen-grammar` stage, stored in `Include/token.h`:

```
>>> from pprint import pprint
>>> import parser
>>> st = parser.expr('a + 1')
>>> pprint(parser.st2list(st))
[258,
 [332,
  [306,
   [310,
    [311,
     [312,
      [313,
       [316,
        [317,
         [318,
          [319,
           [320,
            [321, [322, [323, [324, [325, [1, 'a']]]]]],
            [14, '+'],
            [321, [322, [323, [324, [325, [2, '1']]]]]]]]]]]]]]]]],
 [4, ''],
 [0, '']]
```

To make it easier to understand, you can take all the numbers in the `symbol` and `token` modules, put them into a dictionary and recursively replace the values in the output of `parser.st2list()` with the names:

```python
import symbol
import token
import parser

def lex(expression):
    symbols = {v: k for k, v in symbol.__dict__.items() if isinstance(v, int)}
    tokens = {v: k for k, v in token.__dict__.items() if isinstance(v, int)}
    lexicon = {**symbols, **tokens}
    st = parser.expr(expression)
    st_list = parser.st2list(st)

    def replace(l: list):
        r = []
        for i in l:
            if isinstance(i, list):
                r.append(replace(i))
            else:
                if i in lexicon:
                    r.append(lexicon[i])
                else:
                    r.append(i)
        return r

    return replace(st_list)
```

You can run `lex()` with a simple expression, like `a + 1` to see how this is represented as a parser-tree:

```
>>> from pprint import pprint
>>> pprint(lex('a + 1'))

['eval_input',
 ['testlist',
  ['test',
   ['or_test',
    ['and_test',
     ['not_test',
      ['comparison',
       ['expr',
        ['xor_expr',
         ['and_expr',
          ['shift_expr',
           ['arith_expr',
            ['term',
             ['factor', ['power', ['atom_expr', ['atom', ['NAME', 'a']]]]]],
            ['PLUS', '+'],
            ['term',
             ['factor',
              ['power', ['atom_expr', ['atom', ['NUMBER', '1']]]]]]]]]]]]]]]]],
 ['NEWLINE', ''],
 ['ENDMARKER', '']]
```

In the output, you can see the symbols in lowercase, such as `'test'` and the tokens in uppercase, such as `'NUMBER'`.

### Abstract Syntax Trees

The next stage in the CPython interpreter is to convert the CST generated by the parser into something more logical that can be executed. The structure is a higher-level representation of the code, called an Abstract Syntax Tree (AST).

ASTs are produced inline with the CPython interpreter process, but you can also generate them in both Python using the `ast` module in the Standard Library as well as through the C API.

Before diving into the C implementation of the AST, it would be useful to understand what an AST looks like for a simple piece of Python code.

To do this, here’s a simple app called `instaviz` for this tutorial. It displays the AST and bytecode instructions (which we’ll cover later) in a Web UI.

To install `instaviz`:

```
pip install instaviz
```

Then, open up a REPL by running `python` at the command line with no arguments:

```python
>>> import instaviz
>>> def example():
       a = 1
       b = a + 1
       return b

>>> instaviz.show(example)
```

You’ll see a notification on the command-line that a web server has started on port `8080`. If you were using that port for something else, you can change it by calling `instaviz.show(example, port=9090)` or another port number.

In the web browser, you can see the detailed breakdown of your function:

![Instaviz screenshot](https://files.realpython.com/media/screenshot.e148c89e3a9a.png)

The bottom left graph is the function you declared in REPL, represented as an Abstract Syntax Tree. Each node in the tree is an AST type. They are found in the `ast` module, and all inherit from `_ast.AST`.

Some of the nodes have properties which link them to child nodes, unlike the CST, which has a generic child node property.

For example, if you click on the Assign node in the center, this links to the line `b = a + 1`:

[![Instaviz screenshot 2](https://files.realpython.com/media/Screen_Shot_2019-03-19_at_1.24.17_pm.a5df8d873988.png)](https://files.realpython.com/media/Screen_Shot_2019-03-19_at_1.24.17_pm.a5df8d873988.png)

It has two properties:

1. **`targets`** is a list of names to assign. It is a list because you can assign to multiple variables with a single expression using unpacking
2. **`value`** is the value to assign, which in this case is a `BinOp` statement, `a + 1`.

If you click on the `BinOp` statement, it shows the properties of relevance:

- **`left`:** the node to the left of the operator
- **`op`:** the operator, in this case, an `Add` node (`+`) for addition
- **`right`:** the node to the right of the operator

![Instaviz screenshot 3](https://files.realpython.com/media/Screen_Shot_2019-03-19_at_1.24.37_pm.21a11b49a820.png)

Compiling an AST in C is not a straightforward task, so the `Python/ast.c` module is over 5000 lines of code.

There are a few **entry points**, forming part of the AST’s public API. In the last section on the lexer and parser, you stopped when you’d reached the call to [`PyAST_FromNodeObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/ast.c#L772). By this stage, the Python interpreter process had created a CST in the format of `node *` tree.

Jumping then into [`PyAST_FromNodeObject()`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Python/ast.c#L772) inside `Python/ast.c`, you can see it receives the `node *` tree, the filename, compiler flags, and the `PyArena`.

The return type from this function is [`mod_ty`](https://github.com/python/cpython/blob/d93605de7232da5e6a182fd1d5c220639e900159/Include/ast.h#L10), defined in `Include/Python-ast.h`. `mod_ty` is a container structure for one of the 5 module types in Python:

1. `Module`
2. `Interactive`
3. `Expression`
4. `FunctionType`
5. `Suite`

In `Include/Python-ast.h` you can see that an `Expression` type requires a field `body`, which is an `expr_ty` type. The `expr_ty` type is also defined in `Include/Python-ast.h`:

```c
enum _mod_kind {Module_kind=1, Interactive_kind=2, Expression_kind=3,
                 FunctionType_kind=4, Suite_kind=5};
struct _mod {
    enum _mod_kind kind;
    union {
        struct {
            asdl_seq *body;
            asdl_seq *type_ignores;
        } Module;

        struct {
            asdl_seq *body;
        } Interactive;

        struct {
            expr_ty body;
        } Expression;

        struct {
            asdl_seq *argtypes;
            expr_ty returns;
        } FunctionType;

        struct {
            asdl_seq *body;
        } Suite;

    } v;
};
```





### Conclusion

CPython’s versatility and low-level execution API make it the ideal candidate for an embedded scripting engine. You will see CPython used in many UI applications, such as Game Design, 3D graphics and system automation.

The interpreter process is flexible and efficient, and now you have an understanding of how it works you’re ready to understand the compiler.



