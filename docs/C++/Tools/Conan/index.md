# [Conan](https://docs.conan.io/en/latest/introduction.html) 

Conan is a dependency and package manager for C and C++ languages.

> NOTE: 
>
> 一、上述package是什么含义？

## Decentralized package manager

[![_images/conan-systems.png](https://docs.conan.io/en/latest/_images/conan-systems.png)](https://docs.conan.io/en/latest/_images/conan-systems.png)

## Binary management

One of the most powerful features of Conan is that it can create and manage pre-compiled binaries for any possible platform and configuration.

A package is defined by a “conanfile.py”. This is a file that defines the package’s dependencies, sources, how to build the binaries from sources, etc. One package “conanfile.py” recipe can generate any arbitrary number of binaries, one for each different platform and configuration: operating system, architecture, compiler, build type, etc. These binaries can be created and uploaded to a server with the same commands in all platforms, having a single source of truth for all packages and not requiring a different solution for every different operating system.

## [Getting Started](https://docs.conan.io/en/latest/getting_started.html)