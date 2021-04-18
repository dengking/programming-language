# sean-parent [Polymorphic Task Template in Ten](https://sean-parent.stlab.cc/presentations/2017-11-11-lightning-talk/lightning-talk.pdf)



```C++
#include<memory>
#include<iostream>
#include<utility>

using namespace std;

template<class >
class task;
//...
int main()
{
	task<unique_ptr<int>()> f = [_p = make_unique<int>(42)]() mutable
	{
		return move(_p);
	};
	cout << *f() << endl;
}

```



## 不带small object optimization



```C++
#include<memory>
#include<iostream>
#include<utility>
#include<type_traits>

using namespace std;

template<class >
class task;

//...

template<class R, class ... Args>
class task<R(Args...)>
{
	struct concept;
	template<class F>
	struct model;
	unique_ptr<concept> _p;
public:
	template<class F>
	task(F &&f) :
					_p(make_unique < model<decay_t<F>> > (forward<F>(f)))
	{
	}
	R operator()(Args ... args)
	{
		return _p->_invoke(forward<Args>(args)...);
	}
};

template<class R, class ... Args>
struct task<R(Args...)>::concept
{
	virtual ~concept() = default;
	virtual R _invoke(Args&&...) = 0;
};

template<class R, class ... Args>
template<class F>
struct task<R(Args...)>::model final : concept
{
	template<class G>
	model(G &&f) :
					_f(forward<G>(f))
	{
	}
	R _invoke(Args &&... args) override
	{
		return invoke(_f, forward<Args>(args)…);
	}
	F _f;
};

```



## 带small object optimization

```C++
#include<memory>
#include<iostream>
#include<utility>
#include<type_traits>

using namespace std;

template<class >
class task;

/**
 * @brief
 *
 * @tparam R 返回值类型
 * @tparam Args 参数类型
 */
template<class R, class ... Args>
class task<R(Args...)>
{
	struct concept;
	/**
	 * @tparam F 函数类型
	 * @tparam Small 是否比buffer小
	 */
	template<class F, bool Small>
	struct model;
	/**
	 * buffer的长度
	 */
	static constexpr size_t small_size = sizeof(void*) * 4;
	/**
	 * buffer
	 */
	aligned_storage_t<small_size> _data;
	/**
	 * @brief aliasing，使用void*来作为conversion的bridge
	 *
	 * @return
	 */
	concept& self()
	{
		return *static_cast<concept*>(static_cast<void*>(&_data));
	}
public:
	/**
	 * @brief
	 *
	 * @tparam F callable，它可以是function pointer、functor
	 * @param f
	 */
	template<class F>
	task(F &&f)
	{
		constexpr bool is_small = sizeof(model<decay_t<F>, true> ) <= small_size;
		/**
		 * placement new
		 */
		new (&_data) model<decay_t<F>, is_small>(forward<F>(f));
	}
	~task()
	{
		/**
		 * 使用destructor name来调用destructor
		 */
		self().~concept();
	}
	task(task &&x) noexcept
	{
		x.self()._move(&_data);
	}
	task& operator=(task &&x) noexcept
	{
		self().~concept();
		x.self()._move(&_data);
		return *this;
	}
	R operator()(Args ... args)
	{
		return self()._invoke(forward<Args>(args)...);
	}
};

template<class R, class ... Args>
struct task<R(Args...)>::concept
{
	virtual ~concept() = default;
	virtual R _invoke(Args&&...) = 0;
	virtual void _move(void*) = 0;
};

template<class R, class ... Args>
template<class F>
struct task<R(Args...)>::model<F, true> final : concept
{
	template<class G>
	model(G &&f) :
					_f(forward<G>(f))
	{
	}
	R _invoke(Args &&... args) override
	{
		return invoke(_f, forward<Args>(args)…);
	}
	void _move(void *p) override
	{
		new (p) model(move(*this));
	}
	F _f;
};

template<class R, class ... Args>
template<class F>
struct task<R(Args...)>::model<F, false> final : concept
{
	template<class G>
	model(G &&f) :
					_p(make_unique<F>(forward<F>(f)))
	{
	}
	R _invoke(Args &&... args) override
	{
		return invoke(*_p, forward<Args>(args)…);
	}
	void _move(void *p) override
	{
		new (p) model(move(*this));
	}
	unique_ptr<F> _p;
};

```

1、placement new

```C++
new (&_data) model<decay_t<F>, is_small>(forward<F>(f));
```



## 完整程序

https://github.com/stlab/libraries/blob/main/stlab/concurrency/task.hpp