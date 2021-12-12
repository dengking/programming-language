# Jinja 

感觉像是PHP的玩法。

## wikipedia [Jinja (template engine)](https://en.wikipedia.org/wiki/Jinja_(template_engine))



## palletsprojects [Jinja](https://jinja.palletsprojects.com/en/3.0.x/) 



## [chromium Jinja](https://www.chromium.org/developers/jinja)



## realpython [Primer on Jinja Templating](https://realpython.com/primer-on-jinja-templating/)

> NOTE: 
>
> 从中可知，jinja还支持 "Custom Filters"，这是非常强大的



## Example

[clast](https://github.com/AndrewWalker/clast)/[clastgen](https://github.com/AndrewWalker/clast/tree/master/clastgen)/**[templates.py](https://github.com/AndrewWalker/clast/blob/master/clastgen/templates.py)** 

## 语法

### jinja string template

stackoverflow [jinja2 load template from string: TypeError: no loader for this environment specified](https://stackoverflow.com/questions/39288706/jinja2-load-template-from-string-typeerror-no-loader-for-this-environment-spec)

下面是我的practice:

```Python
from jinja2 import Environment, BaseLoader
C2CppConvertDeclJinjaTemplate = '''
{{m.cpp_name}} ConvertC2Cpp(const {{m.c_name}}& c_obj);
{{m.cpp_name}} ConvertC2Cpp(const {{m.c_name}}* c_obj);
'''
rtemplate = Environment(loader=BaseLoader).from_string(C2CppConvertDeclJinjaTemplate)
class M:
    def __init__(self):
        self.c_name = 'c'
        self.cpp_name = 'cpp'
m=M()
print(rtemplate.render(m=m))
```

输出如下:

```Python
cpp ConvertC2Cpp(const c& c_obj);
cpp ConvertC2Cpp(const c* c_obj);
```



### jinja one line for loop

stackoverflow [How to output a comma delimited list in jinja python template?](https://stackoverflow.com/questions/11974318/how-to-output-a-comma-delimited-list-in-jinja-python-template)

下面是我的practice:

```python
from jinja2 import Environment, BaseLoader

t_str = '''
{% for host in groups['tag_Function_logdb']  %}
{{ host }}:9300
{%- if loop.first %},{% endif %}
{% endfor %}
'''
t = Environment(loader=BaseLoader).from_string(t_str)
groups = {'tag_Function_logdb': [1, 2, 3, 4, 5]}
print(t.render(groups=groups))
```

输出如下:

```C++
1:9300,

2:9300

3:9300

4:9300

5:9300
```



#### join

stackoverflow [Jinja2 template variables to one line](https://stackoverflow.com/questions/38076968/jinja2-template-variables-to-one-line)

下面是我的practice:

```Python
from jinja2 import Environment, BaseLoader
t_str = '''{{ users|join(', ') }}'''
t = Environment(loader=BaseLoader).from_string(t_str)
users = [1,2,3,4,5]
print(t.render(users=users))
```

输出如下:

```
1, 2, 3, 4, 5
```



### jinja in

stackoverflow [How to See if a String Contains Another String in Django Template](https://stackoverflow.com/questions/19627911/how-to-see-if-a-string-contains-another-string-in-django-template)



### pass argument

stackoverflow [How to pass selected, named arguments to Jinja2's include context?](https://stackoverflow.com/questions/9404990/how-to-pass-selected-named-arguments-to-jinja2s-include-context)