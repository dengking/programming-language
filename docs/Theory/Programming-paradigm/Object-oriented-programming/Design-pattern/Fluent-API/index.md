# Fluent API



## wikipedia [Fluent interface](https://en.wikipedia.org/wiki/Fluent_interface)

A fluent interface is normally implemented by using [method cascading](https://infogalactic.com/info/Method_cascading) (concretely [method chaining](https://infogalactic.com/info/Method_chaining)) to **relay**(传达) the **instruction context** of a subsequent call (but a fluent interface entails more than just method chaining [[1\]](https://infogalactic.com/info/Fluent_interface#cite_note-1)). Generally, the **context** is

- defined through the return value of a called method

  > NOTE: context是通过被调用的method的返回值来定义的

- self-referential, where the new context is equivalent to the last context

  > NOTE: "self-referential"的含义是 "自引用的"，即新context和上一个context相同。

- terminated through the return of a void context.

  > NOTE: 需要标识合适终止
  
  

> NOTE: 上面这段话所着重描述的是 **context**，显然被连续调用的method，它们所共用的就是context



### History

A common example is the [iostream](https://infogalactic.com/info/Iostream) library in C++, which uses the `<<` or `>>` [operators](https://infogalactic.com/info/Operator_(computer_programming)) for the message passing, sending multiple data to the same object and allowing "manipulators" for other method calls.



### Examples

### JavaScript

There are many examples of JS libraries that use some variant of this: **jQuery** probably being the most well known. Typically **fluent builders** are used to implement 'DB queries', for example in https://github.com/Medium/dynamite :

```javascript
// getting an item from a table
client.getItem('user-table')
    .setHashKey('userId', 'userA')
    .setRangeKey('column', '@')
    .execute()
    .then(function(data) {
        // data.result: the resulting object
    })
```

> NOTE: 这是典型的asynchronous programming，现代很多programming language都是支持这种paradigm的，在后面的"Asynchronous programming"章节会进行详细描述。

A simple way to do this in javascript is using prototype inheritance and `this`.

```javascript
// example from http://schier.co/post/method-chaining-in-javascript
// define the class
var Kitten = function() {
  this.name = 'Garfield';
  this.color = 'brown';
  this.gender = 'male';
};

Kitten.prototype.setName = function(name) {
  this.name = name;
  return this;
};

Kitten.prototype.setColor = function(color) {
  this.color = color;
  return this;
};

Kitten.prototype.setGender = function(gender) {
  this.gender = gender;
  return this;
};

Kitten.prototype.save = function() {
  console.log(
    'saving ' + this.name + ', the ' +
    this.color + ' ' + this.gender + ' kitten...'
  );

  // save to database here...

  return this;
};

// use it
new Kitten()
  .setName('Bob')
  .setColor('black')
  .setGender('male')
  .save();
```

Although it's lots of clumsy code, a better alternative would be to pack these method in single function thus creating a framework.

A more general way to do this is implemented in [mu-ffsm](https://github.com/0x01/mu-ffsm).

```javascript
var mkChained = function(spec) {
  return function(init) {
    var s = spec[0] ? spec[0](init) : 0;

    var i = function(opt) {
      return spec[1] ? spec[1](s, opt) : s;
    }

    Object.keys(spec).forEach(
      function(name){
        // skip `entry` and `exit` functions
        if(/^\d+$/.test(name))
          return;

        // transition 'name : (s, opt) -> s'
        i[name] = function(opt) {
          s = spec[name](s, opt);
          return i;
        };
    });

    return i;
  }
};

var API = mkChained({
  0:    function(opt)    {return ;/* create initial state */},
  then: function(s, opt) {return s; /* new state */},
  whut: function(s, opt) {return s; /* new state */},
  1:    function(s, opt) {return ;/* compute final value */}
});

// We create an instance of our newly crafted API,
var call = API() // entry
   .whut()       // transition
   .then()       // transition
   .whut();      // transition

// And call it
var result0 = call() // exit
  , result1 = call() // exit
```

> NOTE: JavaScript这种语言是非常容易实现这种功能的

### Java

The [jOOQ](https://infogalactic.com/info/Java_Object_Oriented_Querying) library models SQL as a fluent API in Java

```java
Author a = AUTHOR.as("a");
create.selectFrom(a)
      .where(exists(selectOne()
                   .from(BOOK)
                   .where(BOOK.STATUS.eq(BOOK_STATUS.SOLD_OUT))
                   .and(BOOK.AUTHOR_ID.eq(a.ID))));
```

The [op4j](http://www.op4j.org/) library enables the use of fluent code for performing auxiliary tasks like structure iteration, data conversion, filtering, etc.

```java
String[] datesStr = new String[] {"12-10-1492", "06-12-1978"};

List<Calendar> dates = 
    Op.on(datesStr).toList().map(FnString.toCalendar("dd-MM-yyyy")).get();
```

### C++

A common use of the fluent interface in [C++](https://infogalactic.com/info/C%2B%2B) is the standard [iostream](https://infogalactic.com/info/Iostream), which chains [overloaded operators](https://infogalactic.com/info/Operator_overloading).

The following is an example of providing a **fluent interface wrapper** on top of a more traditional interface in C++:

```C++
 // Basic definition
 class GlutApp {
 private:
     int w_, h_, x_, y_, argc_, display_mode_;
     char **argv_;
     char *title_;
 public:
     GlutApp(int argc, char** argv) {
         argc_ = argc;
         argv_ = argv;
     }
     void setDisplayMode(int mode) {
         display_mode_ = mode;
     }
     int getDisplayMode() {
         return display_mode_;
     }
     void setWindowSize(int w, int h) {
         w_ = w;
         h_ = h;
     }
     void setWindowPosition(int x, int y) {
         x_ = x;
         y_ = y;
     }
     void setTitle(const char *title) {
         title_ = title;
     }
     void create(){;}
 };
 // Basic usage
 int main(int argc, char **argv) {
     GlutApp app(argc, argv);
     app.setDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_ALPHA|GLUT_DEPTH); // Set framebuffer params
     app.setWindowSize(500, 500); // Set window params
     app.setWindowPosition(200, 200);
     app.setTitle("My OpenGL/GLUT App");
     app.create();
 }

 // Fluent wrapper
 class FluentGlutApp : private GlutApp {
 public:
     FluentGlutApp(int argc, char **argv) : GlutApp(argc, argv) {} // Inherit parent constructor
     FluentGlutApp &withDoubleBuffer() {
         setDisplayMode(getDisplayMode() | GLUT_DOUBLE);
         return *this;
     }
     FluentGlutApp &withRGBA() {
         setDisplayMode(getDisplayMode() | GLUT_RGBA);
         return *this;
     }
     FluentGlutApp &withAlpha() {
         setDisplayMode(getDisplayMode() | GLUT_ALPHA);
         return *this;
     }
     FluentGlutApp &withDepth() {
         setDisplayMode(getDisplayMode() | GLUT_DEPTH);
         return *this;
     }
     FluentGlutApp &across(int w, int h) {
         setWindowSize(w, h);
         return *this;
     }
     FluentGlutApp &at(int x, int y) {
         setWindowPosition(x, y);
         return *this;
     }
     FluentGlutApp &named(const char *title) {
         setTitle(title);
         return *this;
     }
     // It doesn't make sense to chain after create(), so don't return *this
     void create() {
         GlutApp::create();
     }
 };
 // Fluent usage
 int main(int argc, char **argv) {
     FluentGlutApp(argc, argv)
         .withDoubleBuffer().withRGBA().withAlpha().withDepth()
         .at(200, 200).across(500, 500)
         .named("My OpenGL/GLUT App")
         .create();
 }
```



### Problems

#### Debugging & error reporting

> NOTE: 带来了调试的不便

#### Logging

> NOTE: 

One more issue is with adding log statements.

```C++
ByteBuffer buffer = ByteBuffer.allocate(10).rewind().limit(100);
```

E.g. to log the state of `buffer` after `rewind(`) method call, it is necessary to break the fluent calls:

```C++
ByteBuffer buffer = ByteBuffer.allocate(10).rewind();
log.debug("First byte after rewind is " + buffer.get(0));
buffer.limit(100);
```



#### Subclasses



## 用法总结

下面是对fluent API的用法总结: 

### Builder pattern

Bluent API的一个重要用法就是实现builder pattern。

[What is the difference between a fluent interface and the Builder pattern?](https://stackoverflow.com/questions/17937755/what-is-the-difference-between-a-fluent-interface-and-the-builder-pattern)

#### C++ Named Parameter Idiom

参见: wikibooks [Named Parameter Idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Named_Parameter)

#### [httpcomponents#HttpClient](http://hc.apache.org/httpcomponents-client-ga/) 

[Fluent API](http://hc.apache.org/httpcomponents-client-ga/tutorial/html/fluent.html)

```C++
// Execute a GET with timeout settings and return response content as String.
Request.Get("http://somehost/")
        .connectTimeout(1000)
        .socketTimeout(1000)
        .execute().returnContent().asString();
```

### Asynchronous programming

当进行**asynchronous programming**的时候，往往需要表达"当 某个**event** 发生时，执行 某个 **callback**"，这个**callback**往往是用户注册的自定义函数，这种是可以使用fluent API来进行描述的，它其实非常类似于builder pattern，即由用户来设置**callback**，最最典型的就是jQuery，现代很多programming language都是支持这种paradigm的。

#### jQuery

在前面的JavaScript章节中给出了非常好的例子。



#### Apache Curator



## Fluent API and pipeline

Fluent API 和 [Unix pipeline](https://infogalactic.com/info/Pipeline_(Unix)) 是有些类似的，它们都是chain。