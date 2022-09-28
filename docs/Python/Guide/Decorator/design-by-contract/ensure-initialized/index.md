

```c++
def ensure_nertc_initialized(func):
    def wrapper(*args, **kwargs):
        if args[0].nertc_initialized:
            return func(*args, **kwargs)
        else:
            qt_message_box("nertc未初始化,请先初始化", icon=QMessageBox.Critical)

    return wrapper

```

