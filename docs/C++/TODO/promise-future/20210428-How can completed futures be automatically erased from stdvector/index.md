# [How can completed futures be automatically erased from std::vector](https://stackoverflow.com/questions/45117450/how-can-completed-futures-be-automatically-erased-from-stdvector)

```C++
void RaiseEvent(EventID messageID)
{
    mEventExecutors.push_back(std::move(std::async([=]{
            auto eventObject = mEventListeners.find(messageID);
            if (eventObject != mEventListeners.end())
            {
                for (auto listener : eventObject->second)
                {
                    listener();
                }
            }
        })
    ));
}
```



[A](https://stackoverflow.com/a/45117709)

You need to be careful though, your code currently has a lot of data races. Consider using another mutex or some other technique to prevent them.

```cpp
std::thread{[=]() {
    // Task is running...
    auto eventObject = mEventListeners.find(messageID);
    if (eventObject != mEventListeners.end())
    {
        for (auto listener : eventObject->second)
        {
            listener();
        }
    }
}.detach(); // detach thread so that it continues
```



[A](https://stackoverflow.com/a/45120652)

> NOTE:
>
> 1、非常好的example

The question itself has been answer by another, but it piqued my curiosity as to how one could implement a fully functional, thread-safe task manager in a minimum number of code lines.

I also wondered whether it would be possible to wait on the tasks as futures, or optionally provide a callback function.

Then of course this begged the question whether those futures could use the sexy continuation syntax of `.then(xxx)` rather than blocking the code.

Here is my attempt.

Much kudos to Christopher Kohlhoff, the author of `boost::asio`. By studying his awesome work, I learned the value of separating classes into:

- handle - controls the lifetime of the object
- service - provides object logic, state shared amongst object impls, and manages the lifetimes of implementation objects should they outlive the handle (anything that relies on a callback usually does), and
- implementation provides per-object state.

So here's an example of calling the code:

```cpp
int main() {
    task_manager mgr;

    // an example of using async callbacks to indicate completion and error
    mgr.submit([] {
                   emit("task 1 is doing something");
                   std::this_thread::sleep_for(1s);
                   emit("task 1 done");
               },
               [](auto err) {
                   if (not err) {
                       emit("task 1 completed");
                   } else {
                       emit("task 1 failed");
                   }
               });

    // an example of returning a future (see later)
    auto f = mgr.submit([] {
        emit("task 2 doing something");
        std::this_thread::sleep_for(1500ms);
        emit("task 2 is going to throw");
        throw std::runtime_error("here is an error");
    }, use_future);

    // an example of returning a future and then immediately using its continuation.
    // note that the continuation happens on the task_manager's thread pool
    mgr.submit([]
               {
                   emit("task 3 doing something");
                   std::this_thread::sleep_for(500ms);
                   emit("task 3 is done");
               },
               use_future)
            .then([](auto f) {
                try {
                    f.get();
                }
                catch(std::exception const& e) {
                    emit("task 3 threw an exception: ", e.what());
                }
            });

    // block on the future of the second example
    try {
        f.get();
    }
    catch (std::exception &e) {
        emit("task 2 threw: ", e.what());
    }
}
```

Which would result in the following output:

```cpp
task 1 is doing something
task 2 doing something
task 3 doing something
task 3 is done
task 1 done
task 1 completed
task 2 is going to throw
task 2 threw: here is an error
```

And here's the complete code (tested on apple clang which is more promiscuous than gcc, so if i've missed a this-> in a lambda, my apologies):

```cpp
#define BOOST_THREAD_PROVIDES_FUTURE 1
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION 1
#define BOOST_THREAD_PROVIDES_EXECUTORS 1

/* written by Richard Hodges 2017
 * You're free to use the code, but please give credit where it's due :)
 */
#include <boost/thread/future.hpp>
#include <boost/thread/executors/basic_thread_pool.hpp>
#include <thread>
#include <utility>
#include <unordered_map>
#include <stdexcept>
#include <condition_variable>

// I made a task an object because I thought I might want to store state in it.
// it turns out that this is not strictly necessary

struct task {

};

/*
 * This is the implementation data for one task_manager
 */
struct task_manager_impl {

    using mutex_type = std::mutex;
    using lock_type = std::unique_lock<mutex_type>;

    auto get_lock() -> lock_type {
        return lock_type(mutex_);
    }

    auto add_task(lock_type const &lock, std::unique_ptr<task> t) {
        auto id = t.get();
        task_map_.emplace(id, std::move(t));
    }

    auto remove_task(lock_type lock, task *task_id) {
        task_map_.erase(task_id);
        if (task_map_.empty()) {
            lock.unlock();
            no_more_tasks_.notify_all();
        }
    }

    auto wait(lock_type lock) {
        no_more_tasks_.wait(lock, [this]() { return task_map_.empty(); });
    }

    // for this example I have chosen to express errors as exceptions
    using error_type = std::exception_ptr;

    mutex_type mutex_;
    std::condition_variable no_more_tasks_;


    std::unordered_map<task *, std::unique_ptr<task>> task_map_;
};

/*
 * This stuff is the protocol to figure out whether to return a future
 * or just invoke a callback.
 * Total respect to Christopher Kohlhoff of asio fame for figuring this out
 * I merely step in his footsteps here, with some simplifications because of c++11
 */
struct use_future_t {
};
constexpr auto use_future = use_future_t();

template<class Handler>
struct make_async_handler {
    auto wrap(Handler handler) {
        return handler;
    }

    struct result_type {
        auto get() -> void {}
    };

    struct result_type result;
};

template<>
struct make_async_handler<const use_future_t &> {
    struct shared_state_type {
        boost::promise<void> promise;
    };

    make_async_handler() {
    }

    template<class Handler>
    auto wrap(Handler &&) {
        return [shared_state = this->shared_state](auto error) {
            // boost promises deal in terms of boost::exception_ptr so we need to marshal.
            // this is a small price to pay for the extra utility of boost::promise over
            // std::promise
            if (error) {
                try {
                    std::rethrow_exception(error);
                }
                catch (...) {
                    shared_state->promise.set_exception(boost::current_exception());
                }
            } else {
                shared_state->promise.set_value();
            }
        };
    }


    struct result_type {
        auto get() -> boost::future<void> { return shared_state->promise.get_future(); }

        std::shared_ptr<shared_state_type> shared_state;
    };

    std::shared_ptr<shared_state_type> shared_state = std::make_shared<shared_state_type>();
    result_type result{shared_state};

};

/*
 * Provides the logic of a task manager. Also notice that it maintains a boost::basic_thread_pool
 * The destructor of a basic_thread_pool will not complete until all tasks are complete. So our
 * program will not crash horribly at exit time.
 */
struct task_manager_service {

    /*
     * through this function, the service has full control over how it is created and destroyed.
     */

    static auto use() -> task_manager_service&
    {
        static task_manager_service me {};
        return me;
    }

    using impl_class = task_manager_impl;

    struct deleter {
        void operator()(impl_class *p) {
            service_->destroy(p);
        }

        task_manager_service *service_;
    };

    /*
     * defining impl_type in terms of a unique_ptr ensures that the handle will be
     * moveable but not copyable.
     * Had we used a shared_ptr, the handle would be copyable with shared semantics.
     * That can be useful too.
     */
    using impl_type = std::unique_ptr<impl_class, deleter>;

    auto construct() -> impl_type {
        return impl_type(new impl_class(),
                         deleter {this});
    }

    auto destroy(impl_class *impl) -> void {
        wait(*impl);
        delete impl;
    }

    template<class Job, class Handler>
    auto submit(impl_class &impl, Job &&job, Handler &&handler) {

        auto make_handler = make_async_handler<Handler>();


        auto async_handler = make_handler.wrap(std::forward<Handler>(handler));

        auto my_task = std::make_unique<task>();
        auto task_ptr = my_task.get();

        auto task_done = [
                this,
                task_id = task_ptr,
                &impl,
                async_handler
        ](auto error) {
            async_handler(error);
            this->remove_task(impl, task_id);
        };
        auto lock = impl.get_lock();
        impl.add_task(lock, std::move(my_task));
        launch(impl, task_ptr, std::forward<Job>(job), task_done);

        return make_handler.result.get();
    };

    template<class F, class Handler>
    auto launch(impl_class &, task *task_ptr, F &&f, Handler &&handler) -> void {
        this->thread_pool_.submit([f, handler] {
            auto error = std::exception_ptr();
            try {
                f();
            }
            catch (...) {
                error = std::current_exception();
            }
            handler(error);
        });
    }


    auto wait(impl_class &impl) -> void {
        impl.wait(impl.get_lock());
    }

    auto remove_task(impl_class &impl, task *task_id) -> void {
        impl.remove_task(impl.get_lock(), task_id);
    }


    boost::basic_thread_pool thread_pool_{std::thread::hardware_concurrency()};

};

/*
 * The task manage handle. Holds the task_manager implementation plus provides access to the
 * owning task_manager_service. In this case, the service is a global static object. In an io loop environment
 * for example, asio, the service would be owned by the io loop.
 */
struct task_manager {

    using service_type = task_manager_service;
    using impl_type = service_type::impl_type;
    using impl_class = decltype(*std::declval<impl_type>());

    task_manager()
            : service_(std::addressof(service_type::use()))
            , impl_(get_service().construct()) {}

    template<class Job, class Handler>
    auto submit(Job &&job, Handler &&handler) {
        return get_service().submit(get_impl(),
                                    std::forward<Job>(job),
                                    std::forward<Handler>(handler));
    }

    auto get_service() -> service_type & {
        return *service_;
    }

    auto get_impl() -> impl_class & {
        return *impl_;
    }

private:

    service_type* service_;
    impl_type impl_;
};


/*
 * helpful thread-safe emitter
 */
std::mutex thing_mutex;

template<class...Things>
void emit(Things &&...things) {
    auto lock = std::unique_lock<std::mutex>(thing_mutex);
    using expand = int[];
    void(expand{0,
                ((std::cout << things), 0)...
    });
    std::cout << std::endl;
}

using namespace std::literals;

int main() {
    task_manager mgr;

    // an example of using async callbacks to indicate completion and error
    mgr.submit([] {
                   emit("task 1 is doing something");
                   std::this_thread::sleep_for(1s);
                   emit("task 1 done");
               },
               [](auto err) {
                   if (not err) {
                       emit("task 1 completed");
                   } else {
                       emit("task 1 failed");
                   }
               });

    // an example of returning a future (see later)
    auto f = mgr.submit([] {
        emit("task 2 doing something");
        std::this_thread::sleep_for(1500ms);
        emit("task 2 is going to throw");
        throw std::runtime_error("here is an error");
    }, use_future);

    // an example of returning a future and then immediately using its continuation.
    // note that the continuation happens on the task_manager's thread pool
    mgr.submit([] {
                   emit("task 3 doing something");
                   std::this_thread::sleep_for(500ms);
                   emit("task 3 is done");
               },
               use_future)
            .then([](auto f) {
                try {
                    f.get();
                }
                catch (std::exception const &e) {
                    emit("task 3 threw an exception: ", e.what());
                }
            });

    // block on the future of the second example
    try {
        f.get();
    }
    catch (std::exception &e) {
        emit("task 2 threw: ", e.what());
    }
}
```