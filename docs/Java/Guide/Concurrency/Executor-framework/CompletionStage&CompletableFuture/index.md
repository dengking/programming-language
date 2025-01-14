# CompletionStage& CompletableFuture

## gpt-4o-mini: Java `CompletionStage` 

In Java, `CompletionStage` is an interface that represents a stage of a computation that may be completed in the future. It is part of the `java.util.concurrent` package and is a key component of the `CompletableFuture` class, which implements the `CompletionStage` interface.



### Key Features of `CompletionStage`

1. **Asynchronous Computation**: `CompletionStage` allows you to define a pipeline of asynchronous tasks that can be executed in a non-blocking manner.
2. **Chaining**: You can chain multiple stages together, where the output of one stage can be used as the input to the next. This is done using methods like `thenApply`, `thenAccept`, and `thenRun`.
3. **Combining Stages**: You can combine multiple `CompletionStage` instances using methods like `allOf` and `anyOf`, allowing you to wait for multiple asynchronous tasks to complete.
4. **Exception Handling**: It provides methods for handling exceptions that may occur during the computation, such as `handle`, `exceptionally`, and `whenComplete`.





## docs.oracle [Interface CompletionStage<T>](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/CompletionStage.html)



## docs.oracle [Class CompletableFuture<T>](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/CompletableFuture.html)





## CompletableFuture `thenRun` VS `thenAccept`

In Java's `CompletableFuture`, both `thenRun` and `thenAccept` are methods used to define actions that should be executed after a `CompletableFuture` completes. However, they serve different purposes and have different signatures. Here's a detailed comparison of the two:

### `thenRun`

- **Purpose**: `thenRun` is used to execute a `Runnable` action after the `CompletableFuture` completes, regardless of whether it completed normally or exceptionally.
- **Input**: It does not take any input from the previous stage. It simply runs the provided `Runnable`.
- **Return Type**: It returns a new `CompletableFuture<Void>`, which represents the completion of the action.
- **Use Case**: Use `thenRun` when you want to perform an action that does not depend on the result of the previous computation.

**Example**:

```java
import java.util.concurrent.CompletableFuture;

public class ThenRunExample {
    public static void main(String[] args) {
        CompletableFuture<Void> future = CompletableFuture.supplyAsync(() -> {
            // Simulate some computation
            sleep(1000);
            return "Hello, World!";
        }).thenRun(() -> {
            // This action does not depend on the result of the previous stage
            System.out.println("Computation is done!");
        });

        // Wait for the future to complete
        future.join();
    }

    private static void sleep(int milliseconds) {
        try {
            Thread.sleep(milliseconds);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
```

### `thenAccept`

- **Purpose**: `thenAccept` is used to execute a `Consumer` action that takes the result of the previous stage as input.
- **Input**: It takes a `Consumer<? super T>` as an argument, where `T` is the type of the result of the previous stage.
- **Return Type**: It returns a new `CompletableFuture<Void>`, which represents the completion of the action.
- **Use Case**: Use `thenAccept` when you want to perform an action that uses the result of the previous computation.

**Example**:

```java
import java.util.concurrent.CompletableFuture;

public class ThenAcceptExample {
    public static void main(String[] args) {
        CompletableFuture<Void> future = CompletableFuture.supplyAsync(() -> {
            // Simulate some computation
            sleep(1000);
            return "Hello, World!";
        }).thenAccept(result -> {
            // This action uses the result of the previous stage
            System.out.println("Received result: " + result);
        });

        // Wait for the future to complete
        future.join();
    }

    private static void sleep(int milliseconds) {
        try {
            Thread.sleep(milliseconds);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
```

### Summary of Differences

| Feature         | `thenRun`                                                    | `thenAccept`                                            |
| --------------- | ------------------------------------------------------------ | ------------------------------------------------------- |
| **Input**       | No input from the previous stage                             | Takes the result of the previous stage as input         |
| **Action Type** | Accepts a `Runnable`                                         | Accepts a `Consumer<T>`                                 |
| **Use Case**    | When you want to perform an action that does not depend on the result | When you want to perform an action that uses the result |
| **Return Type** | Returns `CompletableFuture<Void>`                            | Returns `CompletableFuture<Void>`                       |

