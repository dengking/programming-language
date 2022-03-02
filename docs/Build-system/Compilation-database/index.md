# Compilation database

## Compilation database[¶](https://sarcasm.github.io/notes/dev/compilation-database.html#compilation-database)



### [How to generate a JSON Compilation Database?](https://sarcasm.github.io/notes/dev/compilation-database.html#id26)[¶](https://sarcasm.github.io/notes/dev/compilation-database.html#how-to-generate-a-json-compilation-database)

#### [Build systems and compilers](https://sarcasm.github.io/notes/dev/compilation-database.html#id51)[¶](https://sarcasm.github.io/notes/dev/compilation-database.html#build-systems-and-compilers)

[CMake](https://sarcasm.github.io/notes/dev/compilation-database.html#id54)[¶](https://sarcasm.github.io/notes/dev/compilation-database.html#cmake)

```sh
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
```

> NOTE:
>
> 参见： CMAKE_EXPORT_COMPILE_COMMANDS[¶](https://cmake.org/cmake/help/latest/variable/CMAKE_EXPORT_COMPILE_COMMANDS.html#cmake-export-compile-commands)

[Ninja](https://sarcasm.github.io/notes/dev/compilation-database.html#id55)[¶](https://sarcasm.github.io/notes/dev/compilation-database.html#ninja)

```shell
ninja -t compdb > compile_commands.json
```

> NOTE: 
>
> 我的实践:
>
> ```sh
> ninja -C out/Default -t compdb cc cxx
> ```
>
> 

