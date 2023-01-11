# Consumer

## geeksforgeeks [Java 8 | Consumer Interface in Java with Examples](https://www.geeksforgeeks.org/java-8-consumer-interface-in-java-with-examples/)



```java
// Java Program to demonstrate
// Consumer's accept() method

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.function.Consumer;

public class Main {
	public static void main(String args[])
	{
		// Consumer to display a number
		Consumer<Integer> display = a -> System.out.println(a);

		// Implement display using accept()
		display.accept(10);

		// Consumer to multiply 2 to every integer of a list
		Consumer<List<Integer> > modify = list ->
		{
			for (int i = 0; i < list.size(); i++)
				list.set(i, 2 * list.get(i));
		};

		// Consumer to display a list of numbers
		Consumer<List<Integer> >
			dispList = list -> list.stream().forEach(a -> System.out.print(a + " "));

		List<Integer> list = new ArrayList<Integer>();
		list.add(2);
		list.add(1);
		list.add(3);

		// Implement modify using accept()
		modify.accept(list);

		// Implement dispList using accept()
		dispList.accept(list);
	}
}

```



```shell
javac Main.java
java Main
```

