## 1.8. Directory structure[¶](https://devguide.python.org/setup/#directory-structure)

There are several top-level directories in the CPython source tree. Knowing what each one is meant to hold will help you find where a certain piece of functionality is implemented. Do realize, though, there are always exceptions to every rule.

- `Doc`

  The official documentation. This is what https://docs.python.org/ uses. See also [Building the documentation](https://devguide.python.org/documenting/#building-doc).

- `Grammar`

  Contains the EBNF grammar file for Python.

- `Include`

  Contains all interpreter-wide header files.

- `Lib`

  The part of the standard library implemented in pure Python.

- `Mac`

  Mac-specific code (e.g., using IDLE as an OS X application).

- `Misc`

  Things that do not belong elsewhere. Typically this is varying kinds of developer-specific documentation.

- `Modules`

  The part of the standard library (plus some other code) that is implemented in C.

- `Objects`

  Code for all built-in types.

- `PC`

  Windows-specific code.

- `PCbuild`

  Build files for the version of MSVC currently used for the Windows installers provided on python.org.

- `Parser`

  Code related to the parser. The definition of the AST nodes is also kept here.

- `Programs`

  Source code for C executables, including the main function for the CPython interpreter (in versions prior to Python 3.5, these files are in the Modules directory).

- `Python`

  The code that makes up the core CPython runtime. This includes the compiler, eval loop and various built-in modules.

- `Tools`

  Various tools that are (or have been) used to maintain Python.

