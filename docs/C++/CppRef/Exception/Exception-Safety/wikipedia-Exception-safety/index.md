# wikipedia [Exception safety](https://en.wikipedia.org/wiki/Exception_safety)

There are several levels of exception safety (in decreasing order of safety):

## No-throw guarantee

also known as **failure transparency**: Operations are guaranteed to succeed and satisfy all requirements even in exceptional situations. If an exception occurs, it will be handled internally and not observed by clients.

## Strong exception safety

also known as **commit or rollback semantics**: Operations can fail, but failed operations are guaranteed to have no side effects, leaving the original values intact(原封不动).[[4\]](https://en.wikipedia.org/wiki/Exception_safety#cite_note-4)

## Basic exception safety

also known as a **no-leak guarantee**: Partial execution of failed operations can result in side effects, but all [invariants](https://en.wikipedia.org/wiki/Invariant_(computer_science)) are preserved and there are no [resource leaks](https://en.wikipedia.org/wiki/Resource_leak) (including [memory leaks](https://en.wikipedia.org/wiki/Memory_leak)). Any stored data will contain valid values which may differ from the original values.

## No exception safety

No guarantees are made.



Usually, at least basic exception safety is required to write robust code in such languages. Higher levels of safety can sometimes be difficult to achieve, and might incur an overhead due to extra copying. A key mechanism for exception safety is a `finally` clause, or similar [exception handling syntax](https://en.wikipedia.org/wiki/Exception_handling_syntax), which ensure that certain code is *always* run when a block is exited, including by exceptions. Several languages have constructs that simplify this, notably using the [dispose pattern](https://en.wikipedia.org/wiki/Dispose_pattern), named as `using`, `with`, or `try`-with-resources.

> NOTE: 这其实和resource management密切相关的