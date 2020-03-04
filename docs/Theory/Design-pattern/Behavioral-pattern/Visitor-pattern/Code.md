# Code Examples

## **Visitor** in Python

原文链接：[**Visitor** in Python](https://refactoring.guru/design-patterns/visitor/python/example)。原文代码使用了python3.5中引入的[Type Hints](https://www.python.org/dev/peps/pep-0484/)特性，所以需要在python3.5及以上版本才能够执行。

```python
from __future__ import annotations
from abc import ABC, abstractmethod
from typing import List


class Component(ABC):
    """
    The Component interface declares an `accept` method that should take the
    base visitor interface as an argument.
    """

    @abstractmethod
    def accept(self, visitor: Visitor) -> None:
        pass


class ConcreteComponentA(Component):
    """
    Each Concrete Component must implement the `accept` method in such a way
    that it calls the visitor's method corresponding to the component's class.
    """

    def accept(self, visitor: Visitor) -> None:
        """
        Note that we're calling `visitConcreteComponentA`, which matches the
        current class name. This way we let the visitor know the class of the
        component it works with.
        """

        visitor.visit_concrete_component_a(self)

    def exclusive_method_of_concrete_component_a(self) -> str:
        """
        Concrete Components may have special methods that don't exist in their
        base class or interface. The Visitor is still able to use these methods
        since it's aware of the component's concrete class.
        """

        return "A"


class ConcreteComponentB(Component):
    """
    Same here: visitConcreteComponentB => ConcreteComponentB
    """

    def accept(self, visitor: Visitor):
        visitor.visit_concrete_component_b(self)

    def special_method_of_concrete_component_b(self) -> str:
        return "B"


class Visitor(ABC):
    """
    The Visitor Interface declares a set of visiting methods that correspond to
    component classes. The signature of a visiting method allows the visitor to
    identify the exact class of the component that it's dealing with.
    """

    @abstractmethod
    def visit_concrete_component_a(self, element: ConcreteComponentA) -> None:
        pass

    @abstractmethod
    def visit_concrete_component_b(self, element: ConcreteComponentB) -> None:
        pass


"""
Concrete Visitors implement several versions of the same algorithm, which can
work with all concrete component classes.

You can experience the biggest benefit of the Visitor pattern when using it with
a complex object structure, such as a Composite tree. In this case, it might be
helpful to store some intermediate state of the algorithm while executing
visitor's methods over various objects of the structure.
"""


class ConcreteVisitor1(Visitor):
    def visit_concrete_component_a(self, element) -> None:
        print(f"{element.exclusive_method_of_concrete_component_a()} + ConcreteVisitor1")

    def visit_concrete_component_b(self, element) -> None:
        print(f"{element.special_method_of_concrete_component_b()} + ConcreteVisitor1")


class ConcreteVisitor2(Visitor):
    def visit_concrete_component_a(self, element) -> None:
        print(f"{element.exclusive_method_of_concrete_component_a()} + ConcreteVisitor2")

    def visit_concrete_component_b(self, element) -> None:
        print(f"{element.special_method_of_concrete_component_b()} + ConcreteVisitor2")


def client_code(components: List[Component], visitor: Visitor) -> None:
    """
    The client code can run visitor operations over any set of elements without
    figuring out their concrete classes. The accept operation directs a call to
    the appropriate operation in the visitor object.
    """

    # ...
    for component in components:
        component.accept(visitor)
    # ...


if __name__ == "__main__":
    components = [ConcreteComponentA(), ConcreteComponentB()]

    print("The client code works with all visitors via the base Visitor interface:")
    visitor1 = ConcreteVisitor1()
    client_code(components, visitor1)

    print("It allows the same client code to work with different types of visitors:")
    visitor2 = ConcreteVisitor2()
    client_code(components, visitor2)
```

## **Visitor** in C++

原文链接：[**Visitor** in C++](https://refactoring.guru/design-patterns/visitor/cpp/example)。原文代码使用了C++11的特性，其次原文代码缺少头文件，必须添加后才能够编译通过。

```c++
#include <array>
#include <iostream>
/**
 * The Visitor Interface declares a set of visiting methods that correspond to
 * component classes. The signature of a visiting method allows the visitor to
 * identify the exact class of the component that it's dealing with.
 */
class ConcreteComponentA;
class ConcreteComponentB;

class Visitor {
 public:
  virtual void VisitConcreteComponentA(const ConcreteComponentA *element) const = 0;
  virtual void VisitConcreteComponentB(const ConcreteComponentB *element) const = 0;
};

/**
 * The Component interface declares an `accept` method that should take the base
 * visitor interface as an argument.
 */

class Component {
 public:
  virtual ~Component() {}
  virtual void Accept(Visitor *visitor) const = 0;
};

/**
 * Each Concrete Component must implement the `Accept` method in such a way that
 * it calls the visitor's method corresponding to the component's class.
 */
class ConcreteComponentA : public Component {
  /**
   * Note that we're calling `visitConcreteComponentA`, which matches the
   * current class name. This way we let the visitor know the class of the
   * component it works with.
   */
 public:
  void Accept(Visitor *visitor) const override {
    visitor->VisitConcreteComponentA(this);
  }
  /**
   * Concrete Components may have special methods that don't exist in their base
   * class or interface. The Visitor is still able to use these methods since
   * it's aware of the component's concrete class.
   */
  std::string ExclusiveMethodOfConcreteComponentA() const {
    return "A";
  }
};

class ConcreteComponentB : public Component {
  /**
   * Same here: visitConcreteComponentB => ConcreteComponentB
   */
 public:
  void Accept(Visitor *visitor) const override {
    visitor->VisitConcreteComponentB(this);
  }
  std::string SpecialMethodOfConcreteComponentB() const {
    return "B";
  }
};

/**
 * Concrete Visitors implement several versions of the same algorithm, which can
 * work with all concrete component classes.
 *
 * You can experience the biggest benefit of the Visitor pattern when using it
 * with a complex object structure, such as a Composite tree. In this case, it
 * might be helpful to store some intermediate state of the algorithm while
 * executing visitor's methods over various objects of the structure.
 */
class ConcreteVisitor1 : public Visitor {
 public:
  void VisitConcreteComponentA(const ConcreteComponentA *element) const override {
    std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor1\n";
  }

  void VisitConcreteComponentB(const ConcreteComponentB *element) const override {
    std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor1\n";
  }
};

class ConcreteVisitor2 : public Visitor {
 public:
  void VisitConcreteComponentA(const ConcreteComponentA *element) const override {
    std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor2\n";
  }
  void VisitConcreteComponentB(const ConcreteComponentB *element) const override {
    std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor2\n";
  }
};
/**
 * The client code can run visitor operations over any set of elements without
 * figuring out their concrete classes. The accept operation directs a call to
 * the appropriate operation in the visitor object.
 */
void ClientCode(std::array<const Component *, 2> components, Visitor *visitor) {
  // ...
  for (const Component *comp : components) {
    comp->Accept(visitor);
  }
  // ...
}

int main() {
  std::array<const Component *, 2> components = {new ConcreteComponentA, new ConcreteComponentB};
  std::cout << "The client code works with all visitors via the base Visitor interface:\n";
  ConcreteVisitor1 *visitor1 = new ConcreteVisitor1;
  ClientCode(components, visitor1);
  std::cout << "\n";
  std::cout << "It allows the same client code to work with different types of visitors:\n";
  ConcreteVisitor2 *visitor2 = new ConcreteVisitor2;
  ClientCode(components, visitor2);

  for (const Component *comp : components) {
    delete comp;
  }
  delete visitor1;
  delete visitor2;

  return 0;
}
```

### 编译

```
g++ --std=c++11 Visitor-pattern.cc -o visitor-pattern
```



## geeksforgeeks Visitor design pattern

原文链接：[Visitor design pattern](https://www.geeksforgeeks.org/visitor-design-pattern/)

```java
interface ItemElement 
{ 
	public int accept(ShoppingCartVisitor visitor); 
} 

class Book implements ItemElement 
{ 
	private int price; 
	private String isbnNumber; 

	public Book(int cost, String isbn) 
	{ 
		this.price=cost; 
		this.isbnNumber=isbn; 
	} 

	public int getPrice() 
	{ 
		return price; 
	} 

	public String getIsbnNumber() 
	{ 
		return isbnNumber; 
	} 

	@Override
	public int accept(ShoppingCartVisitor visitor) 
	{ 
		return visitor.visit(this); 
	} 

} 

class Fruit implements ItemElement 
{ 
	private int pricePerKg; 
	private int weight; 
	private String name; 

	public Fruit(int priceKg, int wt, String nm) 
	{ 
		this.pricePerKg=priceKg; 
		this.weight=wt; 
		this.name = nm; 
	} 

	public int getPricePerKg() 
	{ 
		return pricePerKg; 
	} 

	public int getWeight() 
	{ 
		return weight; 
	} 

	public String getName() 
	{ 
		return this.name; 
	} 

	@Override
	public int accept(ShoppingCartVisitor visitor) 
	{ 
		return visitor.visit(this); 
	} 

} 

interface ShoppingCartVisitor 
{ 

	int visit(Book book); 
	int visit(Fruit fruit); 
} 

class ShoppingCartVisitorImpl implements ShoppingCartVisitor 
{ 

	@Override
	public int visit(Book book) 
	{ 
		int cost=0; 
		//apply 5$ discount if book price is greater than 50 
		if(book.getPrice() > 50) 
		{ 
			cost = book.getPrice()-5; 
		} 
		else
			cost = book.getPrice(); 
			
		System.out.println("Book ISBN::"+book.getIsbnNumber() + " cost ="+cost); 
		return cost; 
	} 

	@Override
	public int visit(Fruit fruit) 
	{ 
		int cost = fruit.getPricePerKg()*fruit.getWeight(); 
		System.out.println(fruit.getName() + " cost = "+cost); 
		return cost; 
	} 

} 

class ShoppingCartClient 
{ 

	public static void main(String[] args) 
	{ 
		ItemElement[] items = new ItemElement[]{new Book(20, "1234"), 
							new Book(100, "5678"), new Fruit(10, 2, "Banana"), 
							new Fruit(5, 5, "Apple")}; 

		int total = calculatePrice(items); 
		System.out.println("Total Cost = "+total); 
	} 

	private static int calculatePrice(ItemElement[] items) 
	{ 
		ShoppingCartVisitor visitor = new ShoppingCartVisitorImpl(); 
		int sum=0; 
		for(ItemElement item : items) 
		{ 
			sum = sum + item.accept(visitor); 
		} 
		return sum; 
	} 

} 

```



```c++
//Write CPP code here 

#include <iostream> 
using namespace std; 

class Stock 
{ 
public: 
	virtual void accept(class Visitor *) = 0; 
	
}; 

class Apple : public Stock 
{ 
public: 
	/*virtual*/ void accept(Visitor *); 
	void buy() 
	{ 
		cout << "Apple::buy\n"; 
	} 
	void sell() 
	{ 
		cout << "Apple::sell\n"; 
	} 
	
}; 
class Google : public Stock 
{ 
public: 
	/*virtual*/ void accept(Visitor *); 
	void buy() 
	{ 
		cout << "Google::buy\n"; 
	} 

	void sell() 
	{ 
		cout << "Google::sell\n"; 
	} 
}; 

class Visitor 
{ 
public: 
	virtual void visit(Apple *) = 0; 
	virtual void visit(Google *) = 0; 
	//private: 
	static int m_num_apple, m_num_google; 
	void total_stocks() 
	{ 
		cout << "m_num_apple " << m_num_apple 
			<< ", m_num_google " << m_num_google << '\n'; 
	} 
}; 
int Visitor::m_num_apple = 0; 
int Visitor::m_num_google = 0; 
class BuyVisitor : public Visitor 
{ 
public: 
	BuyVisitor() 
	{ 
		m_num_apple = m_num_google = 0; 
	} 
	/*virtual*/ void visit(Apple *r) 
	{ 
		++m_num_apple; 
		r->buy(); 
		cout << "m_num_apple " << m_num_apple << endl; 
	} 
	/*virtual*/ void visit(Google *b) 
	{ 
		++m_num_google; 
		b->buy(); 
		cout << " m_num_google " << m_num_google << '\n'; 
	} 
}; 

class SellVisitor : public Visitor 
{ 
public: 
	/*virtual*/ void visit(Apple *a) 
	{ 
		
		--m_num_apple; 
		a->sell(); 
		cout << "m_num_apple " << m_num_apple << endl; 
	} 
	/*virtual*/ void visit(Google *g) 
	{ 
		--m_num_google; 
		g->sell(); 
		cout << "m_num_google " << m_num_google << endl; 
	} 
}; 

void Apple::accept(Visitor *v) 
{ 
	v->visit(this); 
} 

void Google::accept(Visitor *v) 
{ 
	v->visit(this); 
} 

int main() 
{ 
	Stock *set[] = { new Apple, new Google, new Google, 
					new Apple, new Apple, 0 }; 

	BuyVisitor buy_operation; 
	SellVisitor sell_operation; 
	for (int i = 0; set[i]; i++) 
	{ 
		set[i]->accept(&buy_operation); 
	} 
	buy_operation.total_stocks(); 

	for (int i = 0; set[i]; i++) 
	{ 

		set[i]->accept(&sell_operation); 
	} 
	sell_operation.total_stocks(); 
} 

```

