# Java Callable & Runnable



## Java Callable VS Runnable

In Java, both `Callable` and `Runnable` are interfaces that represent tasks that can be executed by a thread. However, they have some key differences in terms of functionality and usage. Let's explore these differences in detail.

### Runnable

- **Definition**: `Runnable` is a functional interface that represents a task that can be executed by a thread. It contains a single method, `run()`, which does not return a result and cannot throw checked exceptions.

- **Method**: 
  ```java
  public interface Runnable {
      void run();
  }
  ```

- **Return Type**: The `run()` method does not return any value (its return type is `void`).

- **Exception Handling**: The `run()` method cannot throw checked exceptions. If you need to handle exceptions, you must do so within the `run()` method itself.

- **Usage**: You can use `Runnable` when you want to define a task that does not need to return a result. It is commonly used with `Thread` or `ExecutorService`.

#### Example of Runnable:
```java
public class MyRunnable implements Runnable {
    @Override
    public void run() {
        System.out.println("Running in a thread using Runnable");
    }

    public static void main(String[] args) {
        Thread thread = new Thread(new MyRunnable());
        thread.start();
    }
}
```

### Callable

- **Definition**: `Callable` is also a functional interface that represents a task that can be executed by a thread. It is similar to `Runnable`, but it can return a result and can throw checked exceptions.

- **Method**: 
  ```java
  public interface Callable<V> {
      V call() throws Exception;
  }
  ```

- **Return Type**: The `call()` method returns a value of type `V`, which is specified when you declare the `Callable` instance.

- **Exception Handling**: The `call()` method can throw checked exceptions, allowing for more flexible error handling.

- **Usage**: You can use `Callable` when you need to perform a task that returns a result or when you need to handle checked exceptions. It is commonly used with `ExecutorService`, which can return a `Future` object representing the result of the computation.

#### Example of Callable:
```java
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class MyCallable implements Callable<String> {
    @Override
    public String call() throws Exception {
        return "Result from Callable";
    }

    public static void main(String[] args) {
        ExecutorService executor = Executors.newSingleThreadExecutor();
        Future<String> future = executor.submit(new MyCallable());

        try {
            String result = future.get(); // Blocking call to get the result
            System.out.println("Callable result: " + result);
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        } finally {
            executor.shutdown();
        }
    }
}
```

### Key Differences Between Callable and Runnable

| Feature            | Runnable                                         | Callable                                |
| ------------------ | ------------------------------------------------ | --------------------------------------- |
| Return Type        | `void`                                           | Generic type `V`                        |
| Exception Handling | Cannot throw checked exceptions                  | Can throw checked exceptions            |
| Usage              | Used for tasks that do not return a result       | Used for tasks that return a result     |
| Execution          | Can be executed by `Thread` or `ExecutorService` | Typically executed by `ExecutorService` |
| Result Retrieval   | No result retrieval                              | Result can be retrieved using `Future`  |

### Summary

- Use **`Runnable`** when you want to define a task that does not return a result and does not need to throw checked exceptions.
- Use **`Callable`** when you need to define a task that returns a result or may throw checked exceptions.

Both interfaces are essential for concurrent programming in Java, and choosing between them depends on the specific requirements of your task.



### See also

stackoverflow [The difference between the Runnable and Callable interfaces in Java](https://stackoverflow.com/questions/141284/the-difference-between-the-runnable-and-callable-interfaces-in-java) 

baeldung [Runnable vs. Callable in Java](https://www.baeldung.com/java-runnable-callable) 



## docs.oracle 8 [Interface Callable<V>](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/Callable.html)



## docs.oracle 8 [Interface Runnable](https://docs.oracle.com/javase/8/docs/api/java/lang/Runnable.html)