# Extern and singleton

## MFC `theApp`

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



## [Singleton vs. extern/global variable](https://forums.ogre3d.org/viewtopic.php?f=10&t=29576&sid=d2bf955bdf9876c76a8054b5c4060317)

