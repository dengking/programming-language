# modernescpp [Acquire-Release Semantic](https://www.modernescpp.com/index.php/acquire-release-semantic)

## Synchronisation and ordering constraints

The acquire-release semantic is based on one key idea. A release operation synchronises with an acquire operation on the same atomic and establishes, in addition, an ordering constraint. So, all read and write operations can not be moved before an **acquire operation**, all read and write operations can not be move behind a **release operation**. But what is an acquire or release operation? The reading of an atomic variable with `load` or `test_and_set` is an **acquire operation**. But in addition the acquiring of a lock. Of course, the opposite is also true. The releasing of a lock is a release operation. Accordingly, a `store` or `clear` operation on an atomic variable.

> NOTE: 先acquire，然后release
>
> read-acquire
>
> write-release

It's worth to reason once more on the few last sentences from a different perspective. The lock of a mutex is an acquire operation, the unlock of a mutex a release operation. Figuratively speaking that implies, that an operation on a variable can not moved outside of a [critical section](https://www.modernescpp.com/index.php/threads-sharing-data). That hold for both directions. On the other side, a variable can be moved inside of a criticial section. Because the variable moves from the not protected to the protected area. Now with a little delay the picture.



