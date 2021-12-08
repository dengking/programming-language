# flask implementation



## [`Flask`](https://github.com/pallets/flask/blob/main/src/flask/app.py)

### [Application Object API](https://flask.palletsprojects.com/en/2.0.x/api/#application-object)



### About the First Parameter: **`import_name`** 

> **`import_name`**: the name of the application package

需要注意的是: 它指的是"application"，而不是flask框架，显然一般会将application需要的静态的资源(template、static)等，放到application package中，这样就可以将**`import_name`** 作为锚点而去找到这些文件。

#### [Application Object API](https://flask.palletsprojects.com/en/2.0.x/api/#application-object) # About the First Parameter



#### stackoverflow [How to understand the first parameter in Flask instance init?](https://stackoverflow.com/questions/31768371/how-to-understand-the-first-parameter-in-flask-instance-init)