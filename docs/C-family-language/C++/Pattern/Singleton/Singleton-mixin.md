# Singleton mixin C++



## stackoverflow [Singleton mixin C++](https://stackoverflow.com/questions/10221522/singleton-mixin-c)

**Summary:** How can I create a singleton mixin in C++? I am trying to avoid copying the same `get_instance()` function, the private constructor, etc. But I can't figure out a way to make this a mixin, because the static instance will be shared by everything that inherits from the mixin.