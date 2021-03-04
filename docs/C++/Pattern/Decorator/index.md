# Decorator pattern

在阅读 [CPP / C++ Notes - Design Patterns](https://caiorss.github.io/C-Cpp-Notes/cpp-design-patterns.html) 时，其中有如下描述:

> [Decorator pattern - Wikipedia](https://en.wikipedia.org/wiki/Decorator_pattern)
>
> - C++11: NO
> - The decorator class uses a raw pointer to the wrapped object. All objects in the main functions are allocated on the stack. In this example, there is any problem over using raw pointers since they don't own memory.
> - Note: Also shows the decorator implemented with **Mixins**.

其中的mixin引起了我的注意。

## wikipedia [Decorator pattern](https://en.wikipedia.org/wiki/Decorator_pattern)

### C++

Two options are presented here: first, a dynamic, runtime-composable decorator (has issues with calling decorated functions unless proxied explicitly) and a decorator that uses mixin inheritance.

#### Dynamic Decorator

```C++
#include <iostream>
#include <string>

struct Shape
{
	virtual ~Shape() = default;

	virtual std::string GetName() const = 0;
};

struct Circle: Shape
{
	void Resize(float factor)
	{
		radius *= factor;
	}

	std::string GetName() const override
	{
		return std::string("A circle of radius ") + std::to_string(radius);
	}

	float radius = 10.0f;
};

struct ColoredShape: Shape
{
	ColoredShape(const std::string &color, Shape *shape) :
					color(color), shape(shape)
	{
	}

	std::string GetName() const override
	{
		return shape->GetName() + " which is colored " + color;
	}

	std::string color;
	Shape *shape;
};

int main()
{
	Circle circle;
	ColoredShape colored_shape("red", &circle);
	std::cout << colored_shape.GetName() << std::endl;

}
// g++ test.cpp

```



```C++
#include <memory>
#include <iostream>
#include <string>

struct WebPage
{
	virtual void display()=0;
	virtual ~WebPage() = default;
};

struct BasicWebPage: WebPage
{
	std::string html;
	void display() override
	{
		std::cout << "Basic WEB page" << std::endl;
	}
	~BasicWebPage() = default;
};

struct WebPageDecorator: WebPage
{
	WebPageDecorator(std::unique_ptr<WebPage> webPage) :
					_webPage(std::move(webPage))
	{
	}
	void display() override
	{
		_webPage->display();
	}
	~WebPageDecorator() = default;
private:
	std::unique_ptr<WebPage> _webPage;
};

struct AuthenticatedWebPage: WebPageDecorator
{
	AuthenticatedWebPage(std::unique_ptr<WebPage> webPage) :
					WebPageDecorator(std::move(webPage))
	{
	}

	void authenticateUser()
	{
		std::cout << "authentification done" << std::endl;
	}
	void display() override
	{
		authenticateUser();
		WebPageDecorator::display();
	}
	~AuthenticatedWebPage() = default;
};

struct AuthorizedWebPage: WebPageDecorator
{
	AuthorizedWebPage(std::unique_ptr<WebPage> webPage) :
					WebPageDecorator(std::move(webPage))
	{
	}

	void authorizedUser()
	{
		std::cout << "authorized done" << std::endl;
	}
	void display() override
	{
		authorizedUser();
		WebPageDecorator::display();
	}
	~AuthorizedWebPage() = default;
};

int main(int argc, char *argv[])
{
	std::unique_ptr<WebPage> myPage = std::make_unique<BasicWebPage>();

	myPage = std::make_unique<AuthorizedWebPage>(std::move(myPage));
	myPage = std::make_unique<AuthenticatedWebPage>(std::move(myPage));
	myPage->display();
	std::cout << std::endl;
	return 0;
}
// g++ --std=c++14 test.cpp

```



#### Static Decorator (Mixin Inheritance)

This example demonstrates a static Decorator implementation, which is possible due to C++ ability to inherit from the template argument.

```C++
#include <iostream>
#include <string>

struct Circle
{
	void Resize(float factor)
	{
		radius *= factor;
	}

	std::string GetName() const
	{
		return std::string("A circle of radius ") + std::to_string(radius);
	}

	float radius = 10.0f;
};

template<typename T>
struct ColoredShape: public T
{
	ColoredShape(const std::string &color) :
					color(color)
	{
	}

	std::string GetName() const
	{
		return T::GetName() + " which is colored " + color;
	}

	std::string color;
};

int main()
{
	ColoredShape<Circle> red_circle("red");
	std::cout << red_circle.GetName() << std::endl;
	red_circle.Resize(1.5f);
	std::cout << red_circle.GetName() << std::endl;
}
// g++ --std=c++11 test.cpp

```