### Conclusion

- Use `thenRun` when you want to execute an action that does not require the result of the previous computation.
- Use `thenAccept` when you want to perform an action that utilizes the result of the previous computation.

Choosing between these two methods depends on whether you need to access the result of the previous stage in your subsequent action.



## Example1

In java, I have 3 futures, how can i make the run at the same time and after all of them finish, I get the value

```java
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;

public class CompletableFutureExample {
    public static void main(String[] args) {
        // Create three CompletableFutures
        CompletableFuture<Integer> future1 = CompletableFuture.supplyAsync(() -> {
            // Simulate some computation
            sleep(1000);
            return 1; // Result of future1
        });

        CompletableFuture<Integer> future2 = CompletableFuture.supplyAsync(() -> {
            // Simulate some computation
            sleep(2000);
            return 2; // Result of future2
        });

        CompletableFuture<Integer> future3 = CompletableFuture.supplyAsync(() -> {
            // Simulate some computation
            sleep(1500);
            return 3; // Result of future3
        });

        // Combine all futures and wait for them to complete
        CompletableFuture<Void> allFutures = CompletableFuture.allOf(future1, future2, future3);

        // Wait for all futures to complete
        allFutures.join(); // This will block until all futures are done

        try {
            // Retrieve the results
            Integer result1 = future1.get();
            Integer result2 = future2.get();
            Integer result3 = future3.get();

            // Print the results
            System.out.println("Result 1: " + result1);
            System.out.println("Result 2: " + result2);
            System.out.println("Result 3: " + result3);
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }
    }

    // Helper method to simulate delay
    private static void sleep(int milliseconds) {
        try {
            Thread.sleep(milliseconds);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
```

### Explanation:

1. **Creating CompletableFutures**:
   - Each `CompletableFuture` is created using `CompletableFuture.supplyAsync()`, which runs the provided lambda expression asynchronously. In this example, each future simulates a computation by sleeping for a certain amount of time and then returning a result.
2. **Combining Futures**:
   - `CompletableFuture.allOf(future1, future2, future3)` creates a new `CompletableFuture` that completes when all the provided futures complete. This allows you to wait for all tasks to finish.
3. **Waiting for Completion**:
   - `allFutures.join()` blocks the current thread until all the futures complete. You can also use `allFutures.get()` if you want to handle exceptions.
4. **Retrieving Results**:
   - After all futures are complete, you can retrieve their results using `future1.get()`, `future2.get()`, and `future3.get()`. This will block until the respective future is complete, but since we already waited for all of them to finish, it should return immediately.
5. **Error Handling**:
   - The `try-catch` block is used to handle potential exceptions that may arise from calling `get()` on the futures.

This approach allows you to run multiple tasks concurrently and wait for all of them to finish before processing their results.

## Example2

In Java, how to get notified after all the Futures are finished

In Java, if you want to get notified after all `Future` tasks are finished, you can use the `CompletableFuture` class along with the `thenRun` or `thenAccept` methods to specify a callback that will be executed once all the futures are complete. This is particularly useful for handling the completion of asynchronous tasks without blocking the main thread.

Here's an example demonstrating how to achieve this:

