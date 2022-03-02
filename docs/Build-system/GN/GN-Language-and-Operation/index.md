# googlesource [GN Language and Operation](https://chromium.googlesource.com/chromium/src/tools/gn/+/48062805e19b4697c5fbd926dc649c78b6aaa138/docs/language.md)

> NOTE:
>
> 补充文档:
>
> https://blog.csdn.net/u014786330/article/details/84562388

## Introduction

### Design philosophy

> NOTE: 
>
> gn的design philosophy中的第一条、第二条其实本质上阐述的就是"极简化原则"



## Language



There are some built-in variables whose values depend on the current environment. See `gn help` for more.

> NOTE: 
>
> 一、下面就是"built-in variables"
>
> ```sh
> Built-in predefined variables (type "gn help <variable>" for more help):
> current_cpu: [string] The processor architecture of the current toolchain.
> current_os: [string] The operating system of the current toolchain.
> current_toolchain: [string] Label of the current toolchain.
> default_toolchain: [string] Label of the default toolchain.
> gn_version: [number] The version of gn.
> host_cpu: [string] The processor architecture that GN is running on.
> host_os: [string] The operating system that GN is running on.
> invoker: [string] The invoking scope inside a template.
> python_path: [string] Absolute path of Python.
> root_build_dir: [string] Directory where build commands are run.
> root_gen_dir: [string] Directory for the toolchain's generated files.
> root_out_dir: [string] Root directory for toolchain output files.
> target_cpu: [string] The desired cpu architecture for the build.
> target_gen_dir: [string] Directory for a target's generated files.
> target_name: [string] The name of the current target.
> target_os: [string] The desired operating system for the build.
> target_out_dir: [string] Directory for target output files.
> ```

There are purposefully many omissions in the language. There are no loops or function calls, for example. As per the above design philosophy, if you need this kind of thing you're probably doing it wrong.

> NOTE: 
>
> gn的design philosophy的"极简化原则"

### Lists

> NOTE: 
>
> 语法和python一致
>
> 

```python
a = [ "first" ]
a += [ "second" ]  # [ "first", "second" ]
a += [ "third", "fourth" ]  # [ "first", "second", "third", "fourth" ] 
b = a + [ "fifth" ]  # [ "first", "second", "third", "fourth", "fifth" ] 
```

### Scoping and execution

Files and `{ }` blocks introduce new scopes. Scoped are nested. When you read a variable, the containing scopes will be searched in reverse order until a matching name is found. Variable writes always go to the innermost scope.

> NOTE: 
>
> 典型的tree

## Naming things

### File and directory names

There are three possible forms:

Relative names:

```
"foo.cc"
"src/foo.cc"
"../src/foo.cc"
```

Source-tree absolute names:

```
"//net/foo.cc"
"//base/test/foo.cc"
```

System absolute names (rare, normally used for include directories):

```
"/usr/local/include/"
"/C:/Program Files/Windows Kits/Include"
```

> NOTE:
>
> 一、可以看到上述三者之间的差异在于第一个字符，显然gn根据此就可以区分出是哪种情况

### Labels

> NOTE:
>
> 所谓label，其实就是identifier，在gn中可以直接通过label来reference 

Everything that can participate in the **dependency graph** (targets, configs, and toolchains) are identified by labels which are strings of a defined format. A common label looks like this:

```python
"//base/test:test_support"
```

which consists of a **source-root-absolute path**, a colon, and a name. This means to look for the thing named “test_support” in `src/base/test/BUILD.gn`.

> NOTE:
>
> 给定一个label，如何来解释它

If you want to refer to something in the same buildfile, you can omit the path name and just start with a colon.

```python
":base"
```

> NOTE:
>
> 可以使用c++中的current namespace 来进行类比

Labels can be specified as being relative to the current directory:

```python
"source/plugin:myplugin"
"../net:url_request"
```

If a name is unspecified, it will inherit the directory name:

