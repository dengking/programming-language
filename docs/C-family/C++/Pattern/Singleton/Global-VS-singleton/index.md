# Global and singleton

虽然两种都能够实现execute once、single object，但是相比于global，singleton有着优势:

1、

singleton: construct on first use

global: *initialisation order fiasco*

## stackoverflow [extern vs Singleton class](https://stackoverflow.com/questions/12247912/extern-vs-singleton-class)

[A](https://stackoverflow.com/a/12248004)

Global variables suffer from the *initialisation order fiasco*. Globals in separate translation units are initialised in an unspecified order, so you get undefined behaviour if one refers to another in its constructor.

Static variables in a function scope are initialised the first time the function is called, which (more or less) solves the initialisation order problem.

However, more subtle problems remain; in particular, it is still possible to access the object after it has been destroyed, and there is some overhead to ensure thread-safe initialisation (and, in older compilers, initialisation might not be thread-safe at all). There is no completely safe way to manage globally accessible objects of non-trivial types, and my advice would be to avoid them altogether if possible.

## Example: MFC `theApp`

MFC的`theApp`是全局唯一的，它的声明与定义如下：

`CMFCDemoApp.h`

```c++
class CMFCDemoApp : public CWinApp
{
public:
	CMFCDemoApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMFCDemoApp theApp;
```

`CMFCDemoApp.cpp`

```c++
// 唯一的 CMFCHSTradeApiDemoApp 对象

CMFCDemoApp theApp;
```

## TODO

1、ogre3d [Singleton vs. extern/global variable](https://forums.ogre3d.org/viewtopic.php?f=10&t=29576&sid=d2bf955bdf9876c76a8054b5c4060317)

