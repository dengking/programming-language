# swig object of pointer type

## 问题描述

```C++
typedef enum
{
	kNERtcNoError = 0,		/**< No errors. */

} NERtcErrorCode;
```



```C++
class IRtcEngineEventHandler
{
    virtual void onJoinChannel(channel_id_t cid, uid_t uid, NERtcErrorCode nertc_result, uint64_t elapsed) {
        (void)cid;
        (void)uid;
        (void)nertc_result;
        (void)elapsed;
    }
};
```



swig生成的Python如下:

```Python
    def onJoinChannel(self, cid, uid, nertc_result, elapsed):
        """
        加入房间回调
        """
        logging.info(
            f"cid={cid}, uid={uid}, nertc_result={nertc_result}, elapsed={elapsed}")
        if kNERtcNoError == nertc_result:
            logging.info(f'成功加入房间{cid}')
        else:
            logging.info(f'加入房间设备，错误码{nertc_result}')
        self.channel_id = cid
```

其中 `nertc_result` 的type:

```
<Swig Object of type 'NERtcErrorCode *' at 0x7fb95de0e240>
```

## 解法1

看到这个问题，我第一想法是: 将enum映射为int，于是Google swig enum to int: 

按照 [SWIG: How to map an enum to integer without %including the header?](https://stackoverflow.com/questions/61917132/swig-how-to-map-an-enum-to-integer-without-including-the-header) # [A](https://stackoverflow.com/a/62020933/10173843) 中的写法如下:

```C++
%include "typemaps.i"

%apply unsigned int { NERtcErrorCode };
```

上述解法能够解决问题，需要注意的是: 需要使用unqualified name，即如果将 `NERtcErrorCode` 所属的namespace也带上，则是无法生效的。

## 解法2

Google Swig Object of type ' *' at 0x:

bnikolic [C++ & SWIG: using data pointed to by a pointer](http://www.bnikolic.co.uk/blog/cpp-swig-pointeraccess.html)

这应该是一种更加通用的写法。