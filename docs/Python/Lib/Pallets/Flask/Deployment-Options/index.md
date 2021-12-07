# Deployment Options

## Deployment Options[¶](http://flask.pocoo.org/docs/1.0/deploying/#deployment-options)

While lightweight and easy to use, **Flask’s built-in server is not suitable for production** as it doesn’t scale well. Some of the options available for properly running Flask in production are documented here.

If you want to deploy your Flask application to a **[WSGI](https://en.wikipedia.org/wiki/Web_Server_Gateway_Interface) server** not listed here, look up the server documentation about how to use a WSGI app with it. Just remember that your `Flask` application object is the actual WSGI application.





## Self-hosted options[¶](https://flask.palletsprojects.com/en/1.1.x/deploying/#self-hosted-options)



## FastCGI[¶](https://flask.palletsprojects.com/en/1.1.x/deploying/fastcgi/#fastcgi)

FastCGI is a deployment option on servers like [nginx](https://nginx.org/), [lighttpd](https://www.lighttpd.net/), and [cherokee](http://cherokee-project.com/); see [uWSGI](https://flask.palletsprojects.com/en/1.1.x/deploying/uwsgi/) and [Standalone WSGI Containers](https://flask.palletsprojects.com/en/1.1.x/deploying/wsgi-standalone/) for other options. To use your WSGI application with any of them you will need a FastCGI server first. The most popular one is [flup](https://pypi.org/project/flup/)which we will use for this guide. Make sure to have it installed to follow along.



## uWSGI[¶](https://flask.palletsprojects.com/en/1.1.x/deploying/uwsgi/#uwsgi)

uWSGI is a deployment option on servers like [nginx](https://nginx.org/), [lighttpd](https://www.lighttpd.net/), and [cherokee](http://cherokee-project.com/); see [FastCGI](https://flask.palletsprojects.com/en/1.1.x/deploying/fastcgi/) and [Standalone WSGI Containers](https://flask.palletsprojects.com/en/1.1.x/deploying/wsgi-standalone/) for other options. To use your WSGI application with **uWSGI protocol** you will need a uWSGI server first. uWSGI is both a protocol and an application server; the application server can serve uWSGI, FastCGI, and HTTP protocols.

The most popular uWSGI server is [uwsgi](https://uwsgi-docs.readthedocs.io/en/latest/), which we will use for this guide. Make sure to have it installed to follow along.

> Watch Out
> Please make sure in advance that any `app.run()` calls you might have in your application file are inside an `if __name__ =='__main__':` block or moved to a separate file. Just make sure it’s not called because this will always start a local WSGI server which we do not want if we deploy that application to uWSGI.



## Nginx

vladikk [Serving Flask with Nginx](https://vladikk.com/2013/09/12/serving-flask-with-nginx-on-ubuntu/)