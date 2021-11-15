# Link-time optimization

## draft: cppreference [PImpl](https://en.cppreference.com/w/cpp/language/pimpl) # [Runtime overhead](https://en.cppreference.com/w/cpp/language/pimpl#Runtime_overhead)

> Access overhead: In pImpl, each call to a private member function indirects through a pointer. Each access to a public member made by a private member indirects through another pointer. Both indirections cross **translation unit boundaries** and so can only be optimized out by **link-time optimization**. Note that OO factory requires indirection across translation units to access both public data and implementation detail, and offers even fewer opportunities for the **link time optimizer** due to **virtual dispatch**.