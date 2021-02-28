# Strategy pattern

简记: 以 《Head First设计模式》 中给出的duck class为例来记忆: 对于how to quack、how to swim、how to fly，都有对应的behavior/strategy class/interface来进行描述；

Strategy pattern能够非常好的说明: composition over inheritance: 使用composition and interface来实现:

1、code reuse 

2、polymorphism 

3、extendibility/flexibility

4、information hiding/encapsulation

## jianshu [策略模式(Strategy Pattern)](https://www.jianshu.com/p/422acad380dd)

> NOTE: 这篇文章是摘抄自 《Head First设计模式》，原文中的例子是非常好的。
>

### 一、先从简单的模拟鸭子应用做起

![img](https://upload-images.jianshu.io/upload_images/9571610-f68ce7a411954f0c.png?imageMogr2/auto-orient/strip|imageView2/2/w/694/format/webp)

> Duck继承类图

### 二、现在我们得让鸭子能飞

主管们决定，此模拟程序需要会飞的鸭子来将竞争者抛在后头。

Joe想：我只需要在Duck类中加上`fly()`方法，然后所有鸭子都会继承`fly()`。这是我大显身手展示OO才华的时候了。

### 三、但是，可怕的问题发生了

Joe突然接到经理的电话：Joe,我正在股东会议上，刚刚看了一下展示，有很多“橡皮鸭子”在屏幕上飞来飞去，这是你在开玩笑吗？

Joe忽略了一件事：并非Duck所有的子类都会飞。Joe在Duck超类中加入新的行为，会使得某些并不适合该行为的子类也具有该行为。现在可好了！程序中有了一个无生命的会飞的东西。Joe体会到：

- 对代码所做的局部修改，影响层面可不只是局部。

- 当涉及维护时，为了复用目的而使用继承，结局并不完美。

    

> NOTE: 
>
> 上述"复用"指的是code reuse;
>
> 上述"继承"，主要指implementation inheritance，参见"subtyping-VS-inheritance"章节
>
> defect: **extendibility**差: "Any change in the parent's implementation will force the subclass to change"，关于inheritance的**extendibility**差的，参见`Theory\Programming-paradigm\Object-oriented-programming\Design-pattern\Principle\Composition-over-inheritance`章节



### 四、利用接口如何？

> NOTE: 此处的"接口"指的是Java interface。

Joe认识到继承可能不是答案，我可以把`fly()`从超类中取出来，放进一个`Flyable`接口中。这么一来，只有会飞的鸭子才实现此接口。同样的方式，也可以用来设计一个`Quackable`接口，因为不是所有的鸭子都会叫。

![img](https://upload-images.jianshu.io/upload_images/9571610-829c8bd93072c568.png?imageMogr2/auto-orient/strip|imageView2/2/w/857/format/webp)

> Duck接口类图

你觉得这个设计如何？
虽然`Flyable`与`Quackable`可以解决“一部分”问题（不会再有会飞的橡皮鸭），但是却造成代码无法复用，这只能算是从一个噩梦跳进另一个噩梦。（继承、组合都能达到代码复用的效果，但用组合能使代码耦合度更低）具体来说，如果MallardDuck和RubberDuck的quack()实现是一样呢？目前这种接口的设计就需要在MallardDuck和RubberDuck中重复实现一遍quack()。同理，如果MallardDuck和RedheadDuck的fly()实现是一样的呢？同样需要重复实现一遍fly()。即：用接口的方式无法实现代码复用。

> NOTE: defect: 无法实现**code reuse**

### 五、按照我以前的思维，我可能会想到这样设计

![img](https://upload-images.jianshu.io/upload_images/9571610-e017dda8f5857b59.png?imageMogr2/auto-orient/strip|imageView2/2/w/702/format/webp)

> 我的设计图

从现在看是糟糕的设计，问题如下：

- 继承层级太多，不可控。如有需求变动，可能需要调整**继承关系**才能适应新的需求

- `QuackableDuck`和`FlyableDuck`是“虚拟”的Duck，严格来讲不是IS－A Duck

    > NOTE: 违反semantic

- 如果`MallardDuck`和`RubberDuck`的`quack()`实现是一样呢？MallardDuck和RubberDuck是兄弟关系，不是继承关系，所以无法复用，需要在`MallardDuck`和`RubberDuck`中重复实现一遍`quack()`。即：虽然用了一堆继承，但这种方式不能完全实现代码复用。

> NOTE: defect: 无法实现code reuse

### 六、把问题归零

现在我们知道使用继承并不能很好地解决问题，因为鸭子的行为在子类里不断地改变，并且让所有的子类都有这些行为是不恰当的。Flyable与Quackable接口一开始似乎还挺不错，解决了问题（只有会飞的鸭子才继承Flyable），但是Java接口不具有实现代码（Java 8开始接口有默认实现），所以继承（实现）接口无法达到代码的复用。有一个设计原则，恰好用于此状况：**找出应用中可能需要变化之处，把它们独立出来，不要和那些不需要变化的代码混在一起**

> NOTE: "commonality and variability analysis"，上述设计原则和policy-based design中提出的思路类似。

好，该是把鸭子的行为从Duck类中取出来的时候了！

### 七、分开变化和不会变化的部分

我们知道`Duck`类内的`fly()`和`quack()` 会随着鸭子的不同而改变。为了要把这两个行为从Duck类中分开，我们将把它们从Duck类中取出来，建立一组新类来代表每个行为。

### 八、设计鸭子的行为

新的设计我们将遵循一个设计原则：**针对接口编程，而不是针对实现编程**

我们利用接口代表每个行为，比方说，`FlyBehavior`与`QuackBehavior`，而行为的每个实现都将实现其中的一个接口。所以这次鸭子类不会负责实现Flyable与Quackable接口，反而是由我们制造一组其他类专门实现`FlyBehavior`与`QuackBehavior`，这就称为“行为”类。由行为类而不是Duck类来实现行为接口。这样的做法迥异于以往，以前的做法是：行为来自Duck超类的具体实现，或是实现某个接口并由子类自行实现而来。这两种做法都是依赖于“实现”，我们被实现绑得死死的，没办法更改行为（除非写更多代码）在我们的新设计中，鸭子的子类将使用接口（`FlyBehavior`与`QuackBehavior`）所表示的行为，所以实际的“实现”不会被绑死在鸭子的子类中。

### 九、实现鸭子的行为

在此，我们有两个接口，FlyBehavior和QuackBehavior，还有它们对应的类，负责实现具体的行为。

![img](https:////upload-images.jianshu.io/upload_images/9571610-c2a06379d2fec0c3.png?imageMogr2/auto-orient/strip|imageView2/2/w/909/format/webp)

> 行为接口

这样的设计，可以让飞行和呱呱叫的动作被其他的对象复用，因为这些行为已经与鸭子类无关了。而我们可以新增一些行为，不会影响到既有的行为类。

### 十、整合鸭子的行为

关键在于，鸭子现在会将飞行和呱呱叫的动作“委托”别人处理，而不是使用定义在Duck类（或子类）内的呱呱叫和飞行方法。

> NOTE: delegation

![img](https:////upload-images.jianshu.io/upload_images/9571610-bfc0c2dabc9bbada.png?imageMogr2/auto-orient/strip|imageView2/2/w/1025/format/webp)

最终设计图

### 十一、编码

1、FlyBehavior与行为实现类

```java
public interface FlyBehavior {
    public void fly();
}

public class FlyWithWings implements FlyBehavior{
    @Override
    public void fly() {
        System.out.println("用翅膀飞");
    }
}

public class FlyNoWay implements FlyBehavior {
    @Override
    public void fly() {
        System.out.println("不会飞");
    }
}
```

2、QuackBehavior与行为实现类



```java
public interface QuackBehaivor {
    public void quack();
}

public class Quack implements QuackBehaivor {
    @Override
    public void quack() {
        System.out.println("呱呱叫");
    }
}

public class MuteQuack implements QuackBehaivor {
    @Override
    public void quack() {
        System.out.println("不会叫");
    }
}
```

3、Duck与子类



```csharp
public abstract class Duck {

   FlyBehavior flyBehavior;
   QuackBehaivor quackBehavior;
   
   public abstract void display();
   
   public void performFly() {
       flyBehavior.fly();
   }
   
   public void performQuack() {
       quackBehavior.quack();
   }
   
   public void swim() {
       System.out.println("所有的鸭子都会漂浮");
   }

   // 动态设置飞行行为
   public void setFlyBehavior(FlyBehavior flyBehavior) {
       this.flyBehavior = flyBehavior;
   }

   //动态设置叫的行为
   public void setQuackBehavior(QuackBehaivor quackBehavior) {
       this.quackBehavior = quackBehavior;
   }
}
```



```java
public class MallardDuck extends Duck {
    @Override
    public void display() {
        System.out.println("我是绿头鸭");
    }
}
```

4、测试类



```cpp
public class MiniDuckSimulator {
    
    public static void main(String []args) {
        Duck mallard = new MallardDuck();
        mallard.setFlyBehavior(new FlyWithWings());//用翅膀飞
        mallard.setQuackBehavior(new Quack());//呱呱叫
        
        mallard.performFly();
        mallard.performQuack();
    }
}
```

### 十二、个人见解

以上为《Head First设计模式》中策略模式章节提供的方案，基本上没有问题，但有一点我不太认同，那就是将`FlyBehavior flyBehavior`、`QuackBehaivor quackBehavior` 定义在基类Duck中，并提供`performFly`、`performQuack`方法，这样不还是所有鸭子都有“叫”和“飞”的能力吗？我认为应该将`FlyBehavior`、`QuackBehaivor`分别声明在具有这些行为的鸭子子类里，而不是基类`Duck`中。

### 十三、“有一个”可能比“是一个”更好

“有一个”关系相当有趣：每一鸭子都有一个FlyBehavior和一个QuackBehavior，好将飞行和呱呱叫委托给它们代为处理。
 当你将两个类结合起来用，如同本例一般，这就是组合。这种做法和“继承”不同的地方在于，鸭子的行为不是继承来的，而是和适当的行为对象“组装”来的。这个技巧是一个很重要的设计原则：**多用组合，少用继承**

如你所见，使用组合建立系统具有很大的弹性，不仅可将算法族封装成类，更可以“在运行时动态地改变行为”，只要组合的行为对象符合正确的接口标准即可。

> NOTE: 这一段所描述的其实是: composition  over inheritance；

### 十四、这就是策略模式

**策略模式**：定义了算法族，分别封装起来，让它们之间可以互相替换，此模式让算法的变化独立于使用算法的客户。

### 十五、策略模式结构

策略模式的结构包括三种角色：

- 策略（Strategy）：策略是一个接口，该接口定义算法标识。
- 具体策略（ConcreteStrategy）：具体策略是实现策略接口的类。具体策略实现策略接口所定义的抽象方法，即给出算法标识的具体算法。
- 上下文（Context）：上下文是依赖于策略接口的类，即上下文包含有策略声明的变量。上下文中提供了一个方法，该方法委托策略变量调用具体策略所实现的策略接口中的方法。

策略模式接口的类图如下所示：



![img](https:////upload-images.jianshu.io/upload_images/9571610-9f328bbbcb5326cc.png?imageMogr2/auto-orient/strip|imageView2/2/w/676/format/webp)

策略模式UML

### 十六、策略模式的优点

- 上下文和具体策略是松耦合关系。因此上下文只知道它要使用某一个实现Strategy接口类的实例，但不需要知道具体是哪一个类。
- 策略模式满足“开-闭原则”。当增加新的具体策略时，不需要修改上下文类的代码，上下文就可以引用新的具体策略的实例。

### 十七、适合使用策略模式的情景

- 一个类定义了多种行为，并且这些行为在这个类的方法中以多个条件语句的形式出现，那么可以使用策略模式在类中使用大量的条件语句。
- 程序不希望暴露复杂的、与算法有关的数据结构，那么可以使用策略模式来封装算法。
- 需要使用一个算法的不同变体。

### 十八、用策略模式实现一款冒险游戏

练习：现在需要设计一款冒险游戏，该游戏有各种角色，和各种武器，每个角色可以使用一种武器，但是可以在游戏的过程中换武器。

分析：Character(角色)是抽象类，由具体的角色来继承。具体的角色包括：国王(King)、皇后(Queen)、骑士(Knight)、妖怪(Troll) 。而Weapon(武器)是接口，由具体的武器来实现。所有实际的角色和武器都是具体类。
 任何角色如果想换武器，可以调用setWeapon()方法，此方法定义在Character超类中。在打斗(flight)过程中，会调用到目前武器的useWeapon()方法，攻击其他角色。

设计图如下：

![img](https:////upload-images.jianshu.io/upload_images/9571610-2df009df74778c6a.png?imageMogr2/auto-orient/strip|imageView2/2/w/797/format/webp)

> 冒险游戏设计图



## wikipedia [Strategy pattern](https://en.wikipedia.org/wiki/Strategy_pattern)



## Strategy pattern and policy-based design

在维基百科[*Modern C++ Design*#Policy-based design](https://en.wikipedia.org/wiki/Modern_C%2B%2B_Design#Policy-based_design)中对两者进行了说明：

> It has been described as a [compile-time](https://en.wikipedia.org/wiki/Compile-time) variant of the [strategy pattern](https://en.wikipedia.org/wiki/Strategy_pattern), and has connections with C++ [template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming). 