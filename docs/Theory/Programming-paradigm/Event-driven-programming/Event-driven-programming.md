

# [Event-driven programming](https://en.wikipedia.org/wiki/Event-driven_programming)



In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), **event-driven programming** is a [programming paradigm](https://en.wikipedia.org/wiki/Programming_paradigm) in which the [flow of the program](https://en.wikipedia.org/wiki/Control_flow) is determined by [events](https://en.wikipedia.org/wiki/Event_(computing)) such as user actions ([mouse](https://en.wikipedia.org/wiki/Computer_mouse) clicks, key presses), [sensor](https://en.wikipedia.org/wiki/Sensor) outputs, or [messages](https://en.wikipedia.org/wiki/Message_passing) from other programs or [threads](https://en.wikipedia.org/wiki/Thread_(computer_science)). Event-driven programming is the dominant paradigm used in [graphical user interfaces](https://en.wikipedia.org/wiki/Graphical_user_interface) and other applications (e.g., JavaScript [web applications](https://en.wikipedia.org/wiki/Web_application)) that are centered on performing certain actions in response to [user input](https://en.wikipedia.org/wiki/Input/output). This is also true of programming for [device drivers](https://en.wikipedia.org/wiki/Device_driver) (e.g., [P](https://en.wikipedia.org/wiki/P_(programming_language)) in USB device driver stacks[[1\]](https://en.wikipedia.org/wiki/Event-driven_programming#cite_note-1)).

In an event-driven application, there is generally a [main loop](https://en.wikipedia.org/wiki/Event_loop) that listens for events, and then triggers a [callback function](https://en.wikipedia.org/wiki/Callback_(computer_programming)) when one of those events is detected. In [embedded systems](https://en.wikipedia.org/wiki/Embedded_system), the same may be achieved using [hardware interrupts](https://en.wikipedia.org/wiki/Hardware_interrupt) instead of a constantly running main loop. Event-driven programs can be written in any [programming language](https://en.wikipedia.org/wiki/Programming_language), although the task is easier in languages that provide [high-level abstractions](https://en.wikipedia.org/wiki/Abstraction_(computer_science)), such as [await](https://en.wikipedia.org/wiki/Async/await) and [closures](https://en.wikipedia.org/wiki/Closure_(computer_programming)).

## Event handlers

Main article: [Event handler](https://en.wikipedia.org/wiki/Event_handler)

### A trivial event handler

Because the code for checking for events and the [main loop](https://en.wikipedia.org/wiki/Main_loop) are common amongst applications, many programming frameworks take care of their implementation and expect the user to provide only the code for the event handlers. 

***SUMMARY***:programming framework always do things common

In this simple example there may be a call to an event handler called `OnKeyEnter()` that includes an argument with a string of characters, corresponding to what the user typed before hitting the ENTER key. To add two numbers, storage outside the event handler must be used. The implementation might look like below.



## Common uses

In addition, systems such as Node.js are also event-driven.[[4\]](https://en.wikipedia.org/wiki/Event-driven_programming#cite_note-4)