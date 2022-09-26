# final annotation



```python

try:
    from typing import final
except Exception:
    def final(f):
        """A decorator to indicate final methods and final classes.
        Use this decorator to indicate to type checkers that the decorated
        method cannot be overridden, and decorated class cannot be subclassed.
        For example:
          class Base:
              @final
              def done(self) -> None:
                  ...
          class Sub(Base):
              def done(self) -> None:  # Error reported by type checker
                    ...
          @final
          class Leaf:
              ...
          class Other(Leaf):  # Error reported by type checker
              ...
        There is no runtime checking of these properties.
        """
        return f

```

