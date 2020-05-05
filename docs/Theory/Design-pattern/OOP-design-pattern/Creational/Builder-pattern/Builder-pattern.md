# Builder pattern



## [When would you use the Builder Pattern?](https://stackoverflow.com/questions/328496/when-would-you-use-the-builder-pattern)

原文的[这个回答](https://stackoverflow.com/a/1953567)非常好，下面是java版的代码

```c++
public class Pizza {
  private int size;
  private boolean cheese;
  private boolean pepperoni;
  private boolean bacon;

  public static class Builder {
    //required
    private final int size;

    //optional
    private boolean cheese = false;
    private boolean pepperoni = false;
    private boolean bacon = false;

    public Builder(int size) {
      this.size = size;
    }

    public Builder cheese(boolean value) {
      cheese = value;
      return this;
    }

    public Builder pepperoni(boolean value) {
      pepperoni = value;
      return this;
    }

    public Builder bacon(boolean value) {
      bacon = value;
      return this;
    }

    public Pizza build() {
      return new Pizza(this);
    }
  }

  private Pizza(Builder builder) {
    size = builder.size;
    cheese = builder.cheese;
    pepperoni = builder.pepperoni;
    bacon = builder.bacon;
  }
}

Pizza pizza = new Pizza.Builder(12)
                       .cheese(true)
                       .pepperoni(true)
                       .bacon(true)
                       .build();
```

c++版

```c++
class Pizza;

class Pizza
{
	int size;
	bool cheese;
	bool pepperoni;
	bool bacon;
public:
	class Builder
	{
		//required
		int size_;

		//optional
		bool cheese_ { false };
		bool pepperoni_ { false };
		bool bacon_ { false };
		public:
		friend class Pizza;
		Builder(int value)
		{
			size_ = value;
		}

		Builder& cheese(bool value)
		{
			cheese_ = value;
			return *this;
		}

		Builder& pepperoni(bool value)
		{
			pepperoni_ = value;
			return *this;
		}

		Builder& bacon(bool value)
		{
			bacon_ = value;
			return *this;
		}

		Pizza* build()
		{
			return new Pizza(*this);
		}
	};
private:
	Pizza(Builder& builder)
	{
		size = builder.size_;
		cheese = builder.cheese_;
		pepperoni = builder.pepperoni_;
		bacon = builder.bacon_;
	}
};

int main()
{
	Pizza* pizza = Pizza::Builder(12).cheese(true).pepperoni(true).bacon(true).build();
}

```





## oodesign [Builder Pattern](https://www.oodesign.com/builder-pattern.html)



## 维基百科[Builder pattern](https://en.wikipedia.org/wiki/Builder_pattern)

The Builder is a [design pattern](https://en.wikipedia.org/wiki/Software_design_pattern) designed to provide a flexible solution to various **object creation problems** in [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming). The intent of the Builder design pattern is to [separate](https://en.wikipedia.org/wiki/Separation_of_concerns) the construction of a complex object from its representation. It is one of the Gang of Four [design patterns](https://en.wikipedia.org/wiki/Design_Patterns).



