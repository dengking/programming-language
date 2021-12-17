# palletsprojects  [Template Designer Documentation](https://jinja.palletsprojects.com/en/3.0.x/templates/)

## Synopsis

> NOTE: 
>
> "synopsis": "概要"

A template contains **variables** and/or **expressions**, which get replaced with values when a template is *rendered*; and **tags**, which control the logic of the template. 

> NOTE: 
>
> 一、**variables** 是指什么？
>
> 参见 [Variables](https://jinja.palletsprojects.com/en/3.0.x/templates/#variables) 章节
>
> 二、**tags**
>
> "tag"是什么？上面对tag的解释是:  "**tags**, which control the logic of the template"，它是否表示的是statement？
>
> 下面是原文中，一些使用tag的地方:
>
> 1、"You can also strip whitespace in templates by hand. If you add a minus sign (`-`) to the start or end of a block (e.g. a [For](https://jinja.palletsprojects.com/en/3.0.x/templates/#for-loop) tag)"
>
> 2、With both `trim_blocks` and `lstrip_blocks` enabled, you can put **block tags** on their own lines
>
> 



There are a few kinds of delimiters(分隔符). The default Jinja delimiters are configured as follows:

1、`{% ... %}` for [Statements](https://jinja.palletsprojects.com/en/3.0.x/templates/#list-of-control-structures)

> NOTE: 
>
> 上述  [Statements](https://jinja.palletsprojects.com/en/3.0.x/templates/#list-of-control-structures) 所链接的是 [List of Control Structures](https://jinja.palletsprojects.com/en/3.0.x/templates/#list-of-control-structures) ，因此在jinja中，statement特指control flow statement。

2、`{{ ... }}` for [Expressions](https://jinja.palletsprojects.com/en/3.0.x/templates/#expressions) to print to the template output

> NOTE: 
>
> expression则是比较容易理解的，它和一般programming language中的expression的概念是一致的

3、`{# ... #}` for [Comments](https://jinja.palletsprojects.com/en/3.0.x/templates/#comments) not included in the template output



### Template File Extension

> NOTE: 
>
> 其实就是提供 "heuristic(启发)" :
>
> 1、"Adding a `.jinja` extension"
>
> 2、"Another good heuristic for identifying templates is that they are in a `templates` folder, regardless of extension. This is a common layout for projects."



## Variables

```jinja2
{{ foo.bar }}
{{ foo['bar'] }}
```

It’s important to know that the outer double-curly braces are *not* part of the variable, but the print statement. If you access variables inside **tags** don’t put the braces around them.

> NOTE: 
>
> 一、这段话告诉我们 `{{` 是 "print statement"
>
> 二、上面这段话中的 "**tags**" 要如何理解？
>
> 



## Filters

For example, `{{ name|striptags|title }}` will remove all HTML Tags from variable name and title-case the output (`title(striptags(name))`).

> NOTE: 
>
> `{{ name|striptags|title }}` 表示的含义是: (`title(striptags(name))`)，这让我觉得jinja是非常具有表现力的
>
> `|striptags` 表示 `striptags` 是一个filter。

`{{ listx|join(', ') }}` will join a list with commas (`str.join(', ', listx)`).

> NOTE: 
>
> `join` 是一个filter，它的第一个参数是 `', '`，这是一个字符串，第二个参数是 `listx`

## Tests

> NOTE: 
>
> 一、test一般是和 `if` 一起使用的
>
> 二、可以对什么进行test呢？一般是对variable进行test

## [Whitespace Control](https://jinja.palletsprojects.com/en/3.0.x/templates/#whitespace-control)

With both `trim_blocks` and `lstrip_blocks` enabled, you can put **block tags** on their own lines, and the entire block line will be removed when rendered, preserving the whitespace of the contents.

> NOTE: 
>
> 下面例子中的 `{% if True %}`、`{% endif %}` 都是  **block tags** ； `trim_blocks` and `lstrip_blocks`  用于控制  **block tags** 所占用的行是否会出现在输出文件中

For example, without the `trim_blocks` and `lstrip_blocks` options, this template:

```jinja2
<div>
    {% if True %}
        yay
    {% endif %}
</div>
```

gets rendered with blank lines inside the div:

```jinja2
<div>

        yay

</div>
```

But with both `trim_blocks` and `lstrip_blocks` enabled, the template block lines are removed and other whitespace is preserved:

```jinja2
<div>
        yay
</div>
```

## [Escaping](https://jinja.palletsprojects.com/en/3.0.x/templates/#escaping)

> NOTE: 
>
> ```jinja2
> {% raw %}
> 
> {% endraw %}
> ```
>
> 



## [Line Statements](https://jinja.palletsprojects.com/en/3.0.x/templates/#line-statements)

> NOTE: 
>
> "line statement" 其实就是 statement



## [Template Inheritance](https://jinja.palletsprojects.com/en/3.0.x/templates/#template-inheritance)

The most powerful part of Jinja is template inheritance. Template inheritance allows you to build a base “skeleton” template that contains all the common elements of your site and defines **blocks** that child templates can override.

> NOTE: 
>
> 一、具备inheritance、override、super的power
>
> 二、skeleton、scaffold



### Base Template



```jinja2
<!DOCTYPE html>
<html lang="en">
<head>
    {% block head %}
    <link rel="stylesheet" href="style.css" />
    <title>{% block title %}{% endblock %} - My Webpage</title>
    {% endblock %}
</head>
<body>
    <div id="content">{% block content %}{% endblock %}</div>
    <div id="footer">
        {% block footer %}
        &copy; Copyright 2008 by <a href="http://domain.invalid/">you</a>.
        {% endblock %}
    </div>
</body>
</html>
```

> NOTE: 
>
> 一、可以看到，title block内嵌在head block中



## Assignments

