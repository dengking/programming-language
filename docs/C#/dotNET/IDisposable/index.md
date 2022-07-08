# IDisposable Interface

## microsoft [IDisposable Interface](https://docs.microsoft.com/en-us/dotnet/api/system.idisposable?view=net-6.0)

Provides a mechanism for releasing unmanaged resources.



## stackoverflow [Proper use of the IDisposable interface](https://stackoverflow.com/questions/538060/proper-use-of-the-idisposable-interface)

I know from reading [Microsoft documentation](https://docs.microsoft.com/dotnet/api/system.idisposable) that the "primary" use of the `IDisposable` interface is to clean up **unmanaged resources**.

To me, "unmanaged" means things like database connections, sockets, window handles, etc. But, I've seen code where the `Dispose()` method is implemented to free *managed* resources, which seems redundant to me, since the **garbage collector** should take care of that for you.