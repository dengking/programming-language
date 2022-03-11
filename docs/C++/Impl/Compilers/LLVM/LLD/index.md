# LLD

## LLD - The LLVM Linker[¶](https://lld.llvm.org/#lld-the-llvm-linker)

### Using LLD[¶](https://lld.llvm.org/#using-lld)

If you don’t want to change the system setting, you can use clang’s `-fuse-ld` option. In this way, you want to set `-fuse-ld=lld` to LDFLAGS when building your programs.

> NOTE:
>
> chromium就是这样用的

## source code

[llvm-project](https://github.com/llvm/llvm-project)/[**lld**](https://github.com/llvm/llvm-project/tree/main/lld)/



## errors

### ld-link: error: invalid timestamp

```
ld-link: error: invalid timestamp: -2142000. Expected 32-bit integer
```

#### source code

[llvm-project](https://github.com/llvm/llvm-project)/[lld](https://github.com/llvm/llvm-project/tree/main/lld)/[COFF](https://github.com/llvm/llvm-project/tree/main/lld/COFF)/[**Driver.cpp#L1645**](https://github.com/llvm/llvm-project/blob/main/lld/COFF/Driver.cpp#L1645)

```c++
  // Handle /timestamp
  if (llvm::opt::Arg *arg = args.getLastArg(OPT_timestamp, OPT_repro)) {
    if (arg->getOption().getID() == OPT_repro) {
      config->timestamp = 0;
      config->repro = true;
    } else {
      config->repro = false;
      StringRef value(arg->getValue());
      if (value.getAsInteger(0, config->timestamp))
        fatal(Twine("invalid timestamp: ") + value +
              ".  Expected 32-bit integer");
    }
  } else {
    config->repro = false;
    config->timestamp = time(nullptr);
  }
```



reviews.llvm [**[lld] Use a real timestamp, with the option to use hash or explicit value**](https://reviews.llvm.org/D46966)

这篇文章讲述了添加上述code的原因



#### 如何解决

groups.google [Build different versions](https://groups.google.com/a/chromium.org/g/chromium-dev/c/FPwgrwOOsds)

```
What happens if you run `gn clean <out dir>` before your build?
```

