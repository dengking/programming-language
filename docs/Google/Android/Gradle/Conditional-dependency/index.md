# gradle conditional dependency

Google: gradle conditional dependency

## stackoverflow [conditional dependencies in gradle](https://stackoverflow.com/questions/51153878/conditional-dependencies-in-gradle)



[A](https://stackoverflow.com/a/51154365/10173843)

```java
plugins {
    id "java"
}

repositories {
    jcenter()
}

dependencies {
    if (project.hasProperty("gson")) {
        implementation "com.google.gson:gson:2.8.5"
    } else {
        implementation "org.fasterxml.jackson.core:jackson-core:2.9.0"
    }
}
```

> NOTE: 
>
> 结果测试，上述方法可行

## 素材

discuss.gradle [How to filter by property and add conditional dependency in a multi-project Gradle project?](https://discuss.gradle.org/t/how-to-filter-by-property-and-add-conditional-dependency-in-a-multi-project-gradle-project/32305)