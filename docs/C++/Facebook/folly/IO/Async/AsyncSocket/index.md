

## 数据成员

### callback

```C++
  ImmediateReadCB immediateReadHandler_; ///< LoopCallback for checking read

  ConnectCallback* connectCallback_; ///< ConnectCallback
  ErrMessageCallback* errMessageCallback_; ///< TimestampCallback
  ReadAncillaryDataCallback*
      readAncillaryDataCallback_; ///< AncillaryDataCallback
  SendMsgParamsCallback* ///< Callback for retrieving
      sendMsgParamCallback_; ///< ::sendmsg() parameters
  ReadCallback* readCallback_; ///< ReadCallback
```

