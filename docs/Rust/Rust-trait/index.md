# Rust trait

## cnblogs [Rust：一个不再有 C/C++ 的，实现安全实时软件的未来](https://www.cnblogs.com/dhcn/p/12344538.html)

### 多态性

C++ 支持多种形式的多态，作者认为这有助于语言的丰富性。静态多态中有模板、函数和以及操作符重载；动态多态有子类。但这些表达形式也有非常明显的缺点：子类与父类之间的紧密耦合，导致子类过于依赖父类，缺乏独立性；模板则因为其缺乏参数化的特性而导致调试困难。

> NOTE: 
>
> "子类与父类之间的紧密耦合，导致子类过于依赖父类，缺乏独立性"
>
> 让我想到了 Sean Parent 的 "Inheritance-is-the-base-class-of-evil"
>
> 

Rust 中的 trait 则定义了一种指定静态动态接口共享的行为。Trait 类似于其他语言中接口（interface）的功能，但 Rust 中只支持实现（implements）而没有继承（extends）关系，鼓励基于组合的设计而不是实现继承，降低耦合度。

```rust
trait Rateable
{

	* /// Rate fluff out of 10*
	*/// Ratings above 10 for exceptionally soft bois*
	fn fluff_rating(&self) -> f32;
}

struct Alpaca
{
	days_since_shearing :f32, age :f32
}

impl Rateable for Alpaca
{
	fn fluff_rating(&self) -> f32
	{
		10.0 * 365.0 / self.days_since_shearing
	}
}

```



首先定义一个名为 Rateable 的 trait，然后需要调用函数 fluff_rating 并返回一个浮点数来实现 Rateable。接着就是在 Alpaca 结构体上对 Rateable trait 的实现。下面是使用同样的方法定义 Cat 类型。