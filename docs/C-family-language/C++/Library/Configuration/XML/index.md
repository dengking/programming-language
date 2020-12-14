# XML

## stackoverflow [What XML parser should I use in C++? [closed]](https://stackoverflow.com/questions/9387610/what-xml-parser-should-i-use-in-c)



Just like with standard library containers, what library you should use depends on your needs. Here's a convenient flowchart:

![enter image description here](https://i.stack.imgur.com/hUjpw.png)

So the first question is this: **What do you need?**

### I Need Full XML Compliance

OK, so you need to process XML. Not toy XML, *real* XML. You need to be able to read and write *all* of the XML specification, not just the low-lying, easy-to-parse bits. You need Namespaces, DocTypes, entity substitution, the works. The W3C XML Specification, in its entirety.

The next question is: **Does your API need to conform to DOM or SAX?**

#### I Need Exact DOM and/or SAX Conformance

OK, so you really need the API to be DOM and/or SAX. It can't just be a SAX-style push parser, or a DOM-style retained parser. It *must* be the actual DOM or the actual SAX, to the extent that C++ allows.

You have chosen:

**[Xerces](http://xerces.apache.org/)**

That's your choice. It's pretty much the only C++ XML parser/writer that has full (or as near as C++ allows) DOM and SAX conformance. It also has XInclude support, XML Schema support, and a plethora of other features.

It has no real dependencies. It uses the Apache license.

#### I Don't Care About DOM and/or SAX Conformance

You have chosen:

**[LibXML2](http://xmlsoft.org/)**

LibXML2 offers a C-style interface (if that really bothers you, go use Xerces), though the interface is at least somewhat object-based and easily wrapped. It provides a lot of features, like XInclude support (with callbacks so that you can tell it where it gets the file from), an XPath 1.0 recognizer, RelaxNG and Schematron support (though the error messages leave a *lot* to be desired), and so forth.

It does have a dependency on iconv, but it can be configured without that dependency. Though that does mean that you'll have a more limited set of possible text encodings it can parse.

It uses the MIT license.

### I Do Not Need Full XML Compliance

OK, so full XML compliance doesn't matter to you. Your XML documents are either fully under your control or are guaranteed to use the "basic subset" of XML: no namespaces, entities, etc.

So what does matter to you? The next question is: **What is the most important thing to you in your XML work?**

#### Maximum XML Parsing Performance

Your application needs to take XML and turn it into C++ datastructures as fast as this conversion can possibly happen.

You have chosen:

**[RapidXML](http://rapidxml.sourceforge.net/)**

This XML parser is exactly what it says on the tin: rapid XML. It doesn't even deal with pulling the file into memory; how that happens is up to you. What it does deal with is parsing that into a series of C++ data structures that you can access. And it does this about as fast as it takes to scan the file byte by byte.

Of course, there's no such thing as a free lunch. Like most XML parsers that don't care about the XML specification, Rapid XML doesn't touch namespaces, DocTypes, entities (with the exception of character entities and the 6 basic XML ones), and so forth. So basically nodes, elements, attributes, and such.

Also, it is a DOM-style parser. So it does require that you read all of the text in. However, what it doesn't do is *copy* any of that text (usually). The way RapidXML gets most of its speed is by refering to strings *in-place*. This requires more memory management on your part (you must keep that string alive while RapidXML is looking at it).

RapidXML's DOM is bare-bones. You can get string values for things. You can search for attributes by name. That's about it. There are no convenience functions to turn attributes into other values (numbers, dates, etc). You just get strings.

One other downside with RapidXML is that it is painful for *writing* XML. It requires you to do a lot of explicit memory allocation of string names in order to build its DOM. It does provide a kind of string buffer, but that still requires a lot of explicit work on your end. It's certainly functional, but it's a pain to use.

It uses the MIT licence. It is a header-only library with no dependencies.

- There is a [RapidXML "GitHub patch"](https://github.com/dwd/rapidxml) that allows it to also work with namespaces.

##### I Care About Performance But Not Quite That Much

Yes, performance matters to you. But maybe you need something a bit less bare-bones. Maybe something that can handle more Unicode, or doesn't require so much user-controlled memory management. Performance is still important, but you want something a little less direct.

You have chosen:

**[PugiXML](http://pugixml.org/)**

Historically, this served as inspiration for RapidXML. But the two projects have diverged, with Pugi offering more features, while RapidXML is focused entirely on speed.

PugiXML offers Unicode conversion support, so if you have some UTF-16 docs around and want to read them as UTF-8, Pugi will provide. It even has an XPath 1.0 implementation, if you need that sort of thing.

But Pugi is still quite fast. Like RapidXML, it has no dependencies and is distributed under the MIT License.

#### Reading Huge Documents

You need to read documents that are measured in the *gigabytes* in size. Maybe you're getting them from stdin, being fed by some other process. Or you're reading them from massive files. Or whatever. The point is, what you need is to *not* have to read the entire file into memory all at once in order to process it.

You have chosen:

**LibXML2**

Xerces's SAX-style API will work in this capacity, but LibXML2 is here because it's a bit easier to work with. A SAX-style API is a push-API: it starts parsing a stream and just fires off events that you have to catch. You are forced to manage context, state, and so forth. Code that reads a SAX-style API is a lot more spread out than one might hope.

LibXML2's `xmlReader` object is a pull-API. You *ask* to go to the next XML node or element; you aren't told. This allows you to store context as you see fit, to handle different entities in a way that's much more readable in code than a bunch of callbacks.

##### Alternatives

**[Expat](https://libexpat.github.io/)**

Expat is a well-known C++ parser that uses a pull-parser API. It was written by James Clark.

It's current status is active. The most recent version is 2.2.9, which was released on (2019-09-25).

**[LlamaXML](http://llamaxml.berlios.de/)**

It is an implementation of an StAX-style API. It is a pull-parser, similar to LibXML2's `xmlReader` parser.

But it hasn't been updated since 2005. So again, Caveat Emptor.

#### XPath Support

XPath is a system for querying elements within an XML tree. It's a handy way of effectively naming an element or collection of element by common properties, using a standardized syntax. Many XML libraries offer XPath support.

There are effectively three choices here:

- **LibXML2**: It provides full XPath 1.0 support. Again, it is a C API, so if that bothers you, there are alternatives.
- **PugiXML**: It comes with XPath 1.0 support as well. As above, it's more of a C++ API than LibXML2, so you may be more comfortable with it.
- **TinyXML**: It does not come with XPath support, but there is the **[TinyXPath](http://tinyxpath.sourceforge.net/)** library that provides it. TinyXML is undergoing a conversion to version 2.0, which significantly changes the API, so TinyXPath may not work with the new API. Like TinyXML itself, TinyXPath is distributed under the zLib license.

#### Just Get The Job Done

So, you don't care about XML correctness. Performance isn't an issue for you. Streaming is irrelevant. All you want is *something* that gets XML into memory and allows you to stick it back onto disk again. What *you* care about is API.

You want an XML parser that's going to be small, easy to install, trivial to use, and small enough to be irrelevant to your eventual executable's size.

You have chosen:

**[TinyXML](http://www.grinninglizard.com/tinyxml/)**

I put TinyXML in this slot because it is about as braindead simple to use as XML parsers get. Yes, it's slow, but it's simple and obvious. It has a lot of convenience functions for converting attributes and so forth.

Writing XML is no problem in TinyXML. You just `new` up some objects, attach them together, send the document to a `std::ostream`, and everyone's happy.

There is also something of an ecosystem built around TinyXML, with a more iterator-friendly API, and even an XPath 1.0 implementation layered on top of it.

TinyXML uses the zLib license, which is more or less the MIT License with a different name.



## [pugixml](https://pugixml.org/)