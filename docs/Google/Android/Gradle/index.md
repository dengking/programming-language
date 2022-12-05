# [Gradle](https://gradle.org/)



## gradle model

### project

[Project](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html) 

> There is a one-to-one relationship between a `Project` and a `build.gradle` file



Authoring Gradle Builds # [Build Script Basics](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html) # [ Hello world](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html#sec:hello_world)

> You run a Gradle build using the `gradle` command. The `gradle` command looks for a file called `build.gradle` in the current directory.[[1](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html#_footnotedef_1)] We call this `build.gradle` file a *build script*, although strictly speaking it is a build configuration script, as we will see later. The build script defines a project and its tasks.



gradle 和 cmake、gn、mkdocs 非常类似。



通过创建 `build.gradle` 来定义一个project，在这个 `build.gradle` 中描述属于这个project的task。



### dependency graph、task



## Running Gradle Builds # [Command-Line Interface](https://docs.gradle.org/current/userguide/command_line_interface.html) 



```shell
$ gradle  [taskName...] [--option-name...]
$ gradlew [taskName...] [--option-name...]
```



### [Project reporting](https://docs.gradle.org/current/userguide/command_line_interface.html#sec:command_line_project_reporting) 



#### [ Listing projects](https://docs.gradle.org/current/userguide/command_line_interface.html#listing_projects) 



```shell
$ gradle  projects
$ gradlew projects
```



#### [ Listing tasks](https://docs.gradle.org/current/userguide/command_line_interface.html#sec:listing_tasks)



```shell
$ gradle  tasks
$ gradlew tasks
```



```shell
$ gradle  tasks --all
$ gradlew tasks --all
```



```shell
$ gradle  tasks --group="build setup"
$ gradlew tasks --group="build setup"
```



#### [Show task usage details](https://docs.gradle.org/current/userguide/command_line_interface.html#sec:show_task_details)



```shell
$ gradle  help --task someTask
$ gradlew help --task someTask
```



#### [Reporting dependencies](https://docs.gradle.org/current/userguide/command_line_interface.html#reporting_dependencies)



```shell
$ gradle  myTask --scan
$ gradlew myTask --scan
```



### [Executing tasks](https://docs.gradle.org/current/userguide/command_line_interface.html#sec:command_line_executing_tasks)



#### [Executing tasks in multi-project builds](https://docs.gradle.org/current/userguide/command_line_interface.html#executing_tasks_in_multi_project_builds)

In a [multi-project build](https://docs.gradle.org/current/userguide/intro_multi_project_builds.html#intro_multi_project_builds), subproject tasks can be executed with ":" separating subproject name and task name. The following are equivalent *when run from the root project*:

```shell
$ gradle :my-subproject:taskName
$ gradle my-subproject:taskName
```



#### [Executing multiple tasks](https://docs.gradle.org/current/userguide/command_line_interface.html#executing_multiple_tasks) 



## Authoring Gradle Builds # [Understanding the Build Lifecycle](https://docs.gradle.org/current/userguide/build_lifecycle.html)



We said earlier that the core of Gradle is a language for dependency based programming. 

> NOTE:
>
> 一、上面这段话中描述的"dependency based programming"是指在当代项目开发中，很多项目都需要使用已有的功能、解决方案，这些都被称为"dependency"，那么这就是涉及如何来管理dependency

In Gradle terms this means that you can define tasks and dependencies between tasks. Gradle guarantees that these tasks are executed in the order of their dependencies, and that each task is executed only once. These tasks form a [Directed Acyclic Graph](http://en.wikipedia.org/wiki/Directed_acyclic_graph). There are build tools that build up such a **dependency graph** as they execute their tasks. Gradle builds the complete **dependency graph** *before* any task is executed. This lies at the heart of Gradle and makes many things possible which would not be possible otherwise.

> NOTE:
>
> 一、gradle的dependency graph中，node是task，边表达的是dependency关系

Your build scripts configure this dependency graph. Therefore they are strictly speaking *build configuration scripts*.

> NOTE:
>
> 一、`build.gradle` 就是  *build configuration scripts* 



### [Build phases](https://docs.gradle.org/current/userguide/build_lifecycle.html#sec:build_phases)

A Gradle build has three distinct phases.

| phase              |                                                              |      |
| ------------------ | ------------------------------------------------------------ | ---- |
| **Initialization** | Gradle supports single and multi-project builds. During the initialization phase, Gradle determines which projects are going to take part in the build, and creates a [Project](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html) instance for each of these projects. |      |
| **Configuration**  | During this phase the project objects are configured. The build scripts of *all* projects which are part of the build are executed. |      |
| **Execution**      | Gradle determines the subset of the tasks, created and configured during the configuration phase, to be executed. The subset is determined by the task name arguments passed to the `gradle` command and the current directory. Gradle then executes each of the selected tasks. |      |



### [Settings file](https://docs.gradle.org/current/userguide/build_lifecycle.html#sec:settings_file)



## Authoring Gradle Builds # [Build Script Basics](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html)



### [Projects, plugins and tasks](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html#sec:projects_and_tasks)

Every Gradle build is made up of one or more *projects*. What a project represents depends on what it is that you are doing with Gradle. For example, a project might represent a library JAR or a web application. It might represent a distribution ZIP assembled from the JARs produced by other projects. A project does not necessarily represent a thing to be built. It might represent a thing to be done, such as deploying your application to staging or production environments. Don’t worry if this seems a little vague(模糊不清) for now. Gradle’s build-by-convention support adds a more concrete definition for what a project is.

> NOTE: 
>
> 一、关于gradle的build-by-convention，参见:
>
> stackoverflow [What is build-by-convention in Gradle deep explanation?](https://stackoverflow.com/questions/21190233/what-is-build-by-convention-in-gradle-deep-explanation) # [A](https://stackoverflow.com/a/21190548)
>
> "Build by convention is the idea that if you follow the default conventions, then your builds will be much simpler. So while you *can* change the source directory, you don't need to explicitly specify the source directory. Gradle comes with logical defaults. This is also called *convention over configuration*."



The work that Gradle can do on a project is defined by one or more *tasks*. A task represents some atomic piece of work which a build performs. This might be compiling some classes, creating a JAR, generating Javadoc, or publishing some archives to a repository.

> NOTE:
>
> 一、如何为一个project添加task？后面会进行介绍

Typically, tasks are provided by [applying a plugin](https://docs.gradle.org/current/userguide/plugins.html#sec:plugins_block) so that you do not have to define them yourself. Still, to give you an idea of what a task is, we will look at defining some simple tasks in a build with one project in this chapter.

> NOTE:
>
> 一、思考: plugin如增加task？参见 [Using Gradle Plugins](https://docs.gradle.org/current/userguide/plugins.html) ，其中有如下描述:
>
> > "Plugins add new tasks (e.g. [JavaCompile](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.compile.JavaCompile.html))"
>
> 

### [Hello world](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html#sec:hello_world)

You run a Gradle build using the `gradle` command. The `gradle` command looks for a file called `build.gradle` in the current directory.[[1](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html#_footnotedef_1)] We call this `build.gradle` file a *build script*, although strictly speaking it is a build configuration script, as we will see later. The build script defines a **project** and its **tasks**.

build.gradle

```groovy
tasks.register('hello') {
    doLast {
        println 'Hello world!'
    }
}
```



Output of **`gradle -q hello`**

```shell
> gradle -q hello
Hello world!
```



### [Build scripts are code](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html#sec:build_scripts_are_code)

Gradle’s build scripts give you the full power of Groovy and Kotlin. 



### [Task dependencies](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html#sec:task_dependencies)

As you probably have guessed, you can declare tasks that depend on other tasks.

build.gradle

```groovy
tasks.register('hello') {
    doLast {
        println 'Hello world!'
    }
}
tasks.register('intro') {
    dependsOn tasks.hello
    doLast {
        println "I'm Gradle"
    }
}
```

### [Manipulating existing tasks](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html#sec:manipulating_existing_tasks) 



```groovy
4.times { counter ->
    tasks.register("task$counter") {
        doLast {
            println "I'm task number $counter"
        }
    }
}
tasks.named('task0') { dependsOn('task2', 'task3') }
```



### [Default tasks](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html#sec:default_tasks)



## Authoring Gradle Builds # [Using Gradle Plugins](https://docs.gradle.org/current/userguide/plugins.html)

Gradle at its core intentionally provides very little for real world automation. All of the useful features, like the ability to compile Java code, are added by *plugins*. Plugins add new tasks (e.g. [JavaCompile](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.compile.JavaCompile.html)), domain objects (e.g. [SourceSet](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.SourceSet.html)), conventions (e.g. Java source is located at `src/main/java`) as well as extending core objects and objects from other plugins.

> NOTE:
>
> 一、关于"Plugins add new tasks (e.g. [JavaCompile](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.compile.JavaCompile.html))"，可以参见:
>
> > ```groovy
> > plugins {
> >     id 'java'
> > }
> > 
> > tasks.withType(JavaCompile) {
> >     //enable compilation in a separate daemon process
> >     options.fork = true
> > }
> > ```
>
> 

### [What plugins do](https://docs.gradle.org/current/userguide/plugins.html#sec:what_plugins_do)

Applying a plugin to a project allows the plugin to extend the project’s capabilities. It can do things such as:



By applying plugins, rather than adding logic to the project build script, we can reap a number of benefits. Applying plugins:

1、Promotes reuse and reduces the overhead of maintaining similar logic across multiple projects

2、Allows a higher degree of modularization, enhancing comprehensibility and organization

3、Encapsulates imperative logic and allows build scripts to be as declarative as possible



### [Types of plugins](https://docs.gradle.org/current/userguide/plugins.html#sec:types_of_plugins)

There are two general types of plugins in Gradle:

1、*binary* plugins 

Binary plugins are written either programmatically by implementing [Plugin](https://docs.gradle.org/current/javadoc/org/gradle/api/Plugin.html) interface or declaratively using one of Gradle’s DSL languages. 

Binary plugins can reside within a build script, within the project hierarchy or externally in a plugin jar.

2、*script* plugins.

Script plugins are additional build scripts that further configure the build and usually implement a declarative approach to manipulating the build. They are typically used within a build although they can be externalized and accessed from a remote location.



### [Using plugins](https://docs.gradle.org/current/userguide/plugins.html#sec:using_plugins)

To use the build logic encapsulated in a plugin, Gradle needs to perform two steps. 

1、First, it needs to *resolve* the plugin

*Resolving* a plugin means finding the correct version of the jar which contains a given plugin and adding it to the **script classpath**. Once a plugin is resolved, its API can be used in a build script. **Script plugins** are self-resolving in that they are resolved from the specific file path or URL provided when applying them. Core binary plugins provided as part of the Gradle distribution are automatically resolved.

2、Then it needs to *apply* the plugin to the target, usually a [Project](https://docs.gradle.org/current/dsl/org.gradle.api.Project.html).

*Applying* a plugin means actually executing the plugin’s [Plugin.apply(T)](https://docs.gradle.org/current/javadoc/org/gradle/api/Plugin.html#apply-T-) on the **Project** you want to enhance with the plugin. Applying plugins is *idempotent*. That is, you can safely apply any plugin multiple times without side effects.

### [Binary plugins](https://docs.gradle.org/current/userguide/plugins.html#sec:binary_plugins)

You apply plugins by their *plugin id*, which is a globally unique identifier, or name, for plugins.



#### [Locations of binary plugins](https://docs.gradle.org/current/userguide/plugins.html#sec:binary_plugin_locations)

A plugin is simply any class that implements the [Plugin](https://docs.gradle.org/current/javadoc/org/gradle/api/Plugin.html) interface. 

Gradle provides the core plugins (e.g. `JavaPlugin`) as part of its distribution which means they are automatically resolved. However, non-core binary plugins need to be resolved before they can be applied. 

#### [Applying plugins with the plugins DSL](https://docs.gradle.org/current/userguide/plugins.html#sec:plugins_block)

The plugins DSL provides a succinct and convenient way to declare plugin dependencies. It works with the [Gradle plugin portal](http://plugins.gradle.org/) to provide easy access to both core and community plugins. 



To apply a core plugin, the short name can be used:

build.gradle

```groovy
plugins {
    id 'java'
}
```

To apply a community plugin from the portal, the fully qualified plugin id must be used:

build.gradle

```groovy
plugins {
    id 'com.jfrog.bintray' version '1.8.5'
}
```



#### [Limitations of the plugins DSL](https://docs.gradle.org/current/userguide/plugins.html#plugins_dsl_limitations)

This way of adding plugins to a project is much more than a more convenient syntax. The plugins DSL is processed in a way which allows Gradle to determine the plugins in use very early and very quickly. 



### [Script plugins](https://docs.gradle.org/current/userguide/plugins.html#sec:script_plugins)



```groovy
apply from: 'other.gradle'
```

Script plugins are automatically resolved and can be applied from a script on the local filesystem or at a remote location.





## Authoring Gradle Builds # [Authoring Tasks](https://docs.gradle.org/current/userguide/more_about_tasks.html)



### [Adding dependencies to a task](https://docs.gradle.org/current/userguide/more_about_tasks.html#sec:adding_dependencies_to_tasks)



### [Lifecycle tasks](https://docs.gradle.org/current/userguide/more_about_tasks.html#sec:lifecycle_tasks)

> NOTE:
>
> 一、在"[Command-Line Interface](https://docs.gradle.org/current/userguide/command_line_interface.html) # [ Executing tasks](https://docs.gradle.org/current/userguide/command_line_interface.html#sec:command_line_executing_tasks)"中有如下描述:
>
> > Most builds support a common set of tasks known as [*lifecycle tasks*](https://docs.gradle.org/current/userguide/more_about_tasks.html#sec:lifecycle_tasks). These include the `build`, `assemble`, and `check` tasks.

Lifecycle tasks are tasks that do not do work themselves. They typically do not have any task actions. Lifecycle tasks can represent several concepts:





## Working with Dependencies # [Dependency Management Terminology](https://docs.gradle.org/current/userguide/dependency_management_terminology.html) 



### [Artifact](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_artifact) 

> NOTE:
>
> 一、其实就是产物



### [Capability](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_capability)

A capability identifies a feature offered by one or multiple components. 



### [Configuration](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_configuration)



### [Module](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_module) 

A piece of software that evolves over time e.g. [Google Guava](https://github.com/google/guava). Every module has a name. Each release of a module is optimally represented by a [module version](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_module_version). For convenient consumption, modules can be hosted in a [repository](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_repository).

### [Component](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_component) 

Any single version of a [module](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_module).



### [Repository](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_repository)

A repository hosts a set of [modules](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_module), each of which may provide one or many releases (components) indicated by a [module version](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_module_version). The repository can be based on a binary repository product (e.g. Artifactory or Nexus) or a directory structure in the filesystem. For more information, see [Declaring Repositories](https://docs.gradle.org/current/userguide/declaring_repositories.html#declaring-repositories).



## Working with Dependencies # [Dependency management in Gradle](https://docs.gradle.org/current/userguide/core_dependency_management.html) 

Software projects rarely work in isolation. In most cases, a project relies on reusable functionality in the form of libraries or is broken up into individual components to compose a modularized system. Dependency management is a technique for declaring, resolving and using dependencies required by the project in an automated fashion.



## Working with Dependencies # [Declaring repositories](https://docs.gradle.org/current/userguide/declaring_repositories.html) 



```groovy
repositories {
    mavenCentral()
}
```





## Working with Dependencies # [Declaring dependencies](https://docs.gradle.org/current/userguide/declaring_dependencies.html) 



### [What are dependency configurations](https://docs.gradle.org/current/userguide/declaring_dependencies.html#sec:what-are-dependency-configurations)

> NOTE:
>
> 一、在 [Dependency Management Terminology](https://docs.gradle.org/current/userguide/dependency_management_terminology.html) # [ Configuration](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_configuration) 中对 configuration 的解释为:
>
> > A configuration is a named set of [dependencies](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_dependency) grouped together for a specific goal. Configurations provide access to the underlying, resolved [modules](https://docs.gradle.org/current/userguide/dependency_management_terminology.html#sub:terminology_module) and their artifacts. For more information, see the sections on [dependency configurations](https://docs.gradle.org/current/userguide/declaring_dependencies.html#sec:what-are-dependency-configurations) as well as [resolvable and consumable configurations](https://docs.gradle.org/current/userguide/declaring_dependencies.html#sec:resolvable-consumable-configs).
>
> 简而言之: 通过configuration来分组dependency。

Every dependency declared for a Gradle project applies to a specific scope. For example some dependencies should be used for compiling source code whereas others only need to be available at runtime. Gradle represents the scope of a dependency with the help of a [Configuration](https://docs.gradle.org/current/dsl/org.gradle.api.artifacts.Configuration.html). Every configuration can be identified by a unique name.

![dependency management configurations](https://docs.gradle.org/current/userguide/img/dependency-management-configurations.png)

Figure 1. Configurations use declared dependencies for specific purposes

#### [Configuration inheritance and composition](https://docs.gradle.org/current/userguide/declaring_dependencies.html#sub:config-inheritance-composition)

A configuration can extend other configurations to form an inheritance hierarchy. Child configurations inherit the whole set of dependencies declared for any of its superconfigurations.

Configuration inheritance is heavily used by Gradle core plugins like the [Java plugin](https://docs.gradle.org/current/userguide/java_plugin.html#sec:java_plugin_and_dependency_management). For example the `testImplementation` configuration extends the `implementation` configuration. The configuration hierarchy has a practical purpose: compiling tests requires the dependencies of the source code under test on top of the dependencies needed write the test class. A Java project that uses JUnit to write and execute test code also needs Guava if its classes are imported in the production source code.

```groovy
configurations {
    smokeTest.extendsFrom testImplementation
}

dependencies {
    testImplementation 'junit:junit:4.13'
    smokeTest 'org.apache.httpcomponents:httpclient:4.5.5'
}
```





### [Resolvable and consumable configurations](https://docs.gradle.org/current/userguide/declaring_dependencies.html#sec:resolvable-consumable-configs)



## gradle build VS gradle assemble

stackoverflow [What are the differences between gradle assemble and gradle build tasks?](https://stackoverflow.com/questions/44185165/what-are-the-differences-between-gradle-assemble-and-gradle-build-tasks) # [A](https://stackoverflow.com/a/44185464) 

> `assemble` will build your artifacts, and `build` will assemble your artifacts with additional checks.
>
> ```
> build` depends on `assemble`, so `build` is sort of a superset of `assemble
> ```
>
> You can have a look on the tasks that will be executed by using the `--dry-run` flag. e.g.
>
> ```css
> gradlew build --dry-run
> ```
>
> You will see that apart from `assemble` also `lint` and `test` will be executed.

## todo

javatpoint [Gradle Build](https://www.javatpoint.com/gradle-build)