# [Inversion of control](https://en.wikipedia.org/wiki/Inversion_of_control)

In [software engineering](https://en.wikipedia.org/wiki/Software_engineering), **inversion of control** (**IoC**) is a design principle in which custom-written portions of a [computer program](https://en.wikipedia.org/wiki/Computer_program) receive the [flow of control](https://en.wikipedia.org/wiki/Control_flow) from a generic [framework](https://en.wikipedia.org/wiki/Software_framework). A [software architecture](https://en.wikipedia.org/wiki/Software_architecture) with this design inverts control as compared to traditional [procedural programming](https://en.wikipedia.org/wiki/Procedural_programming): in traditional programming, the custom code that expresses the purpose of the program [calls](https://en.wikipedia.org/wiki/Function_call#Main_concepts) into reusable libraries to take care of generic tasks, but with inversion of control, it is the framework that calls into the custom, or task-specific, code.

Inversion of control is used to increase [modularity](https://en.wikipedia.org/wiki/Modularity_(programming)) of the program and make it [extensible](https://en.wikipedia.org/wiki/Extensible_programming),[[1\]](https://en.wikipedia.org/wiki/Inversion_of_control#cite_note-1) and has applications in [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming) and other [programming paradigms](https://en.wikipedia.org/wiki/Programming_paradigm). The term was used by Michael Mattsson in a thesis[[2\]](https://en.wikipedia.org/wiki/Inversion_of_control#cite_note-2), taken from there[[3\]](https://en.wikipedia.org/wiki/Inversion_of_control#cite_note-3) by Stefano Mazzocchi and popularized by him in 1999 in a defunct Apache Software Foundation project, [Avalon](https://en.wikipedia.org/wiki/Apache_Avalon), then further popularized in 2004 by [Robert C. Martin](https://en.wikipedia.org/wiki/Robert_Cecil_Martin) and [Martin Fowler](https://en.wikipedia.org/wiki/Martin_Fowler).

The term is related to, but different from, the [dependency inversion principle](https://en.wikipedia.org/wiki/Dependency_inversion_principle), which concerns itself with [decoupling dependencies](https://en.wikipedia.org/wiki/Dependency_(computer_science)) between [high-level and low-level](https://en.wikipedia.org/wiki/High-_and_low-level) [layers](https://en.wikipedia.org/wiki/Abstraction_layer) through shared [abstractions](https://en.wikipedia.org/wiki/Abstraction_(computing)). The general concept is also related to [event-driven programming](https://en.wikipedia.org/wiki/Event-driven_programming) in that it is often implemented using IoC, so that the custom code is commonly only concerned with the handling of events, whereas the [event loop](https://en.wikipedia.org/wiki/Event_loop) and dispatch of events/messages is handled by the framework or the runtime environment.

## Overview

As an example, with traditional programming, the [main function](https://en.wikipedia.org/wiki/Main_function) of an application might make function calls into a menu library to display a list of available [commands](https://en.wikipedia.org/wiki/Command_(computing)) and query the user to select one.[[4\]](https://en.wikipedia.org/wiki/Inversion_of_control#cite_note-FowlerDI-4) The library thus would return the chosen option as the value of the function call, and the main function uses this value to execute the associated command. This style was common in [text based interfaces](https://en.wikipedia.org/wiki/Text_based_interface). For example, an [email client](https://en.wikipedia.org/wiki/Email_client) may show a screen with commands to load new mails, answer the current mail, start a new mail, etc., and the program execution would block until the user presses a key to select a command.

With **inversion of control**, on the other hand, the program would be written using a [software framework](https://en.wikipedia.org/wiki/Software_framework) that knows common behavioral and graphical elements, such as [windowing systems](https://en.wikipedia.org/wiki/Windowing_system), menus, controlling the mouse, and so on. The custom code "fills in the blanks" for the **framework**, such as supplying a table of menu items and registering a code subroutine for each item, but it is the **framework** that monitors the user's actions and invokes the subroutine when a menu item is selected. In the mail client example, the framework could follow both the keyboard and mouse inputs and call the command invoked by the user by either means, and at the same time monitor the [network interface](https://en.wikipedia.org/wiki/Network_interface) to find out if new messages arrive and refresh the screen when some network activity is detected. The same framework could be used as the skeleton(框架) for a spreadsheet program or a text editor. Conversely, the framework knows nothing about Web browsers, spreadsheets or text editors; implementing their functionality takes custom code.

Inversion of control carries（携带，运输，传递） the strong connotation（内涵） that the reusable code and the problem-specific code are developed independently even though they operate together in an application. [Software frameworks](https://en.wikipedia.org/wiki/Software_framework), [callbacks](https://en.wikipedia.org/wiki/Callback_(computer_programming)), [schedulers](https://en.wikipedia.org/wiki/Scheduling_(computing)), [event loops](https://en.wikipedia.org/wiki/Event_loop), [dependency injection](https://en.wikipedia.org/wiki/Dependency_injection), and the [template method](https://en.wikipedia.org/wiki/Template_method) are examples of [design patterns](https://en.wikipedia.org/wiki/Design_pattern) that follow the inversion of control principle, although the term is most commonly used in the context of [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming).

Inversion of control serves the following design purposes:

- To [decouple](https://en.wikipedia.org/wiki/Object_decoupling) the execution of a task from implementation.
- To focus a module on the task it is designed for.
- To free modules from assumptions about how other systems do what they do and instead rely on [contracts](https://en.wikipedia.org/wiki/Design_by_contract).
- To prevent [side effects](https://en.wikipedia.org/wiki/Side_effect_(computer_science)) when replacing a module.

**Inversion of control** is sometimes facetiously referred to as the "Hollywood Principle: Don't call us, we'll call you".

## Description

In traditional programming, the [flow](https://en.wikipedia.org/wiki/Control_flow) of the [business logic](https://en.wikipedia.org/wiki/Business_logic) is determined by objects that are [statically bound](https://en.wikipedia.org/wiki/Static_binding) to one another. With inversion of control, the flow depends on the **object graph** that is built up during program execution. Such a dynamic flow is made possible by object interactions that are defined through abstractions. This [run-time binding](https://en.wikipedia.org/wiki/Late_binding) is achieved by mechanisms such as [dependency injection](https://en.wikipedia.org/wiki/Dependency_injection) or a [service locator](https://en.wikipedia.org/wiki/Service_locator_pattern). In IoC, the code could also be linked statically during compilation, but finding the code to execute by reading its description from [external configuration](https://en.wikipedia.org/wiki/Configuration_file) instead of with a direct reference in the code itself.

In dependency injection, a dependent [object](https://en.wikipedia.org/wiki/Object-oriented_programming) or module is coupled to the object it needs at [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)). Which particular object will satisfy the dependency during program execution typically cannot be known at [compile time](https://en.wikipedia.org/wiki/Compile_time)using [static analysis](https://en.wikipedia.org/wiki/Static_code_analysis). While described in terms of object interaction here, the principle can apply to other programming methodologies besides [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming).

In order for the running program to bind objects to one another, the objects must possess compatible [interfaces](https://en.wikipedia.org/wiki/Software_interface). For example, class `A` may delegate behavior to interface `I` which is implemented by class `B`; the program instantiates `A` and `B`, and then injects `B` into `A`.

***SUMMARY***:要想实现在[Overview](#Overview)中描述的思想，需要采用上面这段话中描述的实现方式。

## Implementation techniques

In [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), there are several basic techniques to implement inversion of control. These are:

- Using a [service locator pattern](https://en.wikipedia.org/wiki/Service_locator_pattern)
- Using **dependency injection**, for example
  - Constructor injection
  - Parameter injection
  - Setter injection
  - Interface injection
- Using a contextualized lookup
- Using [template method design pattern](https://en.wikipedia.org/wiki/Template_method_design_pattern)
- Using [strategy design pattern](https://en.wikipedia.org/wiki/Strategy_design_pattern)

In an original article by Martin Fowler,[[9\]](https://en.wikipedia.org/wiki/Inversion_of_control#cite_note-9) the first three different techniques are discussed. In a description about inversion of control types,[[10\]](https://en.wikipedia.org/wiki/Inversion_of_control#cite_note-10) the last one is mentioned. Often the contextualized lookup will be accomplished using a service locator。





