# cpython的build system

使用的是[GNU Autotools](https://en.wikipedia.org/wiki/GNU_Autotools)





如何使用[Autoconf](https://www.gnu.org/software/autoconf/)来生成configure文件？是的，在1.5. Regenerate `configure`[¶](https://devguide.python.org/setup/#regenerate-configure)中，有这样的描述：

> Python’s `configure` script is generated from `configure.ac` using Autoconf. Instead of editing `configure`, edit `configure.ac` and then run `autoreconf` to regenerate `configure` and a number of other files (such as `pyconfig.h`).

关于configure文件，参见：[configure script](https://en.wikipedia.org/wiki/Configure_script)。



`Makefile.pre`是由`configure`生成的。

## `pyconfig.h`

```c
/* pyconfig.h.  Generated from pyconfig.h.in by configure.  /
/ pyconfig.h.in.  Generated from configure.ac by autoheader.  */
```

