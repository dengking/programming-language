[TOC]



## 

# FastCGI[¶](https://flask.palletsprojects.com/en/1.1.x/deploying/fastcgi/#fastcgi)

FastCGI is a deployment option on servers like [nginx](https://nginx.org/), [lighttpd](https://www.lighttpd.net/), and [cherokee](http://cherokee-project.com/); see [uWSGI](https://flask.palletsprojects.com/en/1.1.x/deploying/uwsgi/) and [Standalone WSGI Containers](https://flask.palletsprojects.com/en/1.1.x/deploying/wsgi-standalone/) for other options. To use your WSGI application with any of them you will need a FastCGI server first. The most popular one is [flup](https://pypi.org/project/flup/)which we will use for this guide. Make sure to have it installed to follow along.

