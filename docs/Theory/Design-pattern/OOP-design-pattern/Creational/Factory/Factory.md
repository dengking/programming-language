# Factory (object-oriented programming)

[factory method pattern](https://en.wikipedia.org/wiki/Factory_method_pattern) *and* [abstract factory pattern](https://en.wikipedia.org/wiki/Abstract_factory_pattern)中，都涉及factory。

## 维基百科[Factory (object-oriented programming)](https://en.wikipedia.org/wiki/Factory_(object-oriented_programming))



> NOTE: 显然在OOP中的factory的概念是借用的我们日常时候中，制造product的factory的概念。原文的Factory Method in [LePUS3](https://en.wikipedia.org/wiki/LePUS3)形象化地展示了它。
>
> 如果从factory的广义概念来说的话， [singleton pattern](https://en.wikipedia.org/wiki/Singleton_pattern)也是一种factory，在原文的[Object creation](https://en.wikipedia.org/wiki/Factory_(object-oriented_programming)#Object_creation)段，对此进行了说明。constructor也是一种factory，原文对此有强调。

### Motivation



> NOTE: 原文这一节的第一段中，讨论了 [abstraction](https://en.wikipedia.org/wiki/Abstraction_(computer_science)) （factory）和 concrete （constructor），显然，使用factory相当于添加了一层抽象。在原文的application章节也进行了关于abstraction和concrete的讨论。

Factories may be invoked in various ways, most often a method call (a *[factory method](https://en.wikipedia.org/wiki/Factory_method)*), sometimes by being called as a function if the factory is a [function object](https://en.wikipedia.org/wiki/Function_object) (a *factory function*). In some languages factories are generalizations of constructors, meaning constructors are themselves factories and these are invoked in the same way. In other languages factories and constructors are invoked differently, for example using the keyword `new` to invoke constructors but an ordinary method call to invoke factories; in these languages factories are an abstraction of constructors but not strictly a generalization, as constructors are not themselves factories.

> NOTE: python就是典型的“factories are generalizations of constructors”，关于此的一个例子就是维基百科[Abstract factory pattern#Python example](https://en.wikipedia.org/wiki/Abstract_factory_pattern#Python_example)中给出的例子。
>
> c++就是典型的“factories and constructors are invoked differently”

### Use



Using factories instead of constructors or prototypes allows one to use polymorphism for object creation, not only object use.

> NOTE: 上一句对原文这一节的前两段进行了总结。“object use”即“使用对象”，在OOP中，object use是具备polymorphism特性的，但是object creation则不然，因为此时压根就没有object，对于object creation，我们需要借助factory来获得polymorphism。需要注意id是，在使用factory来实现“polymorphism for object creation”，需要由programmer来实现dispatch，在原文的[Applications](https://en.wikipedia.org/wiki/Factory_(object-oriented_programming)#Applications)章节对此进行了详细描述，关于此的一个例子是：维基百科[Abstract factory pattern#Python example](https://en.wikipedia.org/wiki/Abstract_factory_pattern#Python_example)



#### Object creation

Similarly, using this definition, a [singleton](https://en.wikipedia.org/wiki/Singleton_(mathematics)) implemented by the [singleton pattern](https://en.wikipedia.org/wiki/Singleton_pattern) is a formal factory – it returns an object, but does not create new objects beyond the single instance.

### Examples

```python
def f():
    return A()
```

A simple **factory function** implementing the **singleton pattern** is:

```python
def f():
    if f.obj is None:
        f.obj = A()
    return f.obj

f.obj = None
```



### [Applications](https://en.wikipedia.org/wiki/Factory_(object-oriented_programming)#Applications)

Factories determine the actual *concrete* type of [object](https://en.wikipedia.org/wiki/Object_(computer_science)) to be created, and it is here that the object is actually created. As the factory only returns an abstract interface to the object, the client code does not know – and is not burdened by – the actual concrete type of the object which was just created. However, the type of a concrete object is known by the abstract factory. 

### [Applicability](https://en.wikipedia.org/wiki/Factory_(object-oriented_programming)#Applicability)

### Benefits and variants

#### Descriptive names

> NOTE: 是代码的可读性更高

```c#
    public class Complex
    {
        public double real;
        public double imaginary;

        public static Complex FromCartesian(double real, double imaginary)
        {
            return new Complex(real, imaginary);
        }

        public static Complex FromPolar(double modulus, double angle)
        {
            return new Complex(modulus * Math.Cos(angle), modulus * Math.Sin(angle));
        }

        private Complex(double real, double imaginary)
        {
            this.real = real;
            this.imaginary = imaginary;
        }
    }

Complex product = Complex.FromPolar(1, Math.PI);
```

#### Encapsulation

```java
public class ImageReaderFactory {
    public static ImageReader createImageReader(ImageInputStreamProcessor iisp) {
        if (iisp.isGIF()) {
            return new GifReader(iisp.getInputStream());
        }
        else if (iisp.isJPEG()) {
            return new JpegReader(iisp.getInputStream());
        }
        else {
            throw new IllegalArgumentException("Unknown image type.");
        }
    }
}
```

### [Limitations](https://en.wikipedia.org/wiki/Factory_(object-oriented_programming)#Limitations)

> NOTE: 原文这一节所描述的是OOP设计相关的问题



## Factory and constructor

在原文中，对两者进行了较多的讨论。