# [What is Virtual DOM?](https://stackoverflow.com/questions/21965738/what-is-virtual-dom)

Recently, I looked at Facebook's [React](https://facebook.github.io/react/) framework. It uses a concept called "the Virtual DOM," which I didn't really understand.

What is the Virtual DOM? What are the advantages?

## [A](https://stackoverflow.com/a/21965987)

React creates a tree of custom objects representing a part of the **DOM**. For example, instead of creating an actual DIV element containing a UL element, it creates a React.div object that contains a React.ul object. It can manipulate these objects very quickly without actually touching the real DOM or going through the DOM API. Then, when it renders a component, it uses this **virtual DOM** to figure out what it needs to do with the **real DOM** to get the two trees to match.

You can think of the **virtual DOM** like a **blueprint**. It contains all the details needed to construct the DOM, but because it doesn't require all the heavyweight parts that go into a real DOM, it can be created and changed much more easily.

