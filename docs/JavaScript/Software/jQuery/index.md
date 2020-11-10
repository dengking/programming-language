# jQuery

jQuery是最最能够体现JavaScript特性的library，它就像spring之于Java。





## Wikipedia [jQuery](https://infogalactic.com/info/JQuery)

> NOTE: 这篇文章对jQuery的总结是非常好的，其中所总结的jQuery的设计思想是非常值得学习的。

jQuery's **syntax** is designed to make it easier to navigate a document, select [DOM](https://infogalactic.com/info/Document_Object_Model) elements, create [animations](https://infogalactic.com/info/Animation), handle [events](https://infogalactic.com/info/Event_(computing)), and develop [Ajax](https://infogalactic.com/info/Ajax_(programming)) applications. 

> NOTE: jQuery在这方面是非常成功的:
>
> 1) 它的Ajax programming model可以作为asynchronous的典范，很多其他高级programming language的API都借鉴了Ajax的programming model。



The set of [jQuery core features](https://infogalactic.com/info/JQuery#Features)—DOM element selections, traversal and manipulation—enabled by its *selector engine* (named "Sizzle" from v1.3), created a new "programming style", fusing(融合) algorithms and DOM data structures. 

> NOTE: 在前面我们已经介绍了DOM，通过上述内容，我们需要总结的是: algorithm + DS，各种高级programming language的standard library都提供了algorithm + DS。

### Usage

#### Chaining

Each jQuery command returns a jQuery object, so commands can be chained:

```javascript
$("div.test").add("p.quote").addClass("blue").slideDown("slow");
```

This line finds the union of all `div` tags with class attribute `test` and all `p` tags with class attribute `quote`, adds the class attribute `blue` to each matched element, and then increases their height with an animation. The `$` and `add` functions affect the matched set, while the `addClass` and `slideDown` affect the referenced nodes.

> NOTE: jQuery的这种风格类似于fluent API，参见`Theory\Programming-paradigm\Object-oriented-programming\Design-pattern\Fluent-API`章节

#### AJAX

It is possible to perform browser-independent [Ajax](https://infogalactic.com/info/Ajax_(programming)) queries using `$.ajax` and associated methods to load and manipulate remote data.

```C++
$.ajax({
  type: "POST",
  url: "example.php",
  data: "name=John&location=Boston"
}).done( function(msg) {
  alert( "Data Saved: " + msg );
}).fail( function( xmlHttpRequest, statusText, errorThrown ) {
  alert(
    "Your form submission failed.\n\n"
      + "XML Http Request: " + JSON.stringify( xmlHttpRequest )
      + ",\nStatus Text: " + statusText
      + ",\nError Thrown: " + errorThrown );
});
```

This example posts the data `name=John` and `location=Boston` to `example.php` on the server. When this request finishes the success function is called to alert the user. If the request fails it will alert the user to the failure, the status of the request, and the specific error.

##### Asynchronous

Note that the above example uses the deferred nature of `$.ajax()` to handle its asynchronous nature: `.done()` and `.fail()` create callbacks that run only when the asynchronous process is complete.