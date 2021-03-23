# Custom filter iterator

## stackoverflow [iterators of a vector filtered?](https://stackoverflow.com/questions/44350760/iterators-of-a-vector-filtered)



[A](https://stackoverflow.com/a/44350827)

The answer is certainly "yes." C++ iterators in the STL style can be made to do all sorts of tricks. A common but basic one is making an iterator for `std::map` which when dereferenced gives only the key or the value.

In your particular case, a simple implementation might be like this:

```cpp
template <typename BaseIterator>
struct filtered_iterator : BaseIterator
{
    typedef std::function<bool (const value_type&)> filter_type;

    filtered_iterator() = default;
    filtered_iterator(filter_type filter, BaseIterator base, BaseIterator end = {})
        : BaseIterator(base), _end(end), _filter(filter_type) {
        while (*this != _end && !_filter(**this)) {
            ++*this;
        }
    }

    filtered_iterator& operator++() {
        do {
            BaseIterator::operator++();
        } while (*this != _end && !_filter(**this));
    }

    filtered_iterator operator++(int) {
        filtered_iterator copy = *this;
        ++*this;
        return copy;
    }

private:
    BaseIterator _end;
    filter_type _filter;
};

template <typename BaseIterator>
filtered_iterator<BaseIterator> make_filtered_iterator(
        typename filtered_iterator<BaseIterator>::filter_type filter,
        BaseIterator base, BaseIterator end = {}) {
    return {filter, base, end};
}
```

I set a default value for `end` because typically you can use a default-constructed iterator for that. But in some cases you might want to filter only a subset of the container, in which case specifying the end makes it easy.

> NOTE: 
>
> 1、非常好的implementation