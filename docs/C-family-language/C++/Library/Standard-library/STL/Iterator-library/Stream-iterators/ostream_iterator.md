# `ostream_iterator`

把这两种output stream iterator放到一起来进行介绍。

## cppreference [std::ostream_iterator](https://en.cppreference.com/w/cpp/iterator/ostream_iterator)





## cppreference [std::ostreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/ostreambuf_iterator)



## [std::ostream_iterator](https://en.cppreference.com/w/cpp/iterator/ostream_iterator) VS [std::ostreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/ostreambuf_iterator)

在[std::ostream_iterator](https://en.cppreference.com/w/cpp/iterator/ostream_iterator)中有这样的描述：

> When writing characters, [std::ostreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/ostreambuf_iterator) is more efficient, since it avoids the overhead of constructing and destructing the sentry object once per character.

显然[std::ostream_iterator](https://en.cppreference.com/w/cpp/iterator/ostream_iterator)需要sentry object，那sentry object是什么呢？

而[std::ostreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/ostreambuf_iterator)是通过：

> a boolean flag indicating if the the **end of file condition** has been reached