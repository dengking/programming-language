# 交叉编译

本机的开发环境处理非常简单，我们通过前面章节的介绍很容易可以通过 CMake 来构建和生成所有三方库并导入到工程使用。但最终产品发布不可能只是个别平台，我们期望它能适配所有平台所有架构，本节将详细介绍各个场景下的交叉编译配置及常见问题。

## 交叉编译基础概念

Conan 架构相关描述与其他平台、厂商描述不太一致，但均可以覆盖到。以下是 Conan 架构信息与其他平台架构信息惯用名词的对照表：

| Conan  | Android     | iOS    | Windows   | Linux  |
| ------ | :---------- | :----- | :-------- | :----- |
| armv8  | arm64-v8a   | arm64  | None      | arm64  |
| armv7  | armeabi-v7a | armv7  | None      | None   |
| x86_64 | x86_64      | x86_64 | x64       | x86_64 |
| x86    | x86         | None   | Win32/x86 | x86    |

与 CMake 中不同，Conan 在进行交叉编译时使用的一些名词与 CMake 不同，经常使用 CMake 的朋友在切换到 Conan 时很容易被搞混乱。以下表格列出了在不同场景下的名词解释：

| 工具  | 交叉编译本机环境       | 交叉编译目标环境  |
| ----- | :--------------------- | :---------------- |
| CMake | CMAKE_HOST_SYSTEM_NAME | CMAKE_SYSTEM_NAME |
| Conan | BUILD                  | HOST              |

Conna 要求交叉编译必须指定两个 profile 信息，一个表示 BUILD 环境就是当前编译环境信息，一个表示 HOST 目标系统环境信息。这样它才会启用交叉编译配置。这也就是为什么在前面的章节中的 CMakeLists.txt 会判断是否指定了 `CONAN_PROFILE_BUILD` 和 `CONAN_PROFILE_HOST` 的原因了。

通过 CMake 传递 `CONAN_PROFILE_BUILD` 和 `CONAN_PROFILE_HOST` 参数，我们就可以自动化将这些配置信息传递给 Conan，无需手动去执行 Conan install 流程，减少出错可能和麻烦的步骤。一个简单的示例如下：

```bash
cmake -Bdarwin-arm64 -GXcode \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCONAN_PROFILE_BUILD=default \
  -DCONAN_PROFILE_HOST=$(pwd)/.build/macos-arm64 \
  -DCMAKE_INSTALL_PREFIX=exports
```

- `CONAN_PROFILE_BUILD=default` 表示使用当前系统默认的 profile 文件
- `CONAN_PROFILE_HOST=$(pwd)/.build/macos-arm64` 表示使用当前 .build 目录下的配置文件

下面我们将详细介绍各平台的交叉编译配置。

## macOS

macOS 会有交叉编译吗？有的，在 M1 系列机型出来以后，我们不得不在某些场景下在 M1 机型交叉编译 x86_64 架构的产物。反过来也一样，我们也可以在原来 x86_64 架构中交叉编译 arm64 架构的 macOS SDK。macOS 的交叉编译是最简单的，我们只需要声明以下目标系统的信息在一个自定义的 profile 中，然后在 CMake 初始化时传递给 Conan 即可，自定义 profile 如下：

```
include(default)

[settings]
os=Macos
arch=armv8
compiler=apple-clang
compiler.libcxx=libc++
compiler.cppstd=14
[options]
[build_requires]
[env]
```

- include(default) 表示以本机默认 profile 为基础，下面的配置将覆盖默认 profile

::: warning 注意
我们发现 compiler.libcxx 及 compiler.cppstd 配置不会在 include default profile 时携带，请务必手动配置。
:::

目标配置文件就绪后，使用如下命令初始化 CMake 即可自动进行三方库的编译：

```bash
cmake -Bdarwin-arm64 -GXcode \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCONAN_PROFILE_BUILD=default \
  -DCONAN_PROFILE_HOST=$(pwd)/.build/macos-arm64 \
  -DCMAKE_INSTALL_PREFIX=exports
```

- `.build/macos-arm64` 就是我们上面提到的目标系统配置，该文件可以保存在代码仓库中，确保所有 CI agent 可以访问到该配置。
- `CMAKE_OSX_ARCHITECTURES` 告诉当前项目要表一的目标架构师 arm64，它不会传递给三方库

## iOS

iOS 不仅仅有 x86_64、arm64、armv7，还区分真机、模拟器，并且目前 arm64 和 x86_64 都可以编译出模拟器产物。我们需要给不同的架构设计 profile。内容如下：

### arm64

```
include(default)

[settings]
arch=armv8
compiler=apple-clang
compiler.libcxx=libc++
compiler.cppstd=14
os=iOS
os.version=9.3
os.sdk=iphoneos
```

- `arch=armv8` 表示要编译的是 arm64 架构
- `os=iOS` 表示目标系统为 iOS
- `os.version=9.3` 表示支持的最低 iOS 系统版本为 9.3，与 Xcode deployment target 表述一致。
- `os.sdk=iphoneos` 表示使用真机 SDK 而不是模拟器的

### arm64 模拟器

```
include(default)

[settings]
arch=armv8
compiler=apple-clang
compiler.libcxx=libc++
compiler.cppstd=14
os=iOS
os.version=9.3
os.sdk=iphonesimulator
```

与真机唯一不同的是，`os.sdk` 为 `iphonesimulator`，表示目标系统是模拟器而不是真机。

### armv7

```
include(default)

[settings]
arch=armv7
compiler=apple-clang
compiler.libcxx=libc++
compiler.cppstd=14
os=iOS
os.version=9.3
os.sdk=iphoneos
```

除了 arch 以外，其他与上述架构配置信息一致。

### x86_64 模拟器

```
include(default)

[settings]
arch=x86_64
compiler=apple-clang
compiler.libcxx=libc++
compiler.cppstd=14
os=iOS
os.version=9.3
os.sdk=iphonesimulator
```

### 生成 iOS 目标文件

同样通过 CMake 我们可以传递本机和目标系统 profile 来实现交叉编译到 iOS，以 arm64 举例：

```bash
cmake -Barm64 -GXcode \
  -DCMAKE_BUILD_TYPE=Release \
  -DCONAN_PROFILE_BUILD=default \
  -DCONAN_PROFILE_HOST=$(pwd)/.profiles/ios-arm64-iphoneos \
  -DCMAKE_SYSTEM_NAME=iOS \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=9.3 \
  -DCMAKE_OSX_SYSROOT=iphoneos \
  -DCMAKE_OSX_ARCHITECTURES=arm64
```

- `DCONAN_PROFILE_BUILD` 表示当前本机默认 profile 信息
- `DCONAN_PROFILE_HOST` 表示当前 .profiles 目录下的目标 profile 信息
- `DCMAKE_SYSTEM_NAME` 告诉 CMake 当前工程目标系统是 iOS，它不会传递给三方库
- `DCMAKE_OSX_DEPLOYMENT_TARGET` 告诉 CMake 当前工程目标系统最低版本为 9.3，同上
- `DCMAKE_OSX_SYSROOT` 告诉 CMake 当前工程目标为真机环境
- `DCMAKE_OSX_ARCHITECTURES` 告诉 CMake 当前工程目标架构为 arm64

最后当三方库编译完成、CMake 初始化完成，执行 cmake --build 即可编译整个工程。

## Linux

```
# linux-x86_64
include(default)

[settings]
os=Linux
arch=x86_64
compiler=gcc
compiler.version=7
compiler.libcxx=libstdc++11
compiler.cppstd=14
[options]
[build_requires]
[env]
CC=gcc
CXX=g++
CMAKE_C_COMPILER=gcc
CMAKE_CXX_COMPILER=g++
STRIP=strip
RANLIB=ranlib
AS=as
AR=ar
LD=ld
FC=gfortran
CXXFLAGS="-fvisibility=hidden -fvisibility-inlines-hidden"
CFLAGS="-fvisibility=hidden -fvisibility-inlines-hidden"
```

```
# linux-arm64
include(default)

[settings]
os=Linux
arch=armv8
compiler=gcc
compiler.version=7
compiler.libcxx=libstdc++11
compiler.cppstd=14
[options]
[build_requires]
[env]
CC=aarch64-linux-gnu-gcc
CXX=aarch64-linux-gnu-g++
CMAKE_C_COMPILER=aarch64-linux-gnu-gcc
CMAKE_CXX_COMPILER=aarch64-linux-gnu-g++
STRIP=aarch64-linux-gnu-strip
RANLIB=aarch64-linux-gnu-ranlib
AS=aarch64-linux-gnu-as
AR=aarch64-linux-gnu-ar
LD=aarch64-linux-gnu-ld
FC=aarch64-linux-gnu-gfortran
CXXFLAGS="-fvisibility=hidden -fvisibility-inlines-hidden"
CFLAGS="-fvisibility=hidden -fvisibility-inlines-hidden"
```

