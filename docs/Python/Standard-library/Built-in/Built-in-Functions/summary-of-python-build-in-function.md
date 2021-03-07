# Built-in Functions[¶](https://docs.python.org/3/library/functions.html#built-in-functions)

 ## 分类

### type system相关



#### 类型查询与操作

[`super()`](https://docs.python.org/3/library/functions.html#super)





[`callable()`](https://docs.python.org/3/library/functions.html#callable)

[`issubclass()`](https://docs.python.org/3/library/functions.html#issubclass)

[`isinstance()`](https://docs.python.org/3/library/functions.html#isinstance)

[`type()`](https://docs.python.org/3/library/functions.html#type)

***SUMMARY*** : [What are the differences between type() and isinstance()?](https://stackoverflow.com/questions/1549801/what-are-the-differences-between-type-and-isinstance)



##### decorator

[`classmethod()`](https://docs.python.org/3/library/functions.html#classmethod)

[`staticmethod()`](https://docs.python.org/3/library/functions.html#staticmethod)

[`property()`](https://docs.python.org/3/library/functions.html#property)





##### 属性操作

[`setattr()`](https://docs.python.org/3/library/functions.html#setattr)

[`delattr()`](https://docs.python.org/3/library/functions.html#delattr)

[`getattr()`](https://docs.python.org/3/library/functions.html#getattr)

[`hasattr()`](https://docs.python.org/3/library/functions.html#hasattr)



##### symbol table

[`globals()`](https://docs.python.org/3/library/functions.html#globals)

[`locals()`](https://docs.python.org/3/library/functions.html#locals)



[`dir()`](https://docs.python.org/3/library/functions.html#dir)

[`vars()`](https://docs.python.org/3/library/functions.html#vars)


#### 内置数据类型

[`object()`](https://docs.python.org/3/library/functions.html#object)



按照3.2. The standard type hierarchy[¶](https://docs.python.org/3/reference/datamodel.html#the-standard-type-hierarchy)中进行组织

##### [`numbers.Number`](https://docs.python.org/3/library/numbers.html#numbers.Number)

- [`numbers.Integral`](https://docs.python.org/3/library/numbers.html#numbers.Integral)

  [`int()`](https://docs.python.org/3/library/functions.html#int)

  [`bool()`](https://docs.python.org/3/library/functions.html#bool)

- [`numbers.Real`](https://docs.python.org/3/library/numbers.html#numbers.Real) ([`float`](https://docs.python.org/3/library/functions.html#float))

  [`float()`](https://docs.python.org/3/library/functions.html#float)

- [`numbers.Complex`](https://docs.python.org/3/library/numbers.html#numbers.Complex) ([`complex`](https://docs.python.org/3/library/functions.html#complex))

  [`complex()`](https://docs.python.org/3/library/functions.html#complex)



##### Sequences

###### Immutable sequences

[`str()`](https://docs.python.org/3/library/functions.html#func-str)

[`tuple()`](https://docs.python.org/3/library/functions.html#func-tuple)

[`bytes()`](https://docs.python.org/3/library/functions.html#func-bytes)

###### Mutable sequences

[`list()`](https://docs.python.org/3/library/functions.html#func-list)

[`bytearray()`](https://docs.python.org/3/library/functions.html#func-bytearray)

[`range()`](https://docs.python.org/3/library/functions.html#func-range)

Rather than being a function, [`range`](https://docs.python.org/3/library/stdtypes.html#range) is actually an immutable sequence type, as documented in [Ranges](https://docs.python.org/3/library/stdtypes.html#typesseq-range) and [Sequence Types — list, tuple, range](https://docs.python.org/3/library/stdtypes.html#typesseq).



##### Set types

[`set()`](https://docs.python.org/3/library/functions.html#func-set)

[`frozenset()`](https://docs.python.org/3/library/functions.html#func-frozenset)



##### Mappings

[`dict()`](https://docs.python.org/3/library/functions.html#func-dict)







##### TODO



[`slice()`](https://docs.python.org/3/library/functions.html#slice)





### 数进制

[`bin()`](https://docs.python.org/3/library/functions.html#bin)

[`oct()`](https://docs.python.org/3/library/functions.html#oct)

[`hex()`](https://docs.python.org/3/library/functions.html#hex)



### 字符串与整数

[`chr()`](https://docs.python.org/3/library/functions.html#chr)

[`ord()`](https://docs.python.org/3/library/functions.html#ord)



### 转换为字符串

[`ascii()`](https://docs.python.org/3/library/functions.html#ascii)

[`format()`](https://docs.python.org/3/library/functions.html#format)

[`repr()`](https://docs.python.org/3/library/functions.html#repr)

### 转换为二进制

[`memoryview()`](https://docs.python.org/3/library/functions.html#func-memoryview)

### 算法

[`all()`](https://docs.python.org/3/library/functions.html#all)

[`any()`](https://docs.python.org/3/library/functions.html#any)

[`filter()`](https://docs.python.org/3/library/functions.html#filter)

[`max()`](https://docs.python.org/3/library/functions.html#max)

[`min()`](https://docs.python.org/3/library/functions.html#min)

[`sum()`](https://docs.python.org/3/library/functions.html#sum)

[`sorted()`](https://docs.python.org/3/library/functions.html#sorted)

[`map()`](https://docs.python.org/3/library/functions.html#map)

[`next()`](https://docs.python.org/3/library/functions.html#next)

[`enumerate()`](https://docs.python.org/3/library/functions.html#enumerate)

[`iter()`](https://docs.python.org/3/library/functions.html#iter)

[`len()`](https://docs.python.org/3/library/functions.html#len)

[`reversed()`](https://docs.python.org/3/library/functions.html#reversed)

[`sorted()`](https://docs.python.org/3/library/functions.html#sorted)

[`zip()`](https://docs.python.org/3/library/functions.html#zip)





### 算术运算

[`abs()`](https://docs.python.org/3/library/functions.html#abs)

[`divmod()`](https://docs.python.org/3/library/functions.html#divmod)

[`hash()`](https://docs.python.org/3/library/functions.html#hash)

[`pow()`](https://docs.python.org/3/library/functions.html#pow)

[`round()`](https://docs.python.org/3/library/functions.html#round)



### 执行流程控制

[`breakpoint()`](https://docs.python.org/3/library/functions.html#breakpoint)



### IO

[`input()`](https://docs.python.org/3/library/functions.html#input)

[`open()`](https://docs.python.org/3/library/functions.html#open)

[`print()`](https://docs.python.org/3/library/functions.html#print)





### python解释器相关

[`compile()`](https://docs.python.org/3/library/functions.html#compile)

[`eval()`](https://docs.python.org/3/library/functions.html#eval)

[`exec()`](https://docs.python.org/3/library/functions.html#exec)

[`help()`](https://docs.python.org/3/library/functions.html#help)

[`id()`](https://docs.python.org/3/library/functions.html#id)

[`__import__()`](https://docs.python.org/3/library/functions.html#__import__)







|                                                              |                                                              | Built-in Functions                                           |                                                              |                                                              |
| :----------------------------------------------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| [`abs()`](https://docs.python.org/3/library/functions.html#abs) | [`delattr()`](https://docs.python.org/3/library/functions.html#delattr) | [`hash()`](https://docs.python.org/3/library/functions.html#hash) | [`memoryview()`](https://docs.python.org/3/library/functions.html#func-memoryview) | [`set()`](https://docs.python.org/3/library/functions.html#func-set) |
| [`all()`](https://docs.python.org/3/library/functions.html#all) | [`dict()`](https://docs.python.org/3/library/functions.html#func-dict) | [`help()`](https://docs.python.org/3/library/functions.html#help) | [`min()`](https://docs.python.org/3/library/functions.html#min) | [`setattr()`](https://docs.python.org/3/library/functions.html#setattr) |
| [`any()`](https://docs.python.org/3/library/functions.html#any) | [`dir()`](https://docs.python.org/3/library/functions.html#dir) | [`hex()`](https://docs.python.org/3/library/functions.html#hex) | [`next()`](https://docs.python.org/3/library/functions.html#next) | [`slice()`](https://docs.python.org/3/library/functions.html#slice) |
| [`ascii()`](https://docs.python.org/3/library/functions.html#ascii) | [`divmod()`](https://docs.python.org/3/library/functions.html#divmod) | [`id()`](https://docs.python.org/3/library/functions.html#id) | [`object()`](https://docs.python.org/3/library/functions.html#object) | [`sorted()`](https://docs.python.org/3/library/functions.html#sorted) |
| [`bin()`](https://docs.python.org/3/library/functions.html#bin) | [`enumerate()`](https://docs.python.org/3/library/functions.html#enumerate) | [`input()`](https://docs.python.org/3/library/functions.html#input) | [`oct()`](https://docs.python.org/3/library/functions.html#oct) | [`staticmethod()`](https://docs.python.org/3/library/functions.html#staticmethod) |
| [`bool()`](https://docs.python.org/3/library/functions.html#bool) | [`eval()`](https://docs.python.org/3/library/functions.html#eval) | [`int()`](https://docs.python.org/3/library/functions.html#int) | [`open()`](https://docs.python.org/3/library/functions.html#open) | [`str()`](https://docs.python.org/3/library/functions.html#func-str) |
| [`breakpoint()`](https://docs.python.org/3/library/functions.html#breakpoint) | [`exec()`](https://docs.python.org/3/library/functions.html#exec) | [`isinstance()`](https://docs.python.org/3/library/functions.html#isinstance) | [`ord()`](https://docs.python.org/3/library/functions.html#ord) | [`sum()`](https://docs.python.org/3/library/functions.html#sum) |
| [`bytearray()`](https://docs.python.org/3/library/functions.html#func-bytearray) | [`filter()`](https://docs.python.org/3/library/functions.html#filter) | [`issubclass()`](https://docs.python.org/3/library/functions.html#issubclass) | [`pow()`](https://docs.python.org/3/library/functions.html#pow) | [`super()`](https://docs.python.org/3/library/functions.html#super) |
| [`bytes()`](https://docs.python.org/3/library/functions.html#func-bytes) | [`float()`](https://docs.python.org/3/library/functions.html#float) | [`iter()`](https://docs.python.org/3/library/functions.html#iter) | [`print()`](https://docs.python.org/3/library/functions.html#print) | [`tuple()`](https://docs.python.org/3/library/functions.html#func-tuple) |
| [`callable()`](https://docs.python.org/3/library/functions.html#callable) | [`format()`](https://docs.python.org/3/library/functions.html#format) | [`len()`](https://docs.python.org/3/library/functions.html#len) | [`property()`](https://docs.python.org/3/library/functions.html#property) | [`type()`](https://docs.python.org/3/library/functions.html#type) |
| [`chr()`](https://docs.python.org/3/library/functions.html#chr) | [`frozenset()`](https://docs.python.org/3/library/functions.html#func-frozenset) | [`list()`](https://docs.python.org/3/library/functions.html#func-list) | [`range()`](https://docs.python.org/3/library/functions.html#func-range) | [`vars()`](https://docs.python.org/3/library/functions.html#vars) |
| [`classmethod()`](https://docs.python.org/3/library/functions.html#classmethod) | [`getattr()`](https://docs.python.org/3/library/functions.html#getattr) | [`locals()`](https://docs.python.org/3/library/functions.html#locals) | [`repr()`](https://docs.python.org/3/library/functions.html#repr) | [`zip()`](https://docs.python.org/3/library/functions.html#zip) |
| [`compile()`](https://docs.python.org/3/library/functions.html#compile) | [`globals()`](https://docs.python.org/3/library/functions.html#globals) | [`map()`](https://docs.python.org/3/library/functions.html#map) | [`reversed()`](https://docs.python.org/3/library/functions.html#reversed) | [`__import__()`](https://docs.python.org/3/library/functions.html#__import__) |
| [`complex()`](https://docs.python.org/3/library/functions.html#complex) | [`hasattr()`](https://docs.python.org/3/library/functions.html#hasattr) | [`max()`](https://docs.python.org/3/library/functions.html#max) | [`round()`](https://docs.python.org/3/library/functions.html#round) |                                                              |