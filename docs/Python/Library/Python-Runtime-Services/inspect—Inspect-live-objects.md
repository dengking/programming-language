# [`inspect`](https://docs.python.org/3/library/inspect.html#module-inspect) — Inspect live objects[¶](https://docs.python.org/3/library/inspect.html#module-inspect)



## Application

### logging

#### [How to determine file, function and line number?](https://stackoverflow.com/questions/6810999/how-to-determine-file-function-and-line-number)

```python
import inspect

def PrintFrame():
  callerframerecord = inspect.stack()[1]    # 0 represents this line
                                            # 1 represents line at caller
  frame = callerframerecord[0]
  info = inspect.getframeinfo(frame)
  print(info.filename)                      # __FILE__     -> Test.py
  print(info.function)                      # __FUNCTION__ -> Main
  print(info.lineno)                        # __LINE__     -> 13

def Main():
  PrintFrame()                              # for this line

Main()
```

