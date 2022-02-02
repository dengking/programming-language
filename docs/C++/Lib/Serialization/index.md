# Serialization and deserialization 

## stackoverflow [Is it possible to serialize and deserialize a class in C++?](https://stackoverflow.com/questions/234724/is-it-possible-to-serialize-and-deserialize-a-class-in-c)

[A](https://stackoverflow.com/a/235256)

I recommend Google [protocol buffers](http://code.google.com/p/protobuf/). 

[A](https://stackoverflow.com/a/234816)

I wrote an article describing a rudimentary (but still powerful) serialization system. You may find it interesting: [Using SQLite as an On-disk File Format, Part 2](http://replay.waybackmachine.org/20090412173303/http://www.praeclarum.org/2006/03/using-sqlite-as-on-disk-file-format_31.html).

[A](https://stackoverflow.com/a/22122547)

I encourage anyone who has access to C++11 to take a look at [cereal](http://uscilab.github.io/cereal/), a C++11 header only library for serialization that supports binary, JSON, and XML out of the box. cereal was designed to be easy to extend and use and has a similar syntax to Boost.

[A](https://stackoverflow.com/a/234740)

The [`Boost::serialization`](http://www.boost.org/doc/libs/1_36_0/libs/serialization/doc/index.html) library handles this rather elegantly. I've used it in several projects. There's an example program, showing how to use it, [here](http://www.boost.org/doc/libs/1_36_0/libs/serialization/example/demo.cpp).