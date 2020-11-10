# [Introduction to the DOM](https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Introduction)




This section provides a brief conceptual introduction to the [DOM](https://developer.mozilla.org/en-US/docs/Glossary/DOM): what it is, how it provides structure for [HTML](https://developer.mozilla.org/en-US/docs/Glossary/HTML) and [XML](https://developer.mozilla.org/en-US/docs/Glossary/XML) documents, how you can access it, and how this API presents the reference information and examples.



## [What is the DOM?](https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Introduction#What_is_the_DOM)

The Document Object Model (DOM) is a programming interface for HTML and XML documents. It represents the page so that programs can change the document **structure**, **style**, and **content**. The DOM represents the document as nodes and objects. That way, programming languages can connect to the page.

A Web page is a **document**. This document can be either displayed in the browser window or as the HTML source. But it is the same document in both cases. The **Document Object Model** (DOM) represents that same **document** so it can be manipulated. The DOM is an **object-oriented representation** of the web page, which can be modified with a scripting language such as JavaScript.

> NOTE: 看了DOM，联想到了[ORM](https://en.wikipedia.org/wiki/Object-relational_mapping), ODM

The [W3C DOM](http://www.w3.org/DOM/) and [WHATWG DOM](https://dom.spec.whatwg.org/) standards are implemented in most modern browsers. Many browsers extend the standard, so care must be exercised when using them on the web where documents may be accessed by various browsers with different DOMs.

For example, the standard **DOM** specifies that the `getElementsByTagName` method in the code below must return a list of all the `<P>` elements in the document:

```js
var paragraphs = document.getElementsByTagName("P");
// paragraphs[0] is the first <p> element
// paragraphs[1] is the second <p> element, etc.
alert(paragraphs[0].nodeName);
```

All of the properties, methods, and events available for manipulating and creating web pages are organized into objects (e.g., the `document` object that represents the document itself, the `table` object that implements the special `HTMLTableElement` DOM interface for accessing HTML tables, and so forth). This documentation provides an object-by-object reference to the DOM implemented in Gecko-based browsers.



## [DOM and JavaScript](https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Introduction#DOM_and_JavaScript)



The short example above, like nearly all of the examples in this reference, is [JavaScript](https://developer.mozilla.org/en-US/docs/Glossary/JavaScript). That is to say, it's *written* in JavaScript, but it *uses* the **DOM** to access the document and its elements. The **DOM** is not a programming language, but without it, the **JavaScript language** wouldn't have any model or notion of web pages, HTML documents, XML documents, and their component parts (e.g. elements). Every element in a document—the document as a whole, the head, tables within the document, table headers, text within the table cells—is part of the document object model for that document, so they can all be accessed and manipulated using the **DOM** and a scripting language like JavaScript.

In the beginning, JavaScript and the DOM were tightly intertwined, but eventually, they evolved into separate entities. The page content is stored in the **DOM** and may be accessed and manipulated via JavaScript, so that we may write this approximative equation:

API (HTML or XML page) = DOM + JS (scripting language)

The DOM was designed to be independent of any particular programming language, making the structural representation of the document available from a single, consistent API. Though we focus exclusively on JavaScript in this reference documentation, implementations of the DOM can be built for any language, as this Python example demonstrates:

```python
# Python DOM example
import xml.dom.minidom as m
doc = m.parse(r"C:\Projects\Py\chap1.xml")
doc.nodeName # DOM property of document object
p_list = doc.getElementsByTagName("para")
```

For more information on what technologies are involved in writing JavaScript on the web, see [JavaScript technologies overview](https://developer.mozilla.org/en-US/docs/Web/JavaScript/JavaScript_technologies_overview).



## [How Do I Access the DOM?](https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Introduction#How_Do_I_Access_the_DOM.3F)



You don't have to do anything special to begin using the DOM. Different browsers have different implementations of the DOM, and these implementations exhibit（展示） varying degrees of conformance to the actual **DOM** standard (a subject we try to avoid in this documentation), but every web browser uses some **document object model** to make web pages accessible via JavaScript.

When you create a script–whether it's **inline** in a `<script>` element or included in the web page by means of a script loading instruction–you can immediately begin using the API for the [`document`](https://developer.mozilla.org/en-US/docs/Web/API/Document) or [`window`](https://developer.mozilla.org/en-US/docs/Web/API/Window) elements to manipulate the document itself or to get at the children of that document, which are the various elements in the web page. Your DOM programming may be something as simple as the following, which displays an alert message by using the [`alert()`](https://developer.mozilla.org/en-US/docs/Web/API/Window/alert) function from the [`window`](https://developer.mozilla.org/en-US/docs/Web/API/Window) object, or it may use more sophisticated DOM methods to actually create new content, as in the longer example below.

Another example. This function creates a new H1 element, adds text to that element, and then adds the `H1` to the tree for this document:

```html
<html>
  <head>
    <script>
       // run this function when the document is loaded
       window.onload = function() {

         // create a couple of elements in an otherwise empty HTML page
         var heading = document.createElement("h1");
         var heading_text = document.createTextNode("Big Head!");
         heading.appendChild(heading_text);
         document.body.appendChild(heading);
      }
    </script>
  </head>
  <body>
  </body>
</html>
```



## [Important Data Types](https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Introduction#Important_Data_Types)

This reference tries to describe the various objects and types in simple terms. But there are a number of different data types being passed around the API that you should be aware of. For the sake of simplicity, syntax examples in this API reference typically refer to nodes as `element`s, to arrays of nodes as `nodeList`s (or simply `element`s), and to `attribute` nodes simply as `attribute`s.



The following table briefly describes these data types.
|                |                                                              |
| -------------- | ------------------------------------------------------------ |
| `Document`     | When a member returns an object of type `document` (e.g., the **ownerDocument** property of an element returns the `document`to which it belongs), this object is the root `document` object itself. The [DOM `document` Reference](https://developer.mozilla.org/en-US/docs/Web/API/Document) chapter describes the `document` object. |
| `Element`      | `element` refers to an element or a node of type `element` returned by a member of the DOM API. Rather than saying, for example, that the [`document.createElement()`](https://developer.mozilla.org/en-US/docs/Web/API/Document/createElement) method returns an object reference to a `node`, we just say that this method returns the `element` that has just been created in the DOM. `element` objects implement the DOM `Element` interface and also the more basic `Node` interface, both of which are included together in this reference. |
| `NodeList`     | A `nodeList` is an array of elements, like the kind that is returned by the method [`document.getElementsByTagName()`](https://developer.mozilla.org/en-US/docs/Web/API/Document/getElementsByTagName). Items in a `nodeList` are accessed by index in either of two ways:list.item(1)list[1]These two are equivalent. In the first, **item()** is the single method on the `nodeList` object. The latter uses the typical array syntax to fetch the second item in the list. |
| `Attribute`    | When an `attribute` is returned by a member (e.g., by the **createAttribute()** method), it is an object reference that exposes a special (albeit small) interface for attributes. Attributes are nodes in the DOM just like elements are, though you may rarely use them as such. |
| `NamedNodeMap` | A `namedNodeMap` is like an array, but the items are accessed by name or index, though this latter case is merely a convenience for enumeration, as they are in no particular order in the list. A `namedNodeMap` has an `item()` method for this purpose, and you can also add and remove items from a `namedNodeMap`. |



## [DOM interfaces](https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Introduction#DOM_interfaces)

This guide is about the **objects** and the actual *things* you can use to manipulate the DOM hierarchy. There are many points where understanding how these work can be confusing. For example, the object representing the HTML `form` element gets its **name** property from the `HTMLFormElement` interface but its **className** property from the `HTMLElement` interface. In both cases, the **property** you want is simply in that form object.

But the relationship between objects and the interfaces that they implement in the DOM can be confusing, and so this section attempts to say a little something about the actual interfaces in the **DOM specification** and how they are made available.



### [Interfaces and Objects](https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Introduction#Interfaces_and_Objects)

Many objects borrow from several different interfaces. The table object, for example, implements a specialized [`HTMLTableElement`](https://developer.mozilla.org/en-US/docs/Web/API/HTMLTableElement) interface, which includes such methods as `createCaption` and `insertRow`. But since it's also an HTML element, `table` implements the `Element` interface described in the DOM [`Element`](https://developer.mozilla.org/en-US/docs/Web/API/Element) Reference chapter. And finally, since an HTML element is also, as far as the DOM is concerned, a node in the tree of nodes that make up the object model for an HTML or XML page, the table object also implements the more basic `Node` interface, from which `Element` derives.

When you get a reference to a `table` object, as in the following example, you routinely use all three of these interfaces interchangeably on the object, perhaps without knowing it.

```js
var table = document.getElementById("table");
var tableAttrs = table.attributes; // Node/Element interface
for (var i = 0; i < tableAttrs.length; i++) {
  // HTMLTableElement interface: border attribute
  if(tableAttrs[i].nodeName.toLowerCase() == "border")
    table.border = "1";
}
// HTMLTableElement interface: summary attribute
table.summary = "note: increased border";
```





### [Core Interfaces in the DOM](https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Introduction#Core_Interfaces_in_the_DOM)

This section lists some of the most commonly-used interfaces in the DOM. The idea is not to describe what these APIs do here but to give you an idea of the sorts of methods and properties you will see very often as you use the DOM. These common APIs are used in the longer examples in the [DOM Examples](https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Examples) chapter at the end of this book.

`Document` and `window` objects are the objects whose interfaces you generally use most often in DOM programming. In simple terms, the `window` object represents something like the browser, and the `document` object is the root of the document itself. `Element` inherits from the generic `Node` interface, and together these two interfaces provide many of the methods and properties you use on individual elements. These elements may also have specific interfaces for dealing with the kind of data those elements hold, as in the `table` object example in the previous section.

The following is a brief list of common APIs in web and XML page scripting using the DOM.



- `document.getElementById(id)`
- `document.getElementsByTagName(name)`
- `document.createElement(name)`
- `parentNode.appendChild(node)`
- `element.innerHTML`
- `element.style.left`
- `element.setAttribute()`
- `element.getAttribute()`
- `element.addEventListener()`
- `window.content`
- `window.onload`
- `console.log()`
- `window.scrollTo()`