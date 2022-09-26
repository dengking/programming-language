

# function logger

### stackoverflow [How to do logging at function entry, inside and exit in Python](https://stackoverflow.com/questions/23435488/how-to-do-logging-at-function-entry-inside-and-exit-in-python)



## Example



```Python

def log_build(name):
    """
    decorator for CmakeBasedBuilderBase.build
    :param name:
    :return:
    """

    def decorator(func):
        def wrapper(*args, **kwargs):
            logging.info('----------------------begin build ' + name + '----------------------')
            ret = func(*args, **kwargs)
            logging.info('----------------------end build ' + name + '----------------------')
            return ret

        return wrapper

    return decorator


def log_build_one_arch(func):
    """
    decorator for CmakeBasedBuilderBase.build_one_arch

    :param func:
    :return:
    """

    def wrapper(*args, **kwargs):
        obj = args[0]
        arch = args[1]
        build_type = args[2]
        msg = obj.get_build_info(arch, build_type)
        logging.info('----------------------begin build ' + msg + '----------------------')
        ret = func(*args, **kwargs)
        logging.info('----------------------end build ' + msg + '----------------------')
        return ret

    return wrapper

```