### 手动配置交叉编译环境

#### Docker 安装

我们将使用 Docker 来搭建交叉编译环境，所以主机的操作系统可以是 Windows/Linux/MacOS 中的任意一个。如果您的操作系统是 Linux，且不介意修改环境配置，那可以跳过该步骤。

- 参考官方文档[安装 Docker Desktop](https://docs.docker.com/get-docker/)。
- 执行 `docker --help` 验证 docker 已经安装成功。
- 执行 `docker pull ubuntu:xenial` 安装 Ubuntu 16.04 容器。
  - 使用 ubuntu 16.04 的好处在于编译出的产物依赖的 glibc 版本较低(GLIBC2.23)，这样可以在在市面上绝大部分的 Linux 系统上运行。
- 执行 `docker run -it ubuntu:xenial` 启动容器。

#### 工具链安装

- 更新软件源。

  ```bash
  > apt-get update
  ```

- 安装 cmake wget

  ```bash
  > apt-get install -y cmake wget
  ```

- 安装交叉编译工具链

  ```bash
  # 下载工具链，此处以gcc7.5版本为例
  > wget -c https://releases.linaro.org/components/toolchain/binaries/7.5-2019.12/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
  # 安装到 /usr/local/toolchain/ 目录下，当然也可以放在其他任何你喜欢的地方
  > mkdir -p /usr/local/toolchain
  # 解压工具链
  > tar -xvf gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz -C /usr/local/toolchain
  # 查看是否安装成功
  > ls /usr/local/toolchain
  ```

- 配置工具链到环境变量
  如果您不想修改环境变量，可以跳过此步骤，通过 [CMAKE_TOOLCHAIN_FILE](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#:~:text=In%20normal%20builds%2C%20CMake%20automatically%20determines%20the%20toolchain,Languages%20are%20enabled%20by%20the%20project%20%28%29%20command.) 指定工具链路径。

  ```bash
  > update-alternatives --install /usr/bin/aarch64-linux-gnu-gcc aarch64-linux-gnu-gcc /usr/local/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc 100
  > update-alternatives --install /usr/bin/aarch64-linux-gnu-g++ aarch64-linux-gnu-g++ /usr/local/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++ 100
  > update-alternatives --install /usr/bin/aarch64-linux-gnu-gcov aarch64-linux-gnu-gcov /usr/local/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcov 100
  > update-alternatives --install /usr/bin/aarch64-linux-gnu-gcov-dump aarch64-linux-gnu-gcov-dump /usr/local/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcov-dump 100
  > update-alternatives --install /usr/bin/aarch64-linux-gnu-gcov-tool aarch64-linux-gnu-gcov-tool /usr/local/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcov-tool 100
  > update-alternatives --install /usr/bin/aarch64-linux-gnu-ar aarch64-linux-gnu-ar /usr/local/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-ar 100
  > update-alternatives --install /usr/bin/aarch64-linux-gnu-as aarch64-linux-gnu-as /usr/local/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-as 100
  > update-alternatives --install /usr/bin/aarch64-linux-gnu-ld aarch64-linux-gnu-ld /usr/local/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-ld 100
  > update-alternatives --install /usr/bin/aarch64-linux-gnu-ranlib aarch64-linux-gnu-ranlib /usr/local/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-ranlib 100
  > update-alternatives --install /usr/bin/aarch64-linux-gnu-gfortran aarch64-linux-gnu-gfortran /usr/local/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gfortran 100
  > update-alternatives --install /usr/bin/aarch64-linux-gnu-strip aarch64-linux-gnu-strip /usr/local/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-strip 100
  # check gcc&g++ version
  > aarch64-linux-gnu-gcc -v
  > aarch64-linux-gnu-g++ -v
  ```

- 使用 update-alternatives 可以很轻松的在不同的工具链间切换, 比如在 gcc7/8/9 之间切换:

  ```bash
  > apt install build-essential
  > apt -y install gcc-7 g++-7 gcc-8 g++-8 gcc-9 g++-9
  > update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 7
  > update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 7
  > update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 8
  > update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 8
  > update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 9
  > update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 9
  > update-alternatives --config gcc
  There are 3 choices for the alternative gcc (providing /usr/bin/gcc).

    Selection    Path            Priority   Status
  ------------------------------------------------------------
    0            /usr/bin/gcc-9   9         auto mode
    1            /usr/bin/gcc-7   7         manual mode
    2            /usr/bin/gcc-8   8         manual mode
  * 3            /usr/bin/gcc-9   9         manual mode
  Press <enter> to keep the current choice[*], or type selection number:
  ```

### 使用现成的 Conan docker 镜像

```bash
#run x86_64 docker image
docker run -d zvicii/gcc7-x86_64-ubuntu16.04-jenkins -url jenkins_url_here jenkins_agent_token Linux-x86_64-agent

#run ARM64 docker image
docker run -d zvicii/gcc7-arm64-ubuntu16.04-jenkins -url jenkins_url_here jenkins_agent_token Linux-arm64-agent
```

[gcc7-x86_64-ubuntu16.04-jenkins](https://hub.docker.com/repository/docker/zvicii/gcc7-x86_64-ubuntu16.04-jenkins)

- os: ubuntu16.04
- compiler: gcc7.5
- glibc: 2.23

```dockerfile
FROM conanio/gcc7-ubuntu16.04-jenkins

RUN sudo pip3 install --upgrade cmake
RUN sudo apt-get -q update
RUN curl -fsSL https://deb.nodesource.com/setup_16.x | sudo -E bash -
RUN sudo apt-get -q -y install nodejs openssh-client
RUN sudo apt-get -q -y install libgl1-mesa-dev libx11-dev libx11-xcb-dev libfontenc-dev libice-dev libsm-dev libxau-dev libxaw7-dev libxcomposite-dev libxcursor-dev libxdamage-dev libxdmcp-dev libxext-dev libxfixes-dev libxi-dev libxinerama-dev libxkbfile-dev libxmu-dev libxmuu-dev libxpm-dev libxrandr-dev libxrender-dev libxres-dev libxss-dev libxt-dev libxtst-dev libxv-dev libxvmc-dev libxxf86vm-dev xtrans-dev libxcb-render0-dev libxcb-render-util0-dev libxcb-xkb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev xkb-data libxcb-dri3-dev uuid-dev libxcb-util-dev
RUN mkdir ~/workspace
RUN sudo npm install @yxfe/nepublisher -g --registry=http://npm.netease.im/
RUN conan remote add NetEaseConan http://yunxin-conan.netease.im:8082/artifactory/api/conan/NetEaseConan

CMD sudo chown -R conan ~/workspace ~/.conan
```

[gcc7-arm64-ubuntu16.04-jenkins](https://hub.docker.com/repository/docker/zvicii/gcc7-arm64-ubuntu16.04-jenkins)

- os: ubuntu16.04
- compiler: linaro aarch64-linux-gnu gcc 7.5
- glibc: 2.23

```dockerfile
FROM conanio/gcc7-ubuntu16.04-jenkins

LABEL maintainer="zvicii@qq.com"

ENV CC=aarch64-linux-gnu-gcc \
    CXX=aarch64-linux-gnu-g++ \
    CMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
    CMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ \
    STRIP=aarch64-linux-gnu-strip \
    RANLIB=aarch64-linux-gnu-ranlib \
    AS=aarch64-linux-gnu-as \
    AR=aarch64-linux-gnu-ar \
    LD=aarch64-linux-gnu-ld \
    FC=aarch64-linux-gnu-gfortran

RUN mkdir ~/aarch64-linux-gnu ~/workspace
RUN sudo wget -c https://releases.linaro.org/components/toolchain/binaries/7.5-2019.12/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz -O ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
RUN sudo tar -xf ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz -C ~/aarch64-linux-gnu
RUN sudo wget -c https://releases.linaro.org/components/toolchain/binaries/6.5-2018.12/aarch64-linux-gnu/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz -O ~/aarch64-linux-gnu/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz
RUN sudo tar -xf ~/aarch64-linux-gnu/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu.tar.xz -C ~/aarch64-linux-gnu
RUN sudo rm -rf /home/conan/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/aarch64-linux-gnu/libc
RUN sudo cp -r /home/conan/aarch64-linux-gnu/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu/aarch64-linux-gnu/libc /home/conan/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/aarch64-linux-gnu
RUN sudo rm -rf /home/conan/aarch64-linux-gnu/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu
RUN sudo update-alternatives --install /usr/bin/aarch64-linux-gnu-gcc aarch64-linux-gnu-gcc ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc 100
RUN sudo update-alternatives --install /usr/bin/aarch64-linux-gnu-g++ aarch64-linux-gnu-g++ ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++ 100
RUN sudo update-alternatives --install /usr/bin/aarch64-linux-gnu-gcov aarch64-linux-gnu-gcov ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcov 100
RUN sudo update-alternatives --install /usr/bin/aarch64-linux-gnu-gcov-dump aarch64-linux-gnu-gcov-dump ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcov-dump 100
RUN sudo update-alternatives --install /usr/bin/aarch64-linux-gnu-gcov-tool aarch64-linux-gnu-gcov-tool ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcov-tool 100
RUN sudo update-alternatives --install /usr/bin/aarch64-linux-gnu-ar aarch64-linux-gnu-ar ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-ar 100
RUN sudo update-alternatives --install /usr/bin/aarch64-linux-gnu-as aarch64-linux-gnu-as ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-as 100
RUN sudo update-alternatives --install /usr/bin/aarch64-linux-gnu-ld aarch64-linux-gnu-ld ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-ld 100
RUN sudo update-alternatives --install /usr/bin/aarch64-linux-gnu-ranlib aarch64-linux-gnu-ranlib ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-ranlib 100
RUN sudo update-alternatives --install /usr/bin/aarch64-linux-gnu-gfortran aarch64-linux-gnu-gfortran ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gfortran 100
RUN sudo update-alternatives --install /usr/bin/aarch64-linux-gnu-strip aarch64-linux-gnu-strip ~/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-strip 100
RUN conan profile new default --detect --force
RUN conan profile update settings.arch=armv8 default
RUN sudo pip3 install --upgrade cmake
RUN sudo apt-get -q update
RUN sudo curl -fsSL https://deb.nodesource.com/setup_16.x | sudo -E bash -
RUN sudo apt-get -q -y install nodejs openssh-client
RUN sudo apt-get -q -y install libgl1-mesa-dev libx11-dev libx11-xcb-dev libfontenc-dev libice-dev libsm-dev libxau-dev libxaw7-dev libxcomposite-dev libxcursor-dev libxdamage-dev libxdmcp-dev libxext-dev libxfixes-dev libxi-dev libxinerama-dev libxkbfile-dev libxmu-dev libxmuu-dev libxpm-dev libxrandr-dev libxrender-dev libxres-dev libxss-dev libxt-dev libxtst-dev libxv-dev libxvmc-dev libxxf86vm-dev xtrans-dev libxcb-render0-dev libxcb-render-util0-dev libxcb-xkb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev xkb-data libxcb-dri3-dev uuid-dev libxcb-util-dev
RUN sudo npm install @yxfe/nepublisher -g --registry=http://npm.netease.im/
RUN conan remote add NetEaseConan http://yunxin-conan.netease.im:8082/artifactory/api/conan/NetEaseConan

CMD sudo chown -R conan ~/workspace ~/.conan
```

### 编译 Linux 产物

调用 cmake configure 即可完成产物交叉编译构建。如：

```bash
cmake -Bbuild -G'Unix Makefiles' -DCMAKE_BUILD_TYPE=Release \
  -DCONAN_PROFILE_BUILD=$(pwd)/.profiles/linux-x86_64 \
  -DCONAN_PROFILE_HOST=$(pwd)/.profiles/linux-arm64-gcc7 \
  -DCMAKE_INSTALL_PREFIX=exports
```

## Android

Android 的交叉编译目前是相对难以理解和配置的，并且官方一直期望向 Conan 2.0 迁移，对旧版的包对 Android 交叉编译得兴趣不是特别积极。但这并不是没有办法。为了解决 Android 交叉编译得问题，我们不得不在本机安装一下 NDK，用以给我们当前的 CMake 工程传递工具链信息。其他的就全部交给 Conan 了。以下为 Android 各架构的 profile 配置信息：

### arm64-v8a

```
[settings]
arch=armv8
compiler=clang
compiler.libcxx=c++_static
compiler.cppstd=14
compiler.version=9
os=Android
os.api_level=21

[tool_requires]
android-ndk/r21e
```

与前面 iOS 和 Linux 不同的是，我们修改了 compiler.libcxx 为 `c++_static`，这也是目前支持最好的选择。值得注意的是 `os.api_level=21` 表示支持的 Android 目标最低 API 级别。

另外一点比较特殊的是，我们使用了 `tool_requires` 来引入了 NDK r21e 的包，该包为 Conan 官方维护，想前一章节介绍的一样，tool_requires 只会在本机环境进行编译为目标环境做支撑。它是一个完整的 NDK 包，会从 Conan center 下载下来并完成编译各架构的工具链。这些工具链信息会无缝传递给 Conan 管理的三方库。

::: tip 提示
虽然 Conan center 提供了 NDK 但它下载而来的 NDK 等环境变量信息无法传递给我们的 CMake 工程，这也就是为什么我们要自己安装一份 NDK 来解决当前工程环境配置的问题。
:::

### armeabi-v7a

```
[settings]
arch=armv7
compiler=clang
compiler.libcxx=c++_static
compiler.cppstd=14
compiler.version=9
os=Android
os.api_level=21

[tool_requires]
android-ndk/r21e
```

### x86_64

```
[settings]
arch=x86_64
compiler=clang
compiler.libcxx=c++_static
compiler.cppstd=14
compiler.version=9
os=Android
os.api_level=21

[tool_requires]
android-ndk/r21e
```

### x86

```
[settings]
arch=x86
compiler=clang
compiler.libcxx=c++_static
compiler.cppstd=14
compiler.version=9
os=Android
os.api_level=21

[tool_requires]
android-ndk/r21e
```

### 编译 Android 产物

与其他平台一样，唯一不同的就是传递给当前工程的配置信息要修改为 Android，并且 API level 等信息也要正确的传递进去。如：

```diff
cmake -Barm64-v8a -DCMAKE_BUILD_TYPE=Release \
  -DCONAN_PROFILE_BUILD=default \
  -DCONAN_PROFILE_HOST=$(pwd)/.profiles/android-arm64-v8a-abi21 \
  -DCMAKE_SYSTEM_NAME=Android \
  -DCMAKE_SYSTEM_VERSION=21 \
  -DCMAKE_ANDROID_STL_TYPE=c++_static \
  -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a \
  -DCMAKE_ANDROID_NDK=~/Library/Android/sdk/ndk/21.4.7075529
```

- `CMAKE_SYSTEM_NAME` 修改为 Android
- `CMAKE_SYSTEM_VERSION` 为 Android API level
- `CMAKE_ANDROID_STL_TYPE` 为 Android STL 标准
- `CMAKE_ANDROID_ARCH_ABI` 为 Android 架构信息
- `CMAKE_ANDROID_NDK` 为本机安装的 NDK 路径

::: tip 提示
DCMAKE_ANDROID_NDK 的路径也可以复用 Conan 下载的 NDK 路径，如 ~/.conan/data/android-ndk/...
:::

## Jenkins CI

当各平台编译 profile 完善后，我们就可以接入 CI 来完成出包工作。在不同平台下，我们的统一入口文件就是 CMake，如果您适当的实现了 CMake install 流程。那么打包基本上是全自动化的，无需任何三方脚本支持。以 iOS 和 Android 平台举例：

该脚本实现了 iOS 3 个架构的全自动编译流程，你可以看到，根本没有执行任何 conan 指令和编译三方库的指令，全部通过 CMake 介入。

```bash
script {
    def outputFileName = "nim-${env.STAGE_NAME}-${packageFullVersion}"
    def architectures = [
        "arm64": "iphoneos",
        "armv7": "iphoneos",
        "x86_64": "iphonesimulator",
    ]
    sh "git clean -xdf && mkdir -p exports/x86_64 exports/armv7 exports/arm64 exports/universal"
    def work = [:]
    for (kv in mapToList(architectures)) {
        def localProile = sh(script: "echo \$(pwd)/.profiles/darwin-localhost-10.14", returnStdout: true).trim()
        def hostProfile = sh(script: "echo \$(pwd)/.profiles/ios-${kv[0]}-${kv[1]}", returnStdout: true).trim()
        sh "cmake -B${kv[0]} -GXcode \
            -DCMAKE_BUILD_TYPE=Release \
            -DCONAN_PROFILE_BUILD=${localProile} \
            -DCONAN_PROFILE_HOST=${hostProfile} \
            -DCMAKE_SYSTEM_NAME=iOS \
            -DCMAKE_OSX_DEPLOYMENT_TARGET=9.3 \
            -DCMAKE_OSX_SYSROOT=${kv[1]} \
            -DCMAKE_OSX_ARCHITECTURES=${kv[0]}"
        sh "cmake --build ${kv[0]} --config Release --target install --parallel"
    }
    // C++ framework
    sh "cp -R exports/x86_64/nim.framework exports/universal"
    sh "lipo -create exports/arm64/nim.framework/nim \
        exports/armv7/nim.framework/nim \
        exports/x86_64/nim.framework/nim \
        -output exports/universal/nim.framework/nim"
    sh "cd exports && tar -zcvf ${outputFileName}-cpp.tar.gz universal/nim.framework && cd -"
    archiveArtifacts artifacts: "exports/${outputFileName}-cpp.tar.gz", followSymlinks: false, onlyIfSuccessful: true
    // C++ framework dSYM
    sh "cp -R x86_64/lib/nim.framework.dSYM exports/universal"
    sh "lipo -create arm64/lib/nim.framework.dSYM/Contents/Resources/DWARF/nim \
        armv7/lib/nim.framework.dSYM/Contents/Resources/DWARF/nim \
        x86_64/lib/nim.framework.dSYM/Contents/Resources/DWARF/nim \
        -output exports/universal/nim.framework.dSYM/Contents/Resources/DWARF/nim"
    sh "cd exports && tar -zcvf ${outputFileName}-cpp-dSYM.tar.gz universal/nim.framework.dSYM && cd -"
    archiveArtifacts artifacts: "exports/${outputFileName}-cpp-dSYM.tar.gz", followSymlinks: false, onlyIfSuccessful: true
}
```

同样的，Android 平台也可使用同样手段来完成工作：

```bash
script {
    def outputFileName = "nim-${env.STAGE_NAME}-${packageFullVersion}"
    def architectures = [
        "arm64-v8a": "android-arm64-v8a-abi21",
        "armeabi-v7a": "android-armeabi-v7a-abi21",
        "x86_64": "android-x86_64-abi21",
        "x86": "android-x86-abi21"
    ]
    sh "git clean -xdf && mkdir -p exports/arm64-v8a exports/armeabi-v7a exports/x86_64 exports/x86"
    def work = [:]
    for (kv in mapToList(architectures)) {
        def localProile = sh(script: "echo \$(pwd)/.profiles/darwin-localhost-10.14", returnStdout: true).trim()
        def hostProfile = sh(script: "echo \$(pwd)/.profiles/${kv[1]}", returnStdout: true).trim()
        sh "cmake -B${kv[0]} \
            -DCMAKE_BUILD_TYPE=Release \
            -DCONAN_PROFILE_BUILD=${localProile} \
            -DCONAN_PROFILE_HOST=${hostProfile} \
            -DCMAKE_SYSTEM_NAME=Android \
            -DCMAKE_SYSTEM_VERSION=21 \
            -DCMAKE_ANDROID_STL_TYPE=c++_static \
            -DCMAKE_ANDROID_ARCH_ABI=${kv[0]} \
            -DCMAKE_ANDROID_NDK=${env.HOME}/Library/Android/sdk/ndk/21.4.7075529"
        sh "cmake --build ${kv[0]} --config Release --target install --parallel"
    }
    sh "cd exports && tar -zcvf ${outputFileName}-cpp.tar.gz * && cd -"
    archiveArtifacts artifacts: "exports/${outputFileName}-cpp.tar.gz", followSymlinks: false, onlyIfSuccessful: true
}
```

::: tip 提示
以上为代码片段为提供简单的 Jenkins pipeline 脚本供大家参考，更加完整的示例可参考 IM Elite SDK：[https://g.hz.netease.com/nim2/mini](https://g.hz.netease.com/nim2/mini)
:::
