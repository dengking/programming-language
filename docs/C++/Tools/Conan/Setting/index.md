# Setting of conan

## Default setting

```
C:\Users\dengkai01\.conan\profiles\default
```

内容如下：

```
[settings]
os=Windows
os_build=Windows
arch=x86_64
arch_build=x86_64
compiler=Visual Studio
compiler.version=17
build_type=Release
[options]
[build_requires]
[env]

```

### practice

一般不要修改它。

### 素材

stackoverflow [Conan on windows claims setting isn't set, it is set](https://stackoverflow.com/questions/70587488/conan-on-windows-claims-setting-isnt-set-it-is-set)



## `settings.yml`

其中指定了编译器的版本范围，如果编译器版本不在这个范围内，则需要将其删除，然后重新生成。