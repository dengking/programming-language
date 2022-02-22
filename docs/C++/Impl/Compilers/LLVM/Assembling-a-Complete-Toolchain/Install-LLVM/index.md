# Install LLVM

## Linux

### addictivetips [How to install Clang on Ubuntu](https://www.addictivetips.com/ubuntu-linux-tips/clang-on-ubuntu/)

```C++
sudo apt install clang-12 --install-suggests
```

> NOTE: 
>
> 我第一次按照的时候，使用的国内的阿里云的源，结果报了如下错误:
>
> ```shell
> The following packages have unmet dependencies:
> libclang-6.0-dev : Depends: libclang1-6.0 (= 1:6.0-1ubuntu2) but 1:6.0.1~svn334776-1~exp1~20190309042703.125 is to be installed
>               Depends: libclang-common-6.0-dev (= 1:6.0-1ubuntu2) but 1:6.0.1~svn334776-1~exp1~20190309042703.125 is to be installed
> E: Unable to correct problems, you have held broken packages.
> #[error]Process completed with exit code 100.
> ```
>
> 后来Google "ubuntu install clang failed"，发现了GitHub [actions](https://github.com/actions)/**[virtual-environments](https://github.com/actions/virtual-environments)** 的 [Installing clang from ubuntu package repository fails #46](https://github.com/actions/virtual-environments/issues/46) 中正好遇到了相同的问题:
>
> A: 
>
> > Following https://apt.llvm.org/ doesn't seem to work either
>
> A:
>
> > [#104 (comment)](https://github.com/actions/virtual-environments/issues/104#issuecomment-555806825)
> >
> > works
>



### github [actions](https://github.com/actions)/**[virtual-environments](https://github.com/actions/virtual-environments)** # [Add C++ support (GCC and Clang) on all platforms #104](https://github.com/actions/virtual-environments/issues/104)

On Ubuntu I use this to install it as `clang-9` (takes about a minute):

```sh
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key 2>/dev/null | sudo apt-key add -
sudo add-apt-repository 'deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-9 main' -y
sudo apt-get update -q
sudo apt-get install -y clang-9 lld-9 libc++-9-dev libc++abi-9-dev clang-tools-9
```

> NOTE: 
>
> 初次看上述code，我发现它并没有install clang++，然后我看了 [**LLVM Debian/Ubuntu nightly packages**](https://apt.llvm.org/)，发现其中的install中，有没有clang++。



## Install llvm on macOS

### stackoverflow [How to install clang-tidy on macOS?](https://stackoverflow.com/questions/53111082/how-to-install-clang-tidy-on-macos)

#### [A](https://stackoverflow.com/a/53380855)

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



## [**LLVM Debian/Ubuntu nightly packages**](https://apt.llvm.org/)

