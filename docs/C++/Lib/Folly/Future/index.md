# folly future



## github [Futures.md](https://github.com/facebook/folly/blob/main/folly/docs/Futures.md)

The primary difference from std::future is that you can attach callbacks to Futures (with `thenValue` or `thenTry`), under the control of an executor to manage where work runs, which enables sequential and parallel composition of Futures for cleaner asynchronous code.

> NOTE: 
>
> 可以总结为: folly-future-via-executor-pattern



For safety, `.via` should be preferred. We can chain `.via` operations to give very strong control over where callbacks run:

```c++
std::move(aFuture)
  .thenValue(x)
  .via(e1).thenValue(y1).thenValue(y2)
  .via(e2).thenValue(z);
```

`x` will execute in the context of the executor associated with `aFuture`. `y1` and `y2` will execute in the context of `e1`, and `z` will execute in the context of `e2`. If after `z` you want to get back to the original context, you need to get there with a call to `via` passing the original executor.



## engineering.fb [Futures for C++11 at Facebook](https://engineering.fb.com/2015/06/19/developer-tools/futures-for-c-11-at-facebook/)

```C++
template <typename T> using Callback = std::function<void(T)>;

Output fooSync(Input);
void fooAsync(Input, Callback<Output>);
```



```c++
#include <folly/futures/Future.h> 
using folly::Future;

// Do foo asynchronously; immediately return a Future for the output
Future<Output> fooFuture(Input);

Future<Output> f = fooFuture(input);
// f may not have a value (or exception) yet. But eventually it will.
f.isReady();  // Maybe, maybe not.
f.wait();     // You can synchronously wait for futures to become ready.
f.isReady();  // Now this is guaranteed to be true.
Output o = f.value();  // If f holds an exception, this will throw that exception.
```





```C++
Future<double> f = 
  fooFuture(input)
  .then([](Output o) {
    return o * M_PI;
  })
  .onError([](std::exception const& e) {
    cerr << "Oh bother, " << e.what()
      << ". Returning pi instead." << endl;
    return M_PI;
  });

// get() first waits, and then returns the value
cout << "Result: " << f.get() << endl;
```

### Sequential composition



```C++
// the callback pyramid is syntactically annoying

void asyncA(Output, Callback<OutputA>);
void asyncB(OutputA, Callback<OutputB>);
void asyncC(OutputB, Callback<OutputC>);
void asyncD(OutputC, Callback<OutputD>);

auto result = std::make_shared<double>();
fooAsync(input, [=](Output output) {
  // ...
  asyncA(output, [=](OutputA outputA) {
    // ...
    asyncB(outputA, [=](OutputB outputB) {
      // ...
      asyncC(outputB, [=](OutputC outputC) {
        // ...
        asyncD(outputC, [=](OutputD outputD) {
          *result = outputD * M_PI;
        });
      });
    });
  });
});

// As an exercise for the masochistic reader, express the same thing without
// lambdas. The result is called callback hell.
```

> NOTE: 
>
> 在回调中，在调用另外一个函数

With futures, sequentially composed using `then`, the code cleans up nicely:

```clike
Future<OutputA> futureA(Output);
Future<OutputB> futureB(OutputA);
Future<OutputC> futureC(OutputB);
// then() automatically lifts values (and exceptions) into a Future.
OutputD d(OutputC) {
  if (somethingExceptional) throw anException;
  return OutputD();
}

Future<double> fut =
  fooFuture(input)
  .then(futureA)
  .then(futureB)
  .then(futureC)
  .then(d)
  .then([](OutputD outputD) { // lambdas are ok too
    return outputD * M_PI;
  });
```


