# Generating compilation database from GN



## groups.google [Generating compilation database from GN](https://groups.google.com/a/chromium.org/g/gn-dev/c/uzQJX4-zKXo)

```shell
ninja -C out/Default -t compdb cc cxx
```

> NOTE:
>
> 通过上述command是能够得到compilation database的

