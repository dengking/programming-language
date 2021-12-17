# [`enum`](https://docs.python.org/3/library/enum.html#module-enum) — Support for enumerations[¶](https://docs.python.org/3/library/enum.html#module-enum)





## bitmask enum

### stackoverflow [Is there a Python class/enum for flag/bit mask operations?](https://stackoverflow.com/questions/36829820/is-there-a-python-class-enum-for-flag-bit-mask-operations)

[A](https://stackoverflow.com/a/42642901/10173843)

Python 3.6 has added [`Flag`](https://docs.python.org/3/library/enum.html#flag) and [`IntFlag`](https://docs.python.org/3/library/enum.html#intflag) which support the usual bit-wise operations. As a bonus, the resulting values from the bit-wise operations are still members of the original flag class, and are singletons [1].

> NOTE: 
>
> 总结得非常好

### magnussen [Enum vs Flag for bitmasks in Python](https://blog.magnussen.casa/post/using-enum-as-bitmasks-in-python/)

#### Bitmask filtering in Python

**Using Python 3.6+**

```Python

from enum import IntFlag, auto


class Permissions(IntFlag):
    EXECUTE = auto()  # 2^0
    WRITE = auto()  # 2^1
    READ = auto()  # 2^2


class PermissionSet:
    def __init__(self, *flags):
        """

        :param flags: flag是小写的字符串
        """
        self._permission = Permissions(0)  # Initiate no permissions，它的值是0
        for flag in flags:
            self._permission |= Permissions[flag.upper()]

    def __contains__(self, item):
        """
        检查 self._permission 中是否包含 item
        :param item:
        :return:
        """
        return (self._permission & item._permission) == item._permission


class File:
    def __init__(self, name, *, permission):
        self.name = name
        self._permissions = PermissionSet(*permission)


def test():
    # A filter:
    permission_filter = PermissionSet("read", "execute")
    # The dummy files:
    files = []
    files.append(File("file_rw", permission=("read", "write")))  # Should be excluded from filter
    files.append(File("file_re", permission=("read", "execute")))  # Should be included from filter
    files.append(File("file_w", permission=("read", "execute")))  # Should be included from filter
    files.append(File("file_we", permission=("write", "execute")))  # Should be excluded from filter
    files.append(File("file_rwe", permission=("read", "write", "execute")))  # Should be included from filter
    for f in filter(lambda f: permission_filter in f._permissions, files):  # 筛选出具有 "read"、 "execute" 权限的文件
        print(f.name)

```