```java
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;

public class CompletableFutureNotificationExample {
    public static void main(String[] args) {
        // Create three CompletableFutures
        CompletableFuture<Integer> future1 = CompletableFuture.supplyAsync(() -> {
            // Simulate some computation
            sleep(1000);
            return 1; // Result of future1
        });

        CompletableFuture<Integer> future2 = CompletableFuture.supplyAsync(() -> {
            // Simulate some computation
            sleep(2000);
            return 2; // Result of future2
        });

        CompletableFuture<Integer> future3 = CompletableFuture.supplyAsync(() -> {
            // Simulate some computation
            sleep(1500);
            return 3; // Result of future3
        });

        // Combine all futures and wait for them to complete
        CompletableFuture<Void> allFutures = CompletableFuture.allOf(future1, future2, future3);

        // Add a callback to be notified when all futures are complete
        allFutures.thenRun(() -> {
            try {
                // Retrieve the results
                Integer result1 = future1.get();
                Integer result2 = future2.get();
                Integer result3 = future3.get();

                // Print the results
                System.out.println("All futures completed:");
                System.out.println("Result 1: " + result1);
                System.out.println("Result 2: " + result2);
                System.out.println("Result 3: " + result3);
            } catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
        });

        // Optionally, you can do other work here while the futures are running
        System.out.println("Waiting for futures to complete...");

        // To keep the main thread alive until all futures are done
        allFutures.join(); // This is optional if you want to wait for completion
    }

    // Helper method to simulate delay
    private static void sleep(int milliseconds) {
        try {
            Thread.sleep(milliseconds);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
```

### Explanation:

1. **Creating CompletableFutures**:
   - Similar to the previous example, we create three `CompletableFuture` instances that perform some asynchronous computations.
2. **Combining Futures**:
   - We use `CompletableFuture.allOf(future1, future2, future3)` to create a new `CompletableFuture` that completes when all the provided futures complete.
3. **Adding a Callback**:
   - The `thenRun` method is used to specify a callback that will be executed once all futures are complete. Inside this callback, we retrieve the results of each future using `get()` and print them.
4. **Non-blocking Notification**:
   - The main thread can continue executing other tasks while waiting for the futures to complete. The callback will be executed automatically once all futures are done.
5. **Keeping the Main Thread Alive**:
   - The `allFutures.join()` call is optional if you want to keep the main thread alive until all futures are done. If you don't include it, the main thread may terminate before the callback is executed, especially if the program is running in a simple main method without any other blocking operations.



## Example3

In Java, how to get notified after all the Futures are finished when use `Executors.newCachedThreadPool` 

When using `Executors.newCachedThreadPool()` in Java, you can still utilize `CompletableFuture` to run tasks asynchronously and get notified when all of them are finished. The `newCachedThreadPool()` method creates a thread pool that creates new threads as needed but will reuse previously constructed threads when they are available.

Here's how you can achieve this:

1. Submit tasks to the thread pool using `CompletableFuture.supplyAsync()`.
2. Use `CompletableFuture.allOf()` to combine the futures.
3. Add a callback using `thenRun()` or `thenAccept()` to get notified when all tasks are complete.

Here's an example:

```Java
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class CompletableFutureWithCachedThreadPool {
    public static void main(String[] args) {
        // Create a cached thread pool
        ExecutorService executorService = Executors.newCachedThreadPool();

        // Create three CompletableFutures using the cached thread pool
        CompletableFuture<Integer> future1 = CompletableFuture.supplyAsync(() -> {
            // Simulate some computation
            sleep(1000);
            return 1; // Result of future1
        }, executorService);

        CompletableFuture<Integer> future2 = CompletableFuture.supplyAsync(() -> {
            // Simulate some computation
            sleep(2000);
            return 2; // Result of future2
        }, executorService);

        CompletableFuture<Integer> future3 = CompletableFuture.supplyAsync(() -> {
            // Simulate some computation
            sleep(1500);
            return 3; // Result of future3
        }, executorService);

        // Combine all futures and wait for them to complete
        CompletableFuture<Void> allFutures = CompletableFuture.allOf(future1, future2, future3);

        // Add a callback to be notified when all futures are complete
        allFutures.thenRun(() -> {
            try {
                // Retrieve the results
                Integer result1 = future1.get();
                Integer result2 = future2.get();
                Integer result3 = future3.get();

                // Print the results
                System.out.println("All futures completed:");
                System.out.println("Result 1: " + result1);
                System.out.println("Result 2: " + result2);
                System.out.println("Result 3: " + result3);
            } catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
        });

        // Optionally, you can do other work here while the futures are running
        System.out.println("Waiting for futures to complete...");

        // To keep the main thread alive until all futures are done
        allFutures.join(); // This is optional if you want to wait for completion

        // Shutdown the executor service
        executorService.shutdown();
    }

    // Helper method to simulate delay
    private static void sleep(int milliseconds) {
        try {
            Thread.sleep(milliseconds);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
```



## Example4

Yes, you can use `thenAccept` with a `CompletableFuture` created by `CompletableFuture.allOf()`, but you need to be aware of how `allOf` works and what it returns.

### Understanding `CompletableFuture.allOf()`

- `CompletableFuture.allOf()` takes an array of `CompletableFuture` instances and returns a new `CompletableFuture<Void>` that completes when all of the given futures complete.
- The result of `allOf` does not provide the results of the individual futures; it only indicates that all of them have completed.

### Using `thenAccept` with `CompletableFuture.allOf()`

Since `allOf` returns a `CompletableFuture<Void>`, you can use `thenAccept` to perform an action after all the futures have completed. However, since `thenAccept` does not receive any input from the previous stage (because the result is `Void`), you cannot directly access the results of the individual futures in the `thenAccept` block.

If you want to access the results of the individual futures after they have all completed, you would typically do this by calling `get()` on each of the individual futures after `allOf` completes.

### Example

Here's an example demonstrating how to use `CompletableFuture.allOf()` with `thenAccept`:

```java
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;

public class AllOfThenAcceptExample {
    public static void main(String[] args) {
        CompletableFuture<Integer> future1 = CompletableFuture.supplyAsync(() -> {
            sleep(1000);
            return 1;
        });

        CompletableFuture<Integer> future2 = CompletableFuture.supplyAsync(() -> {
            sleep(2000);
            return 2;
        });

        CompletableFuture<Integer> future3 = CompletableFuture.supplyAsync(() -> {
            sleep(1500);
            return 3;
        });

        // Combine all futures and wait for them to complete
        CompletableFuture<Void> allFutures = CompletableFuture.allOf(future1, future2, future3);

        // Use thenAccept to perform an action after all futures are complete
        allFutures.thenAccept(v -> {
            try {
                // Access the results of the individual futures
                Integer result1 = future1.get();
                Integer result2 = future2.get();
                Integer result3 = future3.get();

                // Print the results
                System.out.println("All futures completed:");
                System.out.println("Result 1: " + result1);
                System.out.println("Result 2: " + result2);
                System.out.println("Result 3: " + result3);
            } catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
        });

        // Optionally, you can do other work here while the futures are running
        System.out.println("Waiting for futures to complete...");

        // To keep the main thread alive until all futures are done
        allFutures.join(); // This will block until all futures are complete

        System.out.println("All tasks are done!");
    }

    // Helper method to simulate delay
    private static void sleep(int milliseconds) {
        try {
            Thread.sleep(milliseconds);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
```

### Explanation of the Example

1. **Creating Individual Futures**: We create three `CompletableFuture` instances that simulate some **asynchronous computations**.

2. **Combining Futures**: We use `CompletableFuture.allOf(future1, future2, future3)` to create a new `CompletableFuture<Void>` that completes when all three futures complete.

3. **Using `thenAccept`**: We use `thenAccept` to define an action that will be executed once all futures are complete. Inside this action, we call `get()` on each individual future to retrieve their results.

4. **Handling Exceptions**: We handle any exceptions that may occur when calling `get()` on the futures.

5. **Blocking for Completion**: We call `allFutures.join()` to block the main thread until all futures are complete.

### Conclusion

You can indeed use `thenAccept` with a `CompletableFuture` created by `CompletableFuture.allOf()`. However, since the result of `allOf` is `Void`, you will need to access the individual futures' results within the `thenAccept` block by calling `get()` on them. This allows you to perform actions based on the results of the individual futures after they have all completed.