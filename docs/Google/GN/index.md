# GN

## googlesource [gn](https://gn.googlesource.com/?format=HTML) / [**gn**](https://gn.googlesource.com/gn/)

GN is a meta-build system that generates [NinjaBuild](https://chromium.googlesource.com/chromium/src/tools/gn/+/48062805e19b4697c5fbd926dc649c78b6aaa138/NinjaBuild.md) files. It's meant to be faster and simpler than GYP. It outputs only Ninja build files.



## googlesource [GN Quick Start guide](https://gn.googlesource.com/gn/+/main/docs/quick_start.md)

### Setting up a build

> NOTE: 
>
> gn本质上三ninja build file的generator，因此使用它的第一步就是 `gn gen`

```shell
gn gen out/my_build
```

### Passing build arguments

```shell
gn args out/my_build
```



```sh
gn args --list out/my_build
```

### Cross-compiling to a target OS or architecture

> NOTE: 



## googlesource [How GN handles cross-compiling](https://chromium.googlesource.com/chromium/src/tools/gn/+/48062805e19b4697c5fbd926dc649c78b6aaa138/docs/cross_compiles.md)