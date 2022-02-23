# Clang-Tidy

## docs [Clang-Tidy](https://clang.llvm.org/extra/clang-tidy/#id1)

## source code

[llvm-project](https://github.com/llvm/llvm-project)/[clang-tools-extra](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra)/[**clang-tidy**](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra/clang-tidy)/

### tool

[llvm-project](https://github.com/llvm/llvm-project)/[clang-tools-extra](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra)/[clang-tidy](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra/clang-tidy)/[**tool**](https://github.com/llvm/llvm-project/tree/main/clang-tools-extra/clang-tidy/tool)/

## Install clang- tidy on macOS

### stackoverflow [How to install clang-tidy on macOS?](https://stackoverflow.com/questions/53111082/how-to-install-clang-tidy-on-macos)

[A](https://stackoverflow.com/a/53380855)

- There is no clang-tidy brew formulae.

As a result it seems like the best way to get clang-tidy on macOS is to simply install all of llvm and then make symlinks for the tools you want to use.

```SHELL
brew install llvm
ln -s "$(brew --prefix llvm)/bin/clang-format" "/usr/local/bin/clang-format"
ln -s "$(brew --prefix llvm)/bin/clang-tidy" "/usr/local/bin/clang-tidy"
ln -s "$(brew --prefix llvm)/bin/clang-apply-replacements" "/usr/local/bin/clang-apply-replacements"
```

> NOTE: 
>
> 一、实践记录
>
> 我的环境中，并没有 `/usr/local/bin/` 路径，因此需要自己手动创建`/usr/local/bin` 路径，另外需要注意的是，`/usr/local/bin` 路径已经在PATH环境变量中了，因此在执行上述命令后，并不需要在做其他修改 

Alternatively, you could [download](http://releases.llvm.org/download.html) the prebuilt binaries and create the same symlinks. It's not a good idea to add all of llvm to your `PATH` because of conflicts with the default clang compiler.



### 安装记录

```SHELL
(base) % brew install llvm
Running `brew update --preinstall`...
==> Auto-updated Homebrew!
Updated 3 taps (homebrew/cask-versions, homebrew/core and homebrew/cask).
==> New Formulae
boost@1.76                                                                        kdoctor                                                                           librasterlite2
==> Updated Formulae
Updated 323 formulae.
==> Deleted Formulae
carina                             go@1.10                            go@1.11                            go@1.12                            go@1.9                             hornetq                            path-extractor
==> New Casks
1password-cli-beta                                           heroic                                                       mysteriumvpn                                                 openshift-client
==> Updated Casks
Updated 230 casks.
==> Deleted Casks
lilypond

==> Downloading https://ghcr.io/v2/homebrew/core/llvm/manifests/13.0.1
######################################################################## 100.0%
==> Downloading https://ghcr.io/v2/homebrew/core/llvm/blobs/sha256:477c5a7aecc0e9d4ae46b0d91a543ff05bfc8dd9425c0164b18b459d58c4f22e
==> Downloading from https://pkg-containers.githubusercontent.com/ghcr1/blobs/sha256:477c5a7aecc0e9d4ae46b0d91a543ff05bfc8dd9425c0164b18b459d58c4f22e?se=2022-02-22T06%3A05%3A00Z&sig=q2naG4J7FwHrdHfmRrFGpvCBRiL5%2BTiAgcptmNd23wQ%3D&sp=r&spr=https
######################################################################## 100.0%
==> Pouring llvm--13.0.1.arm64_monterey.bottle.tar.gz
==> Caveats
To use the bundled libc++ please add the following LDFLAGS:
  LDFLAGS="-L/opt/homebrew/opt/llvm/lib -Wl,-rpath,/opt/homebrew/opt/llvm/lib"

llvm is keg-only, which means it was not symlinked into /opt/homebrew,
because macOS already provides this software and installing another version in
parallel can cause all kinds of trouble.

If you need to have llvm first in your PATH, run:
  echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> ~/.zshrc

For compilers to find llvm you may need to set:
  export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"
  export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"

==> Summary
🍺  /opt/homebrew/Cellar/llvm/13.0.1: 5,467 files, 909.2MB
==> Running `brew cleanup llvm`...
Disable this behaviour by setting HOMEBREW_NO_INSTALL_CLEANUP.
Hide these hints with HOMEBREW_NO_ENV_HINTS (see `man brew`).
```

> NOTE: 
>
> 执行 `brew --prefix llvm` ，输出的结果如下：
>
> ```shell
> /opt/homebrew/opt/llvm
> ```
>
> 

### see also

[
sleepdefic1t](https://gist.github.com/sleepdefic1t)/**[MACOS_CLANG_TIDY.md](https://gist.github.com/sleepdefic1t/e9bdb1a66b05aa043ab9a2ab6c929509)**



## 实践、workflow

kdab [Clang-Tidy, part 1: Modernize your source code using C++11/C++14](https://www.kdab.com/clang-tidy-part-1-modernize-source-code-using-c11c14/)

kdab [Clang Tidy, part 2: Integrate qmake and other build systems using Bear](https://www.kdab.com/clang-tidy-part-2-integrate-qmake-and-other-build-systems-using-bear/)



## microsoft [Exploring Clang Tooling – Using Build Tools with clang-tidy](https://devblogs.microsoft.com/cppblog/exploring-clang-tooling-using-build-tools-with-clang-tidy/)

> NOTE: 
>
> 这篇文章讨论了 `clang-apply-replacements` tool 的用法
>
> 作者的blogger: [Steveire's Blog](https://steveire.wordpress.com/)



## clang-apply-replacements

https://lists.llvm.org/pipermail/cfe-users/2019-April/001505.html