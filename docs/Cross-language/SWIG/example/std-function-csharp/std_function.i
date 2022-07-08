%{
  #include <functional>
  #include <iostream>

  #ifndef SWIG_DIRECTORS
  #error "Directors must be enabled in your SWIG module for std_function.i to work correctly"
  #endif
%}

/**
 * Source of the solution: https://stackoverflow.com/a/32668302/5035991
 */

//
// %std_function
//

// These are the 'action' macros for java code.
// Action defines what to do with argument, i.e. determine its type and name.
// Java version uses typemaps, and c++ uses just a type.
#define param(num,type) $typemap(jstype,type) arg ## num
#define unpack(num,type) arg##num
// For c++ code.
#define lvalref(num,type) type&& arg##num
#define forward(num,type) std::forward<type>(arg##num)

// Macro to provide 'action' for the number of arguments.
#define FE_0(...)
#define FE_1(action,a1) action(0,a1)
#define FE_2(action,a1,a2) action(0,a1), action(1,a2)
#define FE_3(action,a1,a2,a3) action(0,a1), action(1,a2), action(2,a3)
#define FE_4(action,a1,a2,a3,a4) action(0,a1), action(1,a2), action(2,a3), action(3,a4)
#define FE_5(action,a1,a2,a3,a4,a5) action(0,a1), action(1,a2), action(2,a3), action(3,a4), action(4,a5)

// FIXME This is (GET_MACRO) never expanded to FE_0, because __VA_ARGS__ is never empty (it may have single empty arguments, but this is still the argument).
// Therefore, for std::function with 0 args shoulde be used special macro %std_function_void.
// I cannot resolve this currently. The resolution may be to determine if __VA_ARGS__ has 1 empty argument, and take action on this basis.
#define GET_MACRO(_1,_2,_3,_4,_5,NAME,...) NAME
%define FOR_EACH(action,...)
	GET_MACRO(__VA_ARGS__, FE_5, FE_4, FE_3, FE_2, FE_1, FE_0)(action,__VA_ARGS__)
%enddef

%define %std_function(Name, Ret, ...)

%feature("director") Name##Impl;
// %typemap(javaclassmodifiers) Name##Impl "public abstract class";

%{
  struct Name##Impl {
    virtual ~Name##Impl() {}
    virtual Ret call(__VA_ARGS__) = 0;
  };
%}

// %javamethodmodifiers Name##Impl::call "abstract protected";
// %typemap(javaout) Ret Name##Impl::call ";" // Suppress the body of the abstract method

struct Name##Impl {
  virtual ~Name##Impl();
protected:
  virtual Ret call(__VA_ARGS__) = 0;
};

%typemap(maybereturn) SWIGTYPE "return ";
%typemap(maybereturn) void "";

// %typemap(javain) std::function<Ret(__VA_ARGS__)> "$javaclassname.getCPtr($javaclassname.makeNative($javainput))"

%typemap(in) std::function<Ret(__VA_ARGS__)>
{
	if ($input)
	{
		$1 = **(std::function<Ret(__VA_ARGS__)>**)&($input);
	}
	else
	{
		$1 = std::function<Ret(__VA_ARGS__)>();
	}
}

%rename(Name) std::function<Ret(__VA_ARGS__)>;
%rename(call) std::function<Ret(__VA_ARGS__)>::operator();

namespace std {
  struct function<Ret(__VA_ARGS__)> {
    // Copy constructor
    function<Ret(__VA_ARGS__)>(const std::function<Ret(__VA_ARGS__)>&);

    // Call operator
    Ret operator()(__VA_ARGS__) const;

    // Conversion constructor from function pointer
    function<Ret(__VA_ARGS__)>(Ret(*const)(__VA_ARGS__));

    %extend {
      function<Ret(__VA_ARGS__)>(Name##Impl *in) {
    return new std::function<Ret(__VA_ARGS__)>([=](FOR_EACH(lvalref,__VA_ARGS__)){
          return in->call(FOR_EACH(forward,__VA_ARGS__));
    });
      }
    }
  };
}

%enddef

//
// %std_function_void
//

/**
 * Macro for std::function<Ret()> with no arguments.
 * Where:
 *     Ret - return type.
 *
 * Usage:
 * %std_function_void(Functor, void);
 */
%define %std_function_void(Name, Ret)

%feature("director") Name##Impl;
// %typemap(javaclassmodifiers) Name "public abstract class";

%{
  struct Name##Impl {
    virtual ~Name##Impl() {}
    virtual Ret call() = 0;
  };
%}

// %javamethodmodifiers Name::call "abstract protected";
// %typemap(javaout) Ret Name::call ";" // Suppress the body of the abstract method

struct Name##Impl {
  virtual ~Name##Impl();
protected:
  virtual Ret call() = 0;
};

%typemap(maybereturn) SWIGTYPE "return ";
%typemap(maybereturn) void "";

// %typemap(javain) std::function<Ret()> "$javaclassname.getCPtr($javaclassname.makeNative($javainput))"

%typemap(in) std::function<Ret()>
{
	if ($input)
	{
		$1 = **(std::function<Ret()>**)&($input);
	}
	else
	{
		$1 = std::function<Ret()>();
	}
}

%rename(Name) std::function<Ret()>;
%rename(call) std::function<Ret()>::operator();

namespace std {
  struct function<Ret()> {
    // Copy constructor
    function<Ret()>(const std::function<Ret()>&);

    // Call operator
    Ret operator()() const;

    // Conversion constructor from function pointer
    function<Ret()>(Ret(*const)());

    %extend {
      function<Ret()>(Name##Impl *in) {
        return new std::function<Ret()>([=]{
          return in->call();
        });
      }
    }
  };
}

%enddef
