# lambda



# docs.oracle.tutorial [Lambda Expressions](https://docs.oracle.com/javase/tutorial/java/javaOO/lambdaexpressions.html)



## return value



### stackoverflow [Return value by lambda in Java](https://stackoverflow.com/questions/38540481/return-value-by-lambda-in-java)



#### [A](https://stackoverflow.com/a/38540546)

```java
package com.company;


import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Main {

    static int counter = 0;

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        ExecutorService es = Executors.newSingleThreadExecutor();

        // execute only takes Runnable
        es.execute(() -> counter++);

        // force the lambda to be Runnable
        final Future<?> submit = es.submit((Runnable) () -> counter++);
        System.out.println(submit.get());

        // returns a value so it's a Callable<Integer>
        final Future<Integer> submit2 = es.submit(() -> counter++);
        System.out.println(submit2.get());

        // returns nothing so it must be Runnable
        final Future<?> submit3 = es.submit(() -> System.out.println("counter: " + counter));
        System.out.println(submit3.get());

        es.shutdown();
    }

}

```

