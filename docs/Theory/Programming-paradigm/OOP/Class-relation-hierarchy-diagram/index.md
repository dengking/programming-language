# Class relation、Class  hierarchy、class diagram

## 基于class relation来进行思考

### 基于inheritance关系来思考

OOP中的inheritance关系是典型的**nesting关系**，因此它能够呈现出hierarchy structure（如果仅仅允许single-inheritance的话，则是tree），这就是我们常说的**class hierarchy** ，这是典型的以**结构化思维**来进行思考，基于class hierarchy structure来思考OOP中的内容，能够让我们快速的理解OOP中的许多内容，具体分析如下：

> TODO 需要补充一些关于class hierarchy的文章的链接



#### Hierarchy-based implementation

本节标题的含义是：基于hierarchy的实现，OOP中的很多内容都是基于hierarchy来实现的：

##### Initialization  and deinitialization  order

|                                            | 说明                                                       |
| ------------------------------------------ | ---------------------------------------------------------- |
| initialization order（construction order） | object的构造，涉及基类的构造，这就涉及initialization order |
| deinitialization/destruction order         | object                                                     |

> TODO 需要添加C++中的的链接



##### Implementation of polymorphism

function find order：沿着hierarchy进行搜索。

需要结合Python、C++的implementation来进行说明：

- C++ vtable
- Python C3 serialization

> TODO 需要添加链接





## wikipedia [Class diagram](https://en.wikipedia.org/wiki/Class_diagram)



### Relationships

#### Class-level relationships

> NOTE: 两种不同的语义

##### Generalization/Inheritance

##### Realization/Implementation