# gracicot [Concept-Model Idiom Part One: A new look at polymorphism](https://gracicot.github.io/conceptmodel/2017/09/13/concept-model-part1.html)

Ah, good old Oriented Object Programming. Have a problem? Just make an interface, and implement it! Simple as that heh? Well no. Polymorphism as done today with classic OOP is intrusive, forces polymorphic behavior even in place where it’s not really needed, almost always imply heap allocation, needs to carry the vtable pointer and the list goes on. Fortunatly, other pattern and idiom exists. Here’s an approach that might change how you see polymophism, and the problem it tries to solve. Let me introduce the Concept-Model idiom, also called runtime concept, or virtual concept.

> NOTE: 
>
> 1、"Polymorphism as done today with classic OOP is intrusive" 要如何理解？在 mropert [Polymorphic ducks](https://mropert.github.io/2017/11/30/polymorphic_ducks/) 中有一个回答。

I’m not the inventor of this idiom. I’m sure everyone doing OOP ended up doing something that look like it in some way (hint: Adapter pattern). The first time I saw the idea in a concrete way was in the Sean Parent’s talk [Inheritance Is The Base Class of Evil](https://channel9.msdn.com/Events/GoingNative/2013/Inheritance-Is-The-Base-Class-of-Evil). Thanks to him, I manage to use it and practice it a lot. I want to share what I’ve learned by playing around with this pattern.

Let’s start with a simple example:

```C++
// Our interface.
struct abstract_task {
    virtual void process() = 0;
    virtual ~abstract_task() = default;
};

// An implementation of our interface.
struct print_task : abstract_task {
    void process() override {
        std::cout << "Print Task" << std::endl;
    }
};

// A type erased list of tasks.
std::vector<std::unique_ptr<abstract_task>> tasks;

// A function that push a new task in the list.
void push(std::unique_ptr<abstract_task> task) {
    tasks.emplace_back(std::move(task));
}

// execute all tasks and clear the list.
void run() {
    for(auto&& task : tasks) {
        task->process();
    }
    
    tasks.clear();
}
```

> NOTE: 
>
> 1、上述例子是典型的: Polymorphic-object-container

## Concept-Model: The adapter pattern on steroids

> NOTE: 
>
> 1、"on steroids"的意思是: 以更有力或极端的方式
>
> 2、上述adapter pattern，其实更多的是指adaptor external polymorphism pattern

In this section, I’ll explain the process of going from classical OOP to the Concept-Model idiom. I’ll try to break this into many small steps to make understanding easier.



```C++
struct task {
    task(some_library_task task) noexcept :
        self{std::make_unique<library_model_t>(std::move(t))} {}
    task(print_task task) noexcept :
        self{std::make_unique<print_model_t>(std::move(t))} {}
    task(some_other_task task) noexcept :
        self{std::make_unique<some_other_model_t>(std::move(t))} {}
    
    void process() {
        self->process();
    }
    
private:
    // This is our interface, now named concept_t instead of abstract_task
    struct concept_t {
        virtual ~concept_t() = default;
        virtual void process() = 0;
    };
    
    // We name our struct `model` instead of `adapter`
    struct library_model_t : concept_t {
        library_model_t(some_library_task s) noexcept : self{std::move(s)} {}
        void process() override { self.process(); }
        some_library_task self;
    };
    
    struct print_model_t : concept_t {
        library_model_t(print_task s) noexcept : self{std::move(s)} {}
        void process() override { self.process(); }
        print_task self;
    };
    
    struct some_other_model_t : concept_t {
        library_model_t(some_other_task s) noexcept : self{std::move(s)} {}
        void process() override { self.process(); }
        some_other_task self;
    };
 
    // We quite know it's wrapped. Let's name it self
    std::unique_ptr<concept_t> self;
};
```

