[TOC]

# Deployment Options[¶](http://flask.pocoo.org/docs/1.0/deploying/#deployment-options)

While lightweight and easy to use, **Flask’s built-in server is not suitable for production** as it doesn’t scale well. Some of the options available for properly running Flask in production are documented here.

If you want to deploy your Flask application to a **[WSGI](https://en.wikipedia.org/wiki/Web_Server_Gateway_Interface) server** not listed here, look up the server documentation about how to use a WSGI app with it. Just remember that your `Flask` application object is the actual WSGI application.

