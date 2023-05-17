# cppreference [Pseudo-random number generation](https://en.cppreference.com/w/cpp/numeric/random)

The random number library provides classes that generate random and pseudo-random numbers. These classes include:

1、Uniform random bit generators (URBGs), which include both **random number engines**, which are pseudo-random number generators that generate integer sequences with a uniform distribution, and **true random number generators** if available;

> NOTE:
>
> 一、包括:
>
> 1、**random number engines**
>
> 2、**true random number generators** 

2、Random number distributions (e.g. [uniform](https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution), [normal](https://en.cppreference.com/w/cpp/numeric/random/normal_distribution), or [poisson distributions](https://en.cppreference.com/w/cpp/numeric/random/poisson_distribution)) which convert the output of **URBGs** into various **statistical distributions**.



**URBGs** and **distributions** are designed to be used together to produce random values. All of the **random number engines** may be specifically seeded, serialized, and de-serialized for use with repeatable simulators.

> NOTE:
>
> 一、简而言之，包括:
>
> 1、**URBGs**
>
> 2、**distributions** 
>
> 使用的时候，同时需要两者



## Uniform random bit generators

A *uniform random bit generator* is a function object returning unsigned integer values such that each value in the range of possible results has (ideally) equal probability of being returned.

### Random number engines



### Random number engine adaptors



### Predefined random number generators

| Type                   | Definition                                                   |
| ---------------------- | ------------------------------------------------------------ |
| `minstd_rand0` (C++11) | [std::linear_congruential_engine](https://en.cppreference.com/w/cpp/numeric/random/linear_congruential_engine)<[std::uint_fast32_t](https://en.cppreference.com/w/cpp/types/integer), 16807, 0, 2147483647> Discovered in 1969 by Lewis, Goodman and Miller, adopted as "Minimal standard" in 1988 by Park and Miller |
| `mt19937`(C++11)       |                                                              |

### Non-deterministic random numbers

[std::random_device](https://en.cppreference.com/w/cpp/numeric/random/random_device) is a non-deterministic uniform random bit generator, although implementations are allowed to implement [std::random_device](https://en.cppreference.com/w/cpp/numeric/random/random_device) using a pseudo-random number engine if there is no support for non-deterministic random number generation.



## Random number distributions

A random number distribution **post-processes** the output of a URBG in such a way that resulting output is distributed according to a defined statistical probability density function.

Random number distributions satisfy [*RandomNumberDistribution*](https://en.cppreference.com/w/cpp/named_req/RandomNumberDistribution).
