# cmake clean

## stackoverflow [Looking for a 'cmake clean' command to clear up CMake output](https://stackoverflow.com/questions/9680420/looking-for-a-cmake-clean-command-to-clear-up-cmake-output)

### [A](https://stackoverflow.com/a/9680493)

#### CMake 3.X

CMake 3.X offers a 'clean' target.

```shell
cmake --build C:/foo/build/ --target clean
```

From the CMake docs for 3.0.2:

```shell
--clean-first  = Build target 'clean' first, then build.
                 (To clean only, use --target 'clean'.)
```

#### CMake 2.X

There is no `cmake clean` in CMake version 2.X

I usually build the project in a single folder like "build". So if I want to `make clean`, I can just `rm -rf build`.

The "build" folder in the same directory as the root "CMakeLists.txt" is usually a good choice. To build your project, you simply give cmake the location of the CMakeLists.txt as an argument. For example: `cd <location-of-cmakelists>/build && cmake ..`. (From @ComicSansMS)