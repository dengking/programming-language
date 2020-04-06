# Name look up

前面，我们已经讨论了organization的问题，知道了program的organization的重要性，我们往往是按照此对我们的project进行组织，我们的program是需要经过compile后，才能够被执行的，我们需要知道的一个非常重要的事实是：compiler的编译单位是source file（也就是我们在上篇中提及的file unit），即compiler一次只编译一个source file，在c++中，这被称为translation unit。compiler在编译一个source file的时候，需要知道它所碰到的每个name到底是什么，即找到这个name的declaration或者definition，这就是[Name binding](https://en.wikipedia.org/wiki/Name_binding)，这个查找的过程就是所谓的name look up。所以在各种programming language中，我们需要在source file中，说明清楚每个name的信息，以供compiler能够将它bind到正确的entity。

## Scope

前面，我们已经讨论了organization的问题，知道了program的organization的重要性，我们往往是按照此对我们的project进行组织，我们的program是需要经过compile后，才能够被执行的，我们需要知道的一个非常重要的事实是：compiler的编译单位是source file（也就是我们在上篇中提及的file unit），即compiler一次只编译一个source file，在c++中，这被称为translation unit。

在各种program language中，都使用了scope的概念，需要强调的是， **最顶级**scope是file scope，即每个source file就是一个独立的scope，没有包含它的scope了，它被称为global scope，这个global scope中，包含了次一级别的scope，比如class scope、function scope。为什么是这样的呢？其实原因在上一段中已经已经说明清楚了：因为compiler一次只处理一个source file，它应该会为每个source file建立一个scope。

### scope的nesting关系

scope是典型的具备nesting关系的，关于nesting关系，参见工程[data-structure](https://dengking.github.io/data-structure/)。



每种programming language都规定了自己的scope规则。关于scope的更加详细的介绍，参见维基百科[Scope](https://en.wikipedia.org/wiki/Scope_(computer_science)) 。



## Using name and name look up

在定义了一个name后，我们就可以使用它了，这就是本节标题using name的含义。compiler在进行compile的时候，就需要找到这个name的definition，这个过程就是name look up。

## Using name within its scope

在一个name的scope中，引用它的时候，是可以直接使用的，大多数programming language都使用这种规则。

在c++中，将此称为[Unqualified name lookup](https://en.cppreference.com/w/cpp/language/unqualified_lookup)。

## Using name in other namespace/file unit

我们已经知道了，我们的project会包含多个source file，那么就出现了一种非常普遍的情况是：在某个source file中，需要引用另外一个source file中定义的name，那么此时就需要向compiler描述清楚这个来自另外一个source file的name的位置信息，以便compiler能够找到它，否则，你的program就无法编译通过。

显然要引用一个name，必须要提供这个name的位置信息，根据name的位置信息是否依赖于上下文，可以将name分为[Fully qualified name](https://en.wikipedia.org/wiki/Fully_qualified_name)和unqualified name；在一个organization中，当遇到一个name的时候，就需要找到这个是在什么地方声明或定义了这个name，这就引发了[name lookup](https://en.cppreference.com/w/cpp/language/lookup)；



### Example

刚刚看到了这段python代码，这让我想到了c++中命名空间的使用，现可以进行一下对比：

```python
from numpy import *
```

这段代码对应的c++写法就是

```c++
using namespace std;
```

需要注意的是，无论是在python中，还是在`c++`中，这两种写法都是不推荐的。关于为什么不推荐，需要了解了解；需要把握的一个认知是：`c++`是编译型语言，而python是解释型语言，两者是存在中非常大的差异的。



在python中，推荐的写法是：

```python
import numpy as np
np.array(2)
```

在c++中，推荐的写法是：

```c++
#include<map>
std::map<int, int> my_map
```

显然，可以看出来，无论是c++还是python，这两种推荐的写法都是类似的。



### Fully qualified name


#### 维基百科[Fully qualified name](https://en.wikipedia.org/wiki/Fully_qualified_name) 

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), a **fully qualified name** is an **unambiguous** name that specifies which [object](https://en.wikipedia.org/wiki/Object_(computer_science)), [function](https://en.wikipedia.org/wiki/Function_(computer_science)), or [variable](https://en.wikipedia.org/wiki/Variable_(programming)) a call refers to without regard to the context of the call(**fully qualified name**是一个明确的name，它指定调用引用的对象，函数或变量，而不考虑调用的上下文。). In a [hierarchical structure](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)), a name is fully qualified when it "is complete in the sense that it includes

- (a) all names in the hierarchic sequence above the given element and 
- (b) the name of the given element itself." 

总结：切莫以分割的思路来理解这个概念，而是以整体性的视角来看到这个问题。结合下面的例子可知，其实理解这个概念的关键点在于下：

-  在计算机科学中，是广泛采用[hierarchical structure](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming))的，无论是文件系统的路径（显然相对路径就是context-related，而绝对路径就是context-free的） ，还是一些programming language，皆采用了这种组织方式。

##### Programming 

Fully qualified names explicitly refer to [namespaces](https://en.wikipedia.org/wiki/Namespace) that would otherwise be implicit because of the [scope](https://en.wikipedia.org/wiki/Scope_(programming)) of the call.[[2\]](https://en.wikipedia.org/wiki/Fully_qualified_name#cite_note-HejlsbergTorgersen2010-2) While always done to eliminate [ambiguity](https://en.wikipedia.org/wiki/Ambiguity), this can mean different things dependent on context.

Commonly encountered applications of the notion(概念) have been given their own names, such as the [fully qualified domain name](https://en.wikipedia.org/wiki/Fully_qualified_domain_name) and the [fully qualified file name](https://en.wikipedia.org/wiki/Fully_qualified_file_name).

###### Examples 

To distinguish a **fully qualified name** from a regular name, [C++](https://en.wikipedia.org/wiki/C%2B%2B), [Tcl](https://en.wikipedia.org/wiki/Tcl), [Perl](https://en.wikipedia.org/wiki/Perl) and [Ruby](https://en.wikipedia.org/wiki/Ruby_(programming_language)) use two colons (`::`), and [Java](https://en.wikipedia.org/wiki/Java_(programming_language)) uses dots (`.`), as does Visual Basic .NET.[[3\]](https://en.wikipedia.org/wiki/Fully_qualified_name#cite_note-3) and [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)).[[4\]](https://en.wikipedia.org/wiki/Fully_qualified_name#cite_note-4) In Java, [ActionScript](https://en.wikipedia.org/wiki/ActionScript),[[5\]](https://en.wikipedia.org/wiki/Fully_qualified_name#cite_note-5) and other [object-oriented languages](https://en.wikipedia.org/wiki/Object-oriented_language)the use of the dot is known as "dot syntax".[[6\]](https://en.wikipedia.org/wiki/Fully_qualified_name#cite_note-6) Other examples include:

- As an example of a relational database, in [Microsoft SQL Server](https://en.wikipedia.org/wiki/Microsoft_SQL_Server) the fully qualified name of an object is the one that specifies all four parts: `server_name.[database_name].[schema_name].object_name`.[[7\]](https://en.wikipedia.org/wiki/Fully_qualified_name#cite_note-7)

- In [Perl](https://en.wikipedia.org/wiki/Perl), a fully qualified [scalar](https://en.wikipedia.org/wiki/Scalar_(computing)) (`$scalar`) that is in the [package](https://en.wikipedia.org/wiki/Perl_module#Perl_packages_and_namespaces) `package2` would be referred to as `$package2::scalar` 

- In [Ruby](https://en.wikipedia.org/wiki/Ruby_(programming_language)), the fully qualified name of a class is the name of such class with all its parent modules, as Vehicles::Cars::Factory would be the fully qualified name of Factory class within Cars module within Vehicles module.

- In [COBOL](https://en.wikipedia.org/wiki/COBOL), a fully qualified data item name can be created by suffixing a potentially ambiguous identifier with an `IN` (or `OF`) phrase. For example, multiple data item records might contain a member item named `ACCOUNT-ID`, so specifying `ACCOUNT-ID IN CUSTOMER` serves to disambiguate a specific `ACCOUNT-ID` data item, specifically, the one that is a member of the parent `CUSTOMER` data item. Multiple clauses may be necessary to fully disambiguate a given identifier, for example, `ACCOUNT-ID IN CUSTOMER IN LAST-TRANSACTION`. This syntax is equivalent to the "dotted" notation employed in many object-oriented programming languages, but with the identifiers specified in reverse order.

##### Filenames and paths 

The term **fully qualified file name** means a file on a computer whose exact name is completely specified such that it is unambiguous and cannot be mistaken for any other file on that computer system.[[9\]](https://en.wikipedia.org/wiki/Fully_qualified_name#cite_note-9) It is somewhat equivalent on the Internet to a [URL](https://en.wikipedia.org/wiki/URL) specifying the full name of the computer and the entire name of a particular document as a file. The alternative is an **unqualified file name** or a partially qualified file name.

- On [Unix](https://en.wikipedia.org/wiki/Unix)-style systems, [DOS](https://en.wikipedia.org/wiki/DOS), and [Microsoft Windows](https://en.wikipedia.org/wiki/Microsoft_Windows), the name "sample" refers to a file in the [current directory](https://en.wikipedia.org/wiki/Current_directory) named "sample". If the current directory is changed, then the file referred to by the name "sample" is different. If you start the filename with "/" indicating the [root directory](https://en.wikipedia.org/wiki/Root_directory) as in "/Users/Name/sample", then on Unix this is a fully qualified file name. So, for example, instead of referring to `./foo/bar/baz.sh` in `/home/user/quz`, which uses a relative pathname, the fully qualified name would be `/home/user/quz/foo/bar/baz.sh`.[[10\]](https://en.wikipedia.org/wiki/Fully_qualified_name#cite_note-KochanWood2003-10) In DOS, the name is still relative to the root directory of the [current disk](https://en.wikipedia.org/wiki/Drive_letter_assignment), so to get a fully qualified file name, the file name must be prefixed with the drive letter and a colon, as in "C:\Users\Name\sample", where "C:" specifies the "C" drive.
- Also on the above systems, some programs such as the command-line shell will search a [path](https://en.wikipedia.org/wiki/Path_(computing)) for a file. Inserting a leading (back)slash, as in "./name", will stop the searching of the path. This is a *partially* qualified name, but not a fully qualified name as it still depends on the current directory. A fully qualified name, because it contains (back)slashes, will always stop a path search.
- On the [mainframe](https://en.wikipedia.org/wiki/Mainframe_computer) operating system [MUSIC/SP](https://en.wikipedia.org/wiki/MUSIC/SP), if one asks for the file name "X", one is making an unqualified reference either to the file X in the user's library, or to the file X in the common library if the user does not have a file named X and one does exist in the common library. If, however, one were to refer to "*COM:X" one is using a fully qualified file name reference to the file X in the common library even if they have a different file named X in their library. Similarly, a reference to "*USR:X" would mean the file in their own library, and "MA45:X" would be a fully qualified file name referring to the specific file X in the library of user MA45.
- On the [RSTS/E](https://en.wikipedia.org/wiki/RSTS/E) operating system on the [PDP-11](https://en.wikipedia.org/wiki/PDP-11) minicomputer, specifying a file "X.X" would refer to a file in one's own directory. Referring to "$X.X" would be referring to the file "[1,2]X.X" but to fully qualify the file name, one would have to indicate the device, so "SY:[46,145]MYFILE.TXT", "SY:$X.X", or "SY:[1,2]X.X" (the last two examples meaning the same thing) would be a fully qualified file name under RSTS/E.
- On the [UNIVAC Series 90](https://en.wikipedia.org/wiki/UNIVAC_Series_90) mainframe operating system [VS/9](https://en.wikipedia.org/wiki/VS/9), specifying a file "X" could mean either the file X in the account of that user, or could mean the file X in the library of the system manager, $TSOS. However, specifying "$S0103.X" would be a fully qualified file name.
- This term can also include the case where one prefixes a fully qualified file name with a specific computer name as a prefix to a file name (where the particular system permits referencing a file on another system), so long as the exact name is unambiguous.



### Qualified name  in programming language

### [python qualified name](https://docs.python.org/3/glossary.html#term-qualified-name)

显然，python quantified name是按照quantify name来构造的。