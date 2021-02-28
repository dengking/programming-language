# Mixin

[示例代码](https://github.com/scikit-learn/scikit-learn/blob/f0ab589f/sklearn/preprocessing/data.py#L461)如下：
```
class StandardScaler(BaseEstimator, TransformerMixin):
```
[示例代码](https://github.com/scikit-learn/scikit-learn/blob/master/sklearn/base.py)
```
class ClassifierMixin(object):
    """Mixin class for all classifiers in scikit-learn."""
    _estimator_type = "classifier"

    def score(self, X, y, sample_weight=None):
        """Returns the mean accuracy on the given test data and labels.

        In multi-label classification, this is the subset accuracy
        which is a harsh metric since you require for each sample that
        each label set be correctly predicted.

        Parameters
        ----------
        X : array-like, shape = (n_samples, n_features)
            Test samples.

        y : array-like, shape = (n_samples) or (n_samples, n_outputs)
            True labels for X.

        sample_weight : array-like, shape = [n_samples], optional
            Sample weights.

        Returns
        -------
        score : float
            Mean accuracy of self.predict(X) wrt. y.

        """
        from .metrics import accuracy_score
        return accuracy_score(y, self.predict(X), sample_weight=sample_weight)
```
显然 Mixin并非是python独有的，而是在面向对象编程中通用的一种模型，它应该属于programming language theory领域中。

## wikipedia [Mixin](https://en.wikipedia.org/wiki/Mixin)

In [object-oriented programming languages](https://en.wikipedia.org/wiki/Object-oriented_programming_language), a **Mixin** is a [class](https://en.wikipedia.org/wiki/Class_(computer_science)) that contains methods for use by other classes without having to be the parent class of those other classes(Mixin是一个包含其他类使用的方法的类，而不必是其他类的父类). How those other classes gain access to the mixin's methods depends on the language. Mixins are sometimes described as being "**included**" rather than "**inherited**".

总结： a **Mixin** is a [class](https://en.wikipedia.org/wiki/Class_(computer_science)) that contains methods for use by other classes 的实现方式是非常之多的，比如：**inherited**，**composition**，decorator，那它们在使用上有哪些差异之处呢？

Mixins encourage [code reuse](https://en.wikipedia.org/wiki/Code_reuse) and can be used to avoid the inheritance ambiguity that multiple inheritance can cause[[1\]](https://en.wikipedia.org/wiki/Mixin#cite_note-1) (the "[diamond problem](https://en.wikipedia.org/wiki/Multiple_inheritance#The_diamond_problem)"), or to work around lack of support for multiple inheritance in a language. A mixin can also be viewed as an [interface](https://en.wikipedia.org/wiki/Interface_(object-oriented_programming)) with implemented [methods](https://en.wikipedia.org/wiki/Method_(computer_science)). This pattern is an example of enforcing the [dependency inversion principle](https://en.wikipedia.org/wiki/Dependency_inversion_principle).

思考：I did not understand the meaning of the last sentence.

### Definition

Mixins are a language concept that allows a programmer to inject some code into a [class](https://en.wikipedia.org/wiki/Class_(computer_programming)). Mixin programming is a style of [software development](https://en.wikipedia.org/wiki/Software_development), in which units of functionality are created in a class and then mixed in with other classes.[[4\]](https://en.wikipedia.org/wiki/Mixin#cite_note-4)

A mixin class acts as the parent class, containing the desired functionality. A [subclass](https://en.wikipedia.org/wiki/Subclass_(computer_science)) can then inherit or simply reuse this functionality, but not as a means of **specialization**（按照传统的继承的含义，子类就是父类的**specialization**，但是显然，mixin则打破了这种限制). Typically, the mixin will export the desired functionality to a [child class](https://en.wikipedia.org/wiki/Subclass_(computer_science)), without creating a rigid, single "is a" relationship. Here lies the important difference between the concepts of **mixins** and [inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)), in that the child class can still inherit all the features of the parent class, but, the semantics about the child "being a kind of" the parent need not be necessarily applied.

总结：这段描述了mixin和传统的inheritance之间的异同点。显然这是可以回答上面的那个问题中的一部分的：即mixin和传统的inheritance之间的差异，显然mixin可以看做是放松了传统的inheritance的is-a的限制。

## Examples

### In Python

In [Python](https://en.wikipedia.org/wiki/Python_(programming_language)), the `SocketServer` module[[12\]](https://en.wikipedia.org/wiki/Mixin#cite_note-12) has both a `UDPServer` class and a `TCPServer` class. They act as servers for [UDP](https://en.wikipedia.org/wiki/User_Datagram_Protocol) and [TCP](https://en.wikipedia.org/wiki/Transmission_Control_Protocol) socket servers, respectively. Additionally, there are two mixin classes: `ForkingMixIn` and `ThreadingMixIn`. Normally, all new connections are handled within the same process. By extending `TCPServer` with the `ThreadingMixIn` as follows:

```
class ThreadingTCPServer(ThreadingMixIn, TCPServer):
  pass
```

the `ThreadingMixIn` class adds functionality to the TCP server such that each new connection creates a new [thread](https://en.wikipedia.org/wiki/Thread_(computer_science)). Alternatively, using the `ForkingMixIn` would cause the process to be [forked](https://en.wikipedia.org/wiki/Fork_(operating_system)) for each new connection. Clearly, the functionality to create a new thread or fork a process is not terribly useful as a stand-alone class.

In this usage example, the mixins provide alternative underlying functionality without affecting the functionality as a socket server.





## Interfaces and traits

Java 8 introduces a new feature in the form of default methods for interfaces.[[15\]](https://en.wikipedia.org/wiki/Mixin#cite_note-15) Basically it allows a method to be defined in an interface with application in the scenario when a new method is to be added to an interface after the interface class programming setup is done. To add a new function to the interface means to implement the method at every class which uses the interface. Default methods help in this case where they can be introduced to an interface any time and have an implemented structure which is then used by the associated classes. Hence default methods adds a possibility of applying the concept in a mixin sort of a way.

Interfaces combined with [aspect-oriented programming](https://en.wikipedia.org/wiki/Aspect-oriented_programming) can also produce full-fledged mixins in languages that support such features, such as C# or Java. Additionally, through the use of the [marker interface pattern](https://en.wikipedia.org/wiki/Marker_interface_pattern), [generic programming](https://en.wikipedia.org/wiki/Generic_programming), and extension methods, C# 3.0 has the ability to mimic mixins. With C# 3.0 came the introduction of Extension Methods[2] and they can be applied, not only to classes but, also, to interfaces. Extension Methods provide additional functionality on an existing class without modifying the class. It then becomes possible to create a static helper class for specific functionality that defines the extension methods. Because the classes implement the interface (even if the actual interface doesn’t contain any methods or properties to implement) it will pick up all the extension methods also.[[16\]](https://en.wikipedia.org/wiki/Mixin#cite_note-16)[[17\]](https://en.wikipedia.org/wiki/Mixin#cite_note-17)[[18\]](https://en.wikipedia.org/wiki/Mixin#cite_note-18)

[ECMAScript](https://en.wikipedia.org/wiki/ECMAScript) (in most cases implemented as JavaScript) does not need to mimic object composition by stepwise copying fields from one object to another. It natively[[19\]](https://en.wikipedia.org/wiki/Mixin#cite_note-19) supports [Trait](https://en.wikipedia.org/wiki/Trait_(computer_programming)) and Mixin[[20\]](https://en.wikipedia.org/wiki/Mixin#cite_note-20)[[21\]](https://en.wikipedia.org/wiki/Mixin#cite_note-21) based object composition via function objects that implement additional behavior and then are delegated via `call` or `apply` to objects that are in need of such new functionality.







## [对比mixin，composition，decorator](https://stackoverflow.com/questions/533631/what-is-a-mixin-and-why-are-they-useful)
在这个文章中，提问者提出了如下问题：
1. Why would I want to do that rather than put the new functionality into a subclass? 
2. why would a mixin/multiple inheritance approach be better than using composition?
3. What separates a mixin from multiple inheritance? Is it just a matter of semantics?

问题1的答案其实在上一篇文章中已经提出来了，即为了实现[code reuse ](https://en.wikipedia.org/wiki/Code_reuse)

问题3的答案在上一篇文章中已经提出来了，也是上面那篇文章的核心内容，在此不再重复。

现在就说下来第二个问题了。

### mixin和composition
mixin涉及了继承，而composition则不涉及继承
### mixin和decorator
这两种之间的差异是比较明显的，它们目标解决的问题是不同的，结合下面decorator的描述来看，差异就非常明显的：
>The Decorator pattern is used to dynamically add a new feature to an object without changing its implementation. It differs from inheritance because the new feature is added only to that particular object, not to the entire subclass.


### situations where mixins are used:
- You want to provide a lot of optional features for a class.
- You want to use one particular feature in a lot of different classes.
- you want to provide a lot of (not-optional) features for a class, but you want the features in separate classes (and in separate modules) so each module is about one feature (behaviour.) IOW, not for re-use, but for **compartmentalization**
- there is already an existing family of `Parent` class and `Child1`, `Child2`, `ChildN` subclasses inside a 3rd party library, and you want a customized behavior for the entire family. Ideally you would like to add such behavior into `Parent`, and hope the 3rd party library developer will take your Pull Request. Otherwise you will have to implement your own `class NewBehaviorMixin`, and then define a full set of wrapper classes such as `class NewParent(NewBehaviorMixin, Parent): pass` and `class NewChildN(NewBehaviorMixin, ChildN): pass`, etc. (PS: Do you know a better way?) 

### examples

[Mixins and Python](https://www.ianlewis.org/en/mixins-and-python)

[werkzeug's request and response system](http://werkzeug.pocoo.org/docs/0.14/wrappers/#module-werkzeug.wrappers)

