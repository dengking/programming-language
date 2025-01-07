# [eBay](https://github.com/eBay)/**[task-graph-toolkit](https://github.com/eBay/task-graph-toolkit)** 



- Compile time checking of dependency types and task orchestration(编排) wiring(连接).

  > NOTE: 这是什么意思？因为graph不是在运行时动态构建的，而是在程序编译之前构建的，因此compiler可以在compile time进行检查



## Profiler

- Task decorator profiles time taken by task processing along with time spent blocking for its dependencies.
- Profiler data model that allows task processing times and dependencies to be visualized in the profilerview node module. Task graphs and profiling images in the documentation are screenshots from the tool that displays various views of the profiling data.

## Documentation



## Callable Executor

### 用法

```java
public class NumberTask extends Task implements ICallableTask<Integer>
```

`Task` 和 `ICallableTask` 如何进行关联的？

`Task` 包含公共实现和 `ICallableTask` 定义接口 



### Task

> NOTE: 
>
> 相当于graph的node，在例子中，root task `TaskTest` 中将 `ICallableTaskExecutor` 作为构造函数的入参数，每个构建的task都需要通过调用 `executor.addTask` 加入到 executor 中 

```java
public abstract class Task
```

Here are some useful constraints that should be applied to task implementations:

- Tasks should not read from or write to any **global shared contexts**. This includes writing tracking data to **shared tracking objects**. Task tracking data should be logged in the `ResponseContext` to decouple what is being tracked from the system that records it. It also simplifies unit testing tracking required by the task.

- **Task dependencies** should be limited to the scope of the task with no unnecessary data passed in. Avoid passing contexts that contain data outside the scope of the task, especially large application scope contexts.

  > 翻译: 任务依赖项应该限制在任务的范围内，不能传入不必要的数据。避免传递包含任务范围之外数据的上下文，尤其是大型应用程序范围上下文。

- Tasks should not handle unrecoverable exceptions that result in a null response from the task. The decorator added by the executor will ensure exceptions are logged appropriately with the corresponding task name.

  > NOTE: 在task内部是不需要处理导致返回空的exception的，这是因为框架实现了捕获exception功能

- Tasks should not write to any of its dependencies data models. Ideally this should be enforced by all contexts and task result objects being immutable.

  > NOTE: 这段话要如何理解？





#### `ExecType`



### ResponseContext

> NOTE:
>
> 在 `ProfileDecorator` 中将profile data写入到ResponseContext中

The response context is a class owned by each task that records data associated with the task's processing that is not part of the data model returned by the task's `Callable` method.

Data managed by this class:

- **Profiler data** \- The **profiler decorator**(`ProfileDecorator`) records how long the task takes to complete along with the time spent waiting for its dependencies. It also contains the tasks that consume the output of the task's `Callable` method. This allows the **task dependency graph** to be visualized in the profilerview tool(可视化依赖图).
- Diagnostic data(诊断数据)
- Tracking data 
- Errors recorded during task processing

### ICallableTask

Defines the interface of a task. Extends Java Callable interface with these methods:

- `getContext` - accessor to the task response context. Used by the executor to aggregate the response contexts of the tasks it manages.

- `getTaskConfig` - accessor to the task configuration.

- `waitForDependencies` - used by the `ProfileDecorator` to time how long the task blocks for its dependencies to complete before executing. The default implementation of this method is to block for all the task's dependencies to complete but it does allow for custom implementations. E.g. if a task has a critical dependency but has other dependencies it doesn't have to wait for.

  > NOTE: 最后一句话如何理解?





### ProfileDecorator

> NOTE:
>
> 这是典型的使用decorator模式，通过它，框架自动注入的了对task的profile功能
>
> 通过Future的方式实现task之间的dependency。

Decorator class for `ICallableTask` added by the executor class for each task. Handles the following:

- Times how long the task waits for its dependencies to complete(等了多久才执行) and how long the task's Callable method takes to execute(执行了多久). Logs this data in the profiler object in the task's `ResponseContext`.
- Handles exceptions thrown by task's callable method and logs them with the task name(捕获exception).



### SynchronousFuture

Implementation of the Java Future interface that calls the corresponding ICallableTask.call() method synchronously on the thread of a dependent task.

### ICallableTaskFuture

This extension of the Java `Future` interface which adds an additional `get(ICallableTask<?> caller)` method that allows the underlying task to keep track of consumers of its result in the ResponseContext. This enables the profilerview tool to visually represent the DAG of tasks. There is also an accessor method for the underlying `ICallableTask`.

> NOTE:
>
> task 和 future的关系: task是promise，future表示task的返回值，最能够体现它们之间关系的是:
>
> ```java
> /**
>  * Simple interface for submitting tasks to an executor.
>  */
> public interface ICallableTaskExecutor {
> 
>     /**
>      * Submits a task to the executor.  Wraps the task in a profile decorator.
>      */
>     <T> ICallableTaskFuture<T> addTask(ICallableTask<T> task);
>     
>     /**
>      * Fetch a task by name.
>      */
>     <T> ICallableTaskFuture<T> getTask(String name);
> }
> ```
>
> 显然，增加一个task后，就能够拿到它的future



## Workflow

While the Callable Executor interfaces and classes offer useful extensions to basic Java concurrency patterns they don't solve the problem of managing complex applications that can have graphs of hundreds of dependent tasks. The Workflow patterns address this by providing a way to encapsulate groups of tasks into coherent, reusable components of business logic.



task: 

`ITaskFactory`: Interface for creating tasks 

`TaskInstance`  

`TaskInstanceHolder` 

workflow:

`IWorkflow` 

`IWorkflowFactory`: 需要自定义，比如 `TestWorkflowFactory` 

`WorkflowTask`: Task that creates a workflow, calls the workflow executor and then collects the response contexts of tasks executed，它是一个task，通过它应该能够找到 `Workflow`  和 `Task` 之间的关系，它一般作为root 

`Workflow` 

`IWorkflowBuilder`: Interface used by task factories to add their tasks to a workflow. 

`IWorkflowExecutor`: 比如 `WaitForCriticalDataOnlyExecutor`，模板参数的类型为 Response 的类型 



需要自定义 IWorkflowFactory、IWorkflowExecutor、IWorkflowBuilder 



首先构建一个 `WorkflowTask` 对象，它作为root task，它的构造函数参数有:

- `IWorkflowExecutor`
- `IWorkflowFactory`

显然，root task是没有dependency的，然后调用 `task.call()` 驱动整个graph的执行，root task是 `WorkflowTask` 对象，在它的call中，会首先创建一个

top level workflow相当于





## TODO

1 `class Task`  和 `interface ICallableTask` 具有如下相同接口: 

- `waitForDependencies` 
- `getDependencies`



2 `ExecutorTest` 中，第219行，为什么 `four` 没有加入到executor中？

```java
ICallableTaskFuture<Integer> one = executor.addTask(new NumberTask(taskConfig, 1));
ICallableTaskFuture<Integer> two = executor.addTask(new NumberTask(taskConfig, 2));
ICallableTaskFuture<Integer> three = executor.addTask(new NumberTask(taskConfig, 3));
ICallableTaskFuture<Integer> four = new CallableTaskResultNull<Integer>();
```



3 `TestWorkflowFactory` 的 `create` 方法中为每个新创建的 `Workflow` 都构建了一个新的 `JavaCallableTaskExecutor` 







