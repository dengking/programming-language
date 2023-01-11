# Map



## iteration

java apply function to every pair of hashmap:

```java
map.forEach((key, val) -> {
            

        });
```



stackoverflow [How do I efficiently iterate over each entry in a Java Map?](https://stackoverflow.com/questions/46898/how-do-i-efficiently-iterate-over-each-entry-in-a-java-map)

stackoverflow [How to for each the hashmap? [duplicate]](https://stackoverflow.com/questions/4234985/how-to-for-each-the-hashmap)

stackoverflow [Iterate through a HashMap [duplicate]](https://stackoverflow.com/questions/1066589/iterate-through-a-hashmap)



## stackoverflow [Check in Map of list if exists value for specific key Java 8](https://stackoverflow.com/questions/50717771/check-in-map-of-list-if-exists-value-for-specific-key-java-8)



[A](https://stackoverflow.com/a/50717977)

Do the following:

```java
boolean result = m.getOrDefault("Name2", Collections.emptyList()).stream()
    .anyMatch(i -> i.contains("@"));
```

If the `Map` contains a correct key, check whether any of its element of the `List` as value contains the particular character. If the `Map` doesn’t contain the key, mock the empty `Collection` which doesn't contain anything at all and the result is evaluated automatically as `false`.

Edit: As @Michael suggested, using the [`Collections.emptyList()`](https://docs.oracle.com/javase/8/docs/api/java/util/Collections.html#emptyList--) is a better choice than `new ArrayList<>()`.



[A](https://stackoverflow.com/a/50718182)

You can simply use `m.get("Name2")`, place the (nullable) result into an `Optional` and then use a mapping:

```java
boolean result = Optional.ofNullable(m.get("Name2"))
    .map(l -> l.stream().anyMatch(s -> s.contains("@")))
    .orElse(false);
```

This is preferable to looping over the entry set, as `HashMap.get` is O(1) and iterating over the entry set is O(n).



## techiedelight [Check if a value exists in a Map in Java](https://www.techiedelight.com/check-value-exists-map-java/)

### 1. Using `containsValue()` method

```JAVA
import java.util.HashMap;
import java.util.Map;
 
public class Main
{
    public static void main(String[] args)
    {
        Map<Character, Integer> hashMap = new HashMap<>();
        for (int i = 65; i <= 90; i++) {
            hashMap.put((char) i, i);
        }
 
        int value = 70;
        boolean isExists = hashMap.containsValue(value);
 
        System.out.println(isExists);        // true
    }
}
```

### 2. Using `anyMatch()` method

```JAVA
import java.util.HashMap;
import java.util.Map;
 
public class Main
{
    public static void main(String[] args)
    {
        Map<Character, Integer> hashMap = new HashMap<>();
        for (int i = 65; i <= 90; i++) {
            hashMap.put((char) i, i);
        }
 
        int value = 70;
        boolean isExists = hashMap.values().stream().anyMatch(v -> v == value);
 
        System.out.println(isExists);        // true
    }
}
```

### 3. Using for loop

```JAVA
import java.util.HashMap;
import java.util.Map;
 
public class Main
{
    public static void main(String[] args)
    {
        Map<Character, Integer> hashMap = new HashMap<>();
        for (int i = 65; i <= 90; i++) {
            hashMap.put((char) i, i);
        }
 
        int value = 70;
        boolean isExists = false;
        for (Integer v: hashMap.values()) {
            if (v == value) {
                isExists = true;
                break;
            }
        }
 
        System.out.println(isExists);        // true
    }
}
```



## baeldung [How to Check If a Key Exists in a Map](https://www.baeldung.com/java-map-key-exists)

### 2. *containsKey*



```JAVA
@Test
public void whenKeyIsPresent_thenContainsKeyReturnsTrue() {
    Map<String, String> map = Collections.singletonMap("key", "value");
    
    assertTrue(map.containsKey("key"));
    assertFalse(map.containsKey("missing"));
}
```



### 3. *get*

```JAVA
@Test
public void whenKeyHasNullValue_thenGetStillWorks() {
    Map<String, String> map = Collections.singletonMap("nothing", null);

    assertTrue(map.containsKey("nothing"));
    assertNull(map.get("nothing"));
}
```



## stackoverflow [How can I check if a hashmap contains a key of a custom class type?](https://stackoverflow.com/questions/70230358/how-can-i-check-if-a-hashmap-contains-a-key-of-a-custom-class-type)



[A](https://stackoverflow.com/a/70230385)

You are checking if the `Map` contains a `key` of a specific instance of class `B` (not the class itself).

If you need to check if the `Map` contains a key of class `B` you can do:

```java
boolean hasClass = map.keySet().stream().anyMatch(B.class::isInstance);
```

[A](https://stackoverflow.com/a/70230386)

You are creating a new instance of the B() object and that new instance is not in the HashMap. Try changing the code to

```java
HashMap<A, Integer> map = new HashMap<A, Integer>();
B keyToBeTested = new B();
map.put(keyToBeTested, 1);

if (map.containsKey(keyToBeTested) {
    System.out.println("Success!");
}
```