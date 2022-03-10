# chromium gn

一、`build` 目录中保存的是gn build file。

1、toolchain

2、os

3、cpu

面对如此纷繁复杂的build file，如何梳理清楚它们之间的关系？

1、build configuration，主要位于 [chromium](https://github.com/chromium/chromium)/[build](https://github.com/chromium/chromium/tree/main/build)/[config](https://github.com/chromium/chromium/tree/main/build/config) 目录



## [chromium](https://github.com/chromium/chromium)/[**.gn**](https://github.com/chromium/chromium/blob/main/.gn)





## [chromium](https://github.com/chromium/chromium)/[build](https://github.com/chromium/chromium/tree/main/build)/[config](https://github.com/chromium/chromium/tree/main/build/config)/[**BUILDCONFIG.gn**](https://github.com/chromium/chromium/blob/main/build/config/BUILDCONFIG.gn)



### WHAT IS THIS FILE?

This is the master GN build configuration. This file is loaded after the build args (`args.gn`) for the build directory and after the toplevel "`.gn`" file (which points to this file as the build configuration).

> NOTE:
>
> `args.gn`是我们通过`gn args` 命令添加的build args保存下来的文件

### PLATFORM SELECTION

There are two main things to set: "os" and "cpu". The "toolchain" is the name of the GN thing that encodes combinations of these things.

> NOTE: 
>
> 这段话的意思是toolchain是由os和CPU两者决定的
> 

|            |             |                   |
| ---------- | ----------- | ----------------- |
| host_os    | host_cpu    | host_toolchain    |
| target_os  | target_cpu  | default_toolchain |
| current_os | current_cpu | current_toolchain |



### BUILD FLAGS

#### `is_clang`

```python
  # Set to true when compiling with the Clang compiler.
  is_clang = current_os != "linux" ||
             (current_cpu != "s390x" && current_cpu != "s390" &&
              current_cpu != "ppc64" && current_cpu != "ppc" &&
              current_cpu != "mips" && current_cpu != "mips64" &&
              current_cpu != "riscv64")
```

如果`current_os` 是 "linux"，则`is_clang` 就是false，也就是说Linux上上不使用clang编译器的，显然这和chromium宣称的不符合，那实际情况是这样的吗？



### TOOLCHAIN SETUP

> Here we set the default toolchain, as well as the variable `host_toolchain` which will identify the toolchain corresponding to the local system when doing cross-compiles. When not cross-compiling, this will be the same as the default toolchain.

一、从上面的内容可知， variable `host_toolchain` 的目的是支持 cross-compile

#### `host_toolchain`

它是一个build arg，定义如下:

```python
declare_args() {
  # This should not normally be set as a build argument.  It's here so that
  # every toolchain can pass through the "global" value via toolchain_args().
  host_toolchain = ""
}
```



```python
if (host_toolchain == "") { # 含义是用户没有指定host toolchain，则需要使用default toolchain
  # This should only happen in the top-level context.
  # In a specific toolchain context, the toolchain_args()
  # block should have propagated a value down.
  # TODO(dpranke): Add some sort of assert here that verifies that
  # no toolchain omitted host_toolchain from its toolchain_args().

  if (host_os == "linux") {
    if (target_os != "linux") {
      host_toolchain = "//build/toolchain/linux:clang_$host_cpu"
    } else if (is_clang) {
      host_toolchain = "//build/toolchain/linux:clang_$host_cpu"
    } else {
      host_toolchain = "//build/toolchain/linux:$host_cpu"
    }
  } else if (host_os == "mac") {
    host_toolchain = "//build/toolchain/mac:clang_$host_cpu"
  } else if (host_os == "win") {
    # On Windows always use the target CPU for host builds for x86/x64. On the
    # configurations we support this will always work and it saves build steps.
    # Windows ARM64 targets require an x64 host for cross build.
    if (target_cpu == "x86" || target_cpu == "x64") {
      if (is_clang) {
        host_toolchain = "//build/toolchain/win:win_clang_$target_cpu"
      } else {
        host_toolchain = "//build/toolchain/win:$target_cpu"
      }
    } else if (is_clang) {
      host_toolchain = "//build/toolchain/win:win_clang_$host_cpu"
    } else {
      host_toolchain = "//build/toolchain/win:$host_cpu"
    }
  } else if (host_os == "aix") {
    host_toolchain = "//build/toolchain/aix:$host_cpu"
  } else if (host_os == "zos") {
    host_toolchain = "//build/toolchain/zos:$host_cpu"
  } else {
    assert(false, "Unsupported host_os: $host_os")
  }
}

```

一、`host_os` 仅仅需要考虑:

1、"linux"

2、"mac"

3、"win"

4、"aix"

5、"zos"

在实际开发中，这可能在上述OS中编译；

二、根据上述code可知，toolchain在 `//build/toolchain/` 中定义

#### `_default_toolchain`、`set_default_toolchain`

```python

_default_toolchain = ""

if (target_os == "android") {
  assert(host_os == "linux" || host_os == "mac",
         "Android builds are only supported on Linux and Mac hosts.")
  _default_toolchain = "//build/toolchain/android:android_clang_$target_cpu"
} else if (target_os == "chromeos" || target_os == "linux") {
  # See comments in build/toolchain/cros/BUILD.gn about board compiles.
  if (is_clang) {
    _default_toolchain = "//build/toolchain/linux:clang_$target_cpu"
  } else {
    _default_toolchain = "//build/toolchain/linux:$target_cpu"
  }
} else if (target_os == "fuchsia") {
  _default_toolchain = "//build/toolchain/fuchsia:$target_cpu"
} else if (target_os == "ios") {
  _default_toolchain = "//build/toolchain/ios:ios_clang_$target_cpu"
} else if (target_os == "mac") {
  assert(host_os == "mac" || host_os == "linux",
         "Mac cross-compiles are unsupported.")
  _default_toolchain = "//build/toolchain/mac:clang_$target_cpu"
} else if (target_os == "win") {
  # On Windows, we use the same toolchain for host and target by default.
  # Beware, win cross builds have some caveats, see docs/win_cross.md
  if (is_clang) {
    _default_toolchain = "//build/toolchain/win:win_clang_$target_cpu"
  } else {
    _default_toolchain = "//build/toolchain/win:$target_cpu"
  }
} else if (target_os == "winuwp") {
  # Only target WinUWP on for a Windows store application and only
  # x86, x64 and arm are supported target CPUs.
  assert(target_cpu == "x86" || target_cpu == "x64" || target_cpu == "arm" ||
         target_cpu == "arm64")
  _default_toolchain = "//build/toolchain/win:uwp_$target_cpu"
} else if (target_os == "aix") {
  _default_toolchain = "//build/toolchain/aix:$target_cpu"
} else if (target_os == "zos") {
  _default_toolchain = "//build/toolchain/zos:$target_cpu"
} else {
  assert(false, "Unsupported target_os: $target_os")
}

# If a custom toolchain has been set in the args, set it as default. Otherwise,
# set the default toolchain for the platform (if any).
if (custom_toolchain != "") {
  set_default_toolchain(custom_toolchain)
} else if (_default_toolchain != "") {
  set_default_toolchain(_default_toolchain)
}

```



### OS DEFINITIONS



### TARGET DEFAULTS

> Set up the default configuration for every build target of the given type.

原文的这一段内容是非常值得阅读的，并且原文这一段的内容非常繁杂，因为它cover了太多的内容，这也可以看出chromium项目的灵活、强大。

#### `default_compiler_configs`



### COMPONENT SETUP



## [chromium](https://github.com/chromium/chromium)/[build](https://github.com/chromium/chromium/tree/main/build)/[config](https://github.com/chromium/chromium/tree/main/build/config)/[compiler](https://github.com/chromium/chromium/tree/main/build/config/compiler)/[**BUILD.gn**](https://github.com/chromium/chromium/blob/main/build/config/compiler/BUILD.gn)

## `build/toolchain/`

预备知识: 

1、[gn `toolchain` command](https://chromium.googlesource.com/chromium/src/tools/gn/+/48062805e19b4697c5fbd926dc649c78b6aaa138/docs/language.md#declaring-a-toolchain)

> Toolchains are declared with the `toolchain` command, which sets the commands to use for each compile and link operation. The toolchain also specifies a set of arguments to pass to the build config file when executing. This allows you to pass configuration information to the alternate toolchain.

2、[**root_build_dir**](https://gn.googlesource.com/gn/+/master/docs/reference.md#var_root_build_dir)

3、[**rebase_path**](https://gn.googlesource.com/gn/+/master/docs/reference.md#func_rebase_path)

使用它的目的是获得abstract path

### [chromium](https://github.com/chromium/chromium)/[build](https://github.com/chromium/chromium/tree/main/build)/[toolchain](https://github.com/chromium/chromium/tree/main/build/toolchain)/[**gcc_toolchain.gni**](https://github.com/chromium/chromium/blob/main/build/toolchain/gcc_toolchain.gni)

#### `template("gcc_toolchain") `

可以看到，其中调用了 [gn `toolchain` command](https://chromium.googlesource.com/chromium/src/tools/gn/+/48062805e19b4697c5fbd926dc649c78b6aaa138/docs/language.md#declaring-a-toolchain) 。

#### `template("clang_toolchain")` 

```python
# This is a shorthand for gcc_toolchain instances based on the Chromium-built
# version of Clang. Only the toolchain_cpu and toolchain_os variables need to
# be specified by the invoker, and optionally toolprefix if it's a
# cross-compile case. Note that for a cross-compile case this toolchain
# requires a config to pass the appropriate -target option, or else it will
# actually just be doing a native compile. The invoker can optionally override
# use_gold too.
template("clang_toolchain") {
  gcc_toolchain(target_name) {
    prefix = rebase_path("$clang_base_path/bin", root_build_dir)
    cc = "${prefix}/clang"
    cxx = "${prefix}/clang++"
    ld = cxx
    readelf = "${prefix}/llvm-readelf"
    ar = "${prefix}/llvm-ar"
    nm = "${prefix}/llvm-nm"

    forward_variables_from(invoker,
                           [
                             "strip",
                             "default_shlib_subdir",
                             "dwp",
                             "enable_linker_map",
                             "loadable_module_extension",
                             "use_unstripped_as_runtime_outputs",
                           ])

    toolchain_args = {
      if (defined(invoker.toolchain_args)) {
        forward_variables_from(invoker.toolchain_args, "*")
      }
      is_clang = true
    }
  }
}
```

可以看到，`template("clang_toolchain")` 继承了 `template("gcc_toolchain") `。



## 关于c++的编译选项

都在 `build/config/c++/BUILD.gn` 中进行指定



需要搞清楚chromium在编译过程中，是如何来运用完整的llvm toolchain？这些都需要对gn和ninja有一些认识。

重点关注它的iOS编译

