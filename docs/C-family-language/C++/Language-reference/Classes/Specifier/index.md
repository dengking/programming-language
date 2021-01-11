# Specifier

下面是C++中关于class的一些specifier:

|          | 章节                                                         |      |
| -------- | ------------------------------------------------------------ | ---- |
| default  |                                                              |      |
| delete   |                                                              |      |
| explicit | `C++\Language-reference\Classes\Special-member-function\Constructor\User-defined-conversion\Explicit-specifier` |      |
| final    |                                                              |      |
| override | `C++\Language-reference\Classes\Subtype-polymorphism\Inheritance\Virtual-function\Override-specifier` |      |



## Use specific to make it explicit principle

C++中提供了丰富的specifier，programmer应该优先使用specifier来使意图明确。

CppCoreGuidelines [C.80: Use `=default` if you have to be explicit about using the default semantics](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c80-use-default-if-you-have-to-be-explicit-about-using-the-default-semantics)

CppCoreGuidelines [C.81: Use `=delete` when you want to disable default behavior (without wanting an alternative)](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c81-use-delete-when-you-want-to-disable-default-behavior-without-wanting-an-alternative)

### microsoft [Explicitly Defaulted and Deleted Functions](https://docs.microsoft.com/en-us/cpp/cpp/explicitly-defaulted-and-deleted-functions?view=msvc-160)



### Example

下面是关于此的一些example:

codeproject [C++ Tip: Make Your Class Non-copyable Without Boost](https://www.codeproject.com/Tips/1220451/Cplusplus-Tip-Make-Your-Class-Non-copyable-Without) # Boost noncopyable

```C++
  class noncopyable
  {
  protected:
#if !defined(BOOST_NO_CXX11_DEFAULTED_FUNCTIONS) && 
         !defined(BOOST_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS)
      BOOST_CONSTEXPR noncopyable() = default;
      ~noncopyable() = default;
#else
      noncopyable() {}
      ~noncopyable() {}
#endif
#if !defined(BOOST_NO_CXX11_DELETED_FUNCTIONS)
      noncopyable( const noncopyable& ) = delete;
      noncopyable& operator=( const noncopyable& ) = delete;
#else
  private:  // emphasize the following members are private
      noncopyable( const noncopyable& );
      noncopyable& operator=( const noncopyable& );
#endif
  };
```

从上述boost的source code可以看出，它是典型的遵循上述原则的。

stackoverflow [C++ Singleton design pattern](https://stackoverflow.com/questions/1008019/c-singleton-design-pattern) # [A](https://stackoverflow.com/a/1008289)

```C++
class S
{
    public:
        static S& getInstance()
        {
            static S    instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
    private:
        S() {}                    // Constructor? (the {} brackets) are needed here.

        // C++ 03
        // ========
        // Don't forget to declare these two. You want to make sure they
        // are inaccessible(especially from outside), otherwise, you may accidentally get copies of
        // your singleton appearing.
        S(S const&);              // Don't Implement
        void operator=(S const&); // Don't implement

        // C++ 11
        // =======
        // We can use the better technique of deleting the methods
        // we don't want.
    public:
        S(S const&)               = delete;
        void operator=(S const&)  = delete;

        // Note: Scott Meyers mentions in his Effective Modern
        //       C++ book, that deleted functions should generally
        //       be public as it results in better error messages
        //       due to the compilers behavior to check accessibility
        //       before deleted status
};
```

