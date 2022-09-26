

```Python

class CmakeBasedBuilderFactory(object):
    """
    这是仿照下面文章:
    https://medium.com/@geoffreykoh/implementing-the-factory-pattern-via-dynamic-registry-and-python-decorators-479fc1537bbe

    "The factory class for creating builders"

    """

    registry = {}
    """ Internal registry for available builders """

    @classmethod
    def register(cls, name):
        def inner_wrapper(wrapped_class):
            if name in cls.registry:
                logging.warning('Builder %s already exists. Will replace it', name)
            cls.registry[name] = wrapped_class
            return wrapped_class

        return inner_wrapper

    # end register()

    @classmethod
    def create_builder(cls, name, *args, **kwargs):
        """ Factory command to create the builder.
        This method gets the appropriate Builder class from the registry
        and creates an instance of it, while passing in the parameters
        given in ``kwargs``.
        Args:
            name (str): The name of the executor to create.
        Returns:
            An instance of the executor that is created.
        """

        if name not in cls.registry:
            logging.warning('Executor %s does not exist in the registry', name)
            return None

        exec_class = cls.registry[name]
        executor = exec_class(*args, **kwargs)
        return executor

    # end create_builder()
```

