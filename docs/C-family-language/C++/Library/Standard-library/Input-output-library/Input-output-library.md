# Input/output library



c++的io library可以说将stream的概念发扬光大了。

c的io library也是基于stream的。



## cppreference [Input/output library](Input/output library)





## bit stream

看了一下，c++的io library是不支持bit stream的，那在c++中如何来处理bit stream呢？

https://sakhnik.com/2018/03/23/bitstream.html

http://www.drdobbs.com/bitstream-parsing-in-c/184402014



## buffer stream

今天写了这样的一个程序

```c++
std::pair<void*, size_t> Pack()
{
	MsgHeaderType Header;
	Header.MsgHead.nSenderID = 1;
	Header.MsgHead.nToken = 1;
	Header.DataHead.uResponseNumber = 3;

	CHSInsFundDataPushField F1;
	std::memset(&F1, 0, sizeof(CHSInsFundDataPushField));
	F1.ProductIndex = 1;
	F1.ProductNo = 1;
	std::strcpy(F1.ProductRef, "1");

	CHSInsFundDataPushField F2;
	std::memset(&F2, 0, sizeof(CHSInsFundDataPushField));
	F2.ProductIndex = 2;
	F2.ProductNo = 2;
	std::strcpy(F2.ProductRef, "2");

	CHSInsFundDataPushField F3;
	std::memset(&F3, 0, sizeof(CHSInsFundDataPushField));
	F3.ProductIndex = 3;
	F3.ProductNo = 3;
	std::strcpy(F3.ProductRef, "3");

	std::size_t size = sizeof(MsgHeaderType) + 3 * sizeof(CHSInsFundDataPushField);
	void*Buffer = std::malloc(size);
	std::memcpy(Buffer, &Header, sizeof(MsgHeaderType));
	std::memcpy((char*) Buffer + sizeof(MsgHeaderType), &F1, sizeof(CHSInsFundDataPushField));
	std::memcpy((char*) Buffer + sizeof(MsgHeaderType) + sizeof(CHSInsFundDataPushField), &F2, sizeof(CHSInsFundDataPushField));
	std::memcpy((char*) Buffer + sizeof(MsgHeaderType) + sizeof(CHSInsFundDataPushField) * 2, &F3, sizeof(CHSInsFundDataPushField));
	return std::pair<void*, size_t> { Buffer, size };
}
```

这个程序其实非常简单，即将一些struct object输出到一个buffer中，我想的是在c++中有没有更加灵活的方式来实现它？我的第一想法是stream，即我需要一个buffer stream，它可以实现类似于`stringstream`的`<<`操作，下面是查询到的一些与此相关的内容：

[What exactly is streambuf? How do I use it?](https://stackoverflow.com/questions/8116541/what-exactly-is-streambuf-how-do-i-use-it)

[A dynamic buffer type in C++?](https://stackoverflow.com/questions/1874354/a-dynamic-buffer-type-in-c)

另外与此相关的有：c++ byte array，因为我们一般使用byte array来作为buffer。这让我想到了redis的做法，redis为每个client都添加了一个buffer。