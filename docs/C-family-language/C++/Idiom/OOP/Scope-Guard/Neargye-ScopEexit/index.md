# [Neargye](https://github.com/Neargye)/[scope_guard](https://github.com/Neargye/scope_guard)



## [Examples](https://github.com/Neargye/scope_guard/blob/master/example)

- [Scope Guard on exit](https://github.com/Neargye/scope_guard/blob/master/example/scope_exit_example.cpp)

    ```C++
    std::fstream file("test.txt");
    SCOPE_EXIT{ file.close(); }; // File closes when exit the enclosing scope or errors occur.
    ```

- [Scope Guard on fail](https://github.com/Neargye/scope_guard/blob/master/example/scope_fail_example.cpp)

    ```C++
    persons.push_back(person); // Add the person to db.
    SCOPE_FAIL{ persons.pop_back(); }; // If errors occur, we should roll back.
    ```

- [Scope Guard on success](https://github.com/Neargye/scope_guard/blob/master/example/scope_success_example.cpp)

    ```C++
    person = new Person{/*...*/};
    // ...
    SCOPE_SUCCESS{ persons.push_back(person); }; // If no errors occur, we should add the person to db.
    ```

- Custom Scope Guard

    ```C++
    persons.push_back(person); // Add the person to db.
    
    MAKE_SCOPE_EXIT(scope_exit) { // Following block is executed when exit the enclosing scope or errors occur.
      persons.pop_back(); // If the db insertion fails, we should roll back.
    };
    // MAKE_SCOPE_EXIT(name) {action} - macro is used to create a new scope_exit object.
    scope_exit.dismiss(); // An exception was not thrown, so don't execute the scope_exit.
    ```

    ```C++
    persons.push_back(person); // Add the person to db.
    
    auto scope_exit = make_scope_exit([]() { persons.pop_back(); });
    // make_scope_exit(A&& action) - function is used to create a new scope_exit object. It can be instantiated with a lambda function, a std::function<void()>, a functor, or a void(*)() function pointer.
    // ...
    scope_exit.dismiss(); // An exception was not thrown, so don't execute the scope_exit.
    ```

- With Scope Guard

    ```C++
    std::fstream file("test.txt");
    WITH_SCOPE_EXIT({ file.close(); }) { // File closes when exit the enclosing with scope or errors occur.
      // ...
    };
    ```

## Read code

### [scope_guard](https://github.com/Neargye/scope_guard)/[include](https://github.com/Neargye/scope_guard/tree/master/include)/[scope_guard.hpp](https://github.com/Neargye/scope_guard/blob/master/include/scope_guard.hpp)

它的实现和[SergiusTheBest](https://github.com/SergiusTheBest)/[ScopeExit](https://github.com/SergiusTheBest/ScopeExit)比较类似，都借助了macro、lambda。显然它的实现是更加完备的。它采用了policy-based design

