# -*- coding: utf-8 -*-
# @Time    : 2019/3/25 19:11
# @Author  : dengkai17334
# @File    : __init__.py

import sys
from types import ModuleType

__version__ = "1.0.0.dev0"

# This import magic raises concerns quite often which is why the implementation
# and motivation is explained here in detail now.
#
# The majority of the functions and classes provided by lazy_import work on the
# HTTP and WSGI layer.  There is no useful grouping for those which is why
# they are all importable from "lazy_import" instead of the modules where they are
# implemented.  The downside of that is, that now everything would be loaded at
# once, even if unused.
#
# The implementation of a lazy-loading module in this file replaces the
# lazy_import package when imported from within.  Attribute access to the lazy_import
# module will then lazily import from the modules that implement the objects.

# import mapping to objects in other modules
all_by_module = {
    "lazy_import.m": ["M", "M2"]

}

object_origins = {}
for module, items in all_by_module.items():
    for item in items:
        object_origins[item] = module


class module(ModuleType):
    """Automatically import objects from the modules."""

    def __getattr__(self, name):
        if name in object_origins:
            module = __import__(object_origins[name], None, None, [name])
            for extra_name in all_by_module[module.__name__]:
                setattr(self, extra_name, getattr(module, extra_name))
            return getattr(module, name)
        return ModuleType.__getattribute__(self, name)

    def __dir__(self):
        """Just show what we want to show."""
        result = list(new_module.__all__)
        result.extend(
            (
                "__file__",
                "__doc__",
                "__all__",
                "__docformat__",
                "__name__",
                "__path__",
                "__package__",
                "__version__",
            )
        )
        return result


# keep a reference to this module so that it's not garbage collected
old_module = sys.modules[__name__]

new_module = sys.modules[__name__] = module(__name__)

new_module.__dict__.update({
    '__file__': __file__,
    "__package__": "lazy_import",
    '__path__': __path__,
    '__doc__': __doc__,
    '__all__': tuple(object_origins),
    '__version__': __version__,
    "__docformat__": "restructuredtext en",
})