```
"//net" = "//net:net"
"//tools/gn" = "//tools/gn:gn"
```

## Build configuration

### Overall build flow

1、Look for `.gn` file in the current directory and walk up the directory tree until one is found. Set this directory to be the “source root” and interpret this file to find the name of the **build config file**.

> NOTE: 
>
> 一、[chromium](https://github.com/chromium/chromium)/[**.gn**](https://github.com/chromium/chromium/blob/main/.gn) ，其中指定的 `buildconfig` 如下:
>
> ```python
> # The location of the build configuration file.
> buildconfig = "//build/config/BUILDCONFIG.gn"
> ```
>
> 

2、Execute the **build config file** (this is the **default toolchain**).

3、Load the `BUILD.gn` file in the root directory.

4、Recursively load `BUILD.gn` in other directories to resolve all current dependencies. If a **BUILD file** isn't found in the specified location, GN will look in the corresponding location inside `tools/gn/secondary`.

5、When a target's dependencies are resolved, write out the `.ninja` file to disk.

6、When all targets are resolved, write out the root `build.ninja` file.

### The build config file

The first file executed is the **build config file**. The name of this file is specified in the `.gn` file that marks the root of the repository. In Chrome it is `src/build/config/BUILDCONFIG.gn`. There is only one build config file.

This file sets up the scope in which all other build files will execute. Any arguments, variables, defaults, etc. set up in this file will be visible to all files in the build.

It is executed once for each toolchain (see “Toolchains”).

### Build arguments

Typically arguments would be declared in an imported file (to share them among some subset of the build) or in the main build config file (to make them **global**).

### Target defaults

You can set up some default values for a given target type. This is normally done in the build config file to set a list of default configs that defines the **build flags** and other **setup information** for each target type.

> NOTE: 
>
> 一、显然这种方式可以实现code reuse
>
> 二、这种方式是按照target type来指定default

For example, when you declare a `static_library`, the **target defaults** for a **static library** are applied. These values can be overwritten, modified, or preserved by a target.

```python
# This call is typically in the build config file (see above).
set_defaults("static_library") {
  configs = [ "//build:rtti_setup", "//build:extra_warnings" ]
}

# This would be in your directory's BUILD.gn file.
static_library("mylib") {
  # At this point configs is set to [ "//build:rtti_setup", "//build:extra_warnings" ]
  # by default but may be modified.
  configs -= "//build:extra_warnings"  # Don't want these warnings.
  configs += ":mylib_config"  # Add some more configs.
}
```



## Targets

A target is a node in the build graph. It usually represents some kind of executable or library file that will be generated. Targets depend on other targets. The built-in target types (see `gn help <targettype>` for more help) are:

1、`action`: Run a script to generate a file.

2、`action_foreach`: Run a script once for each source file.

3、`component`: Configurable to be another type of library.

4、`executable`: Generates an executable file.

5、`group`: A virtual dependency node that refers to one or more other targets.

6、`shared_library`: A .dll or .so.

7、`source_set`: A lightweight virtual static library (usually preferrable over a real static library since it will build faster).

8、`static_library`: A .lib or .a file (normally you'll want a source_set instead).

9、`test`: Generates an executable but annotates it as a test.

You can extend this to make custom target types using templates (see below).

## Configs

> NOTE:
>
> config其实就是build configuration

Configs are named objects that specify sets of flags, include directories, and defines. They can be applied to a target and pushed to dependent targets.

> NOTE: 
>
> "They can be applied to a target and pushed to dependent targets" 其实就是inheritance

To define a config:

```python
config("myconfig") {
  includes = [ "src/include" ]
  defines = [ "ENABLE_DOOM_MELON" ]
}
```

To apply a config to a target:

```python
executable("doom_melon") {
  configs = [ ":myconfig" ]
}
```

### Public configs



## Toolchains

A toolchain is a set of build commands to run for different types of input files and link tasks.

## Templates

