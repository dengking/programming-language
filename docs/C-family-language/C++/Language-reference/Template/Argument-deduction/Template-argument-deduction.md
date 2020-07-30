# Template argument deduction



## cppreference [Template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction)



Template argument deduction takes place after the function template [name lookup](https://en.cppreference.com/w/cpp/language/lookup) (which may involve [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)) and before [template argument substitution](https://en.cppreference.com/w/cpp/language/function_template#Template_argument_substitution) (which may involve [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae)) and [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution).

> NOTE: 需要将这些流程给串联起来

### Deduction from a function call



If there are multiple parameters, each `P/A` pair is deduced **separately** and the deduced template arguments are then **combined**. If deduction fails or is ambiguous for any `P/A` pair or if different pairs yield different deduced template arguments, or if any template argument remains neither deduced nor explicitly specified, compilation fails.

> NOTE: 上面这段话非常重要，它描述了deduction的机制



#### Non-deduced contexts

> NOTE: 在某些情况下，programmer是不想启用deduction的，所以C++提供了给了programmer这种控制能力。

In the following cases, the types, templates, and non-type values that are used to compose `P` do not participate in **template argument deduction**, but instead use the **template arguments** that were either deduced elsewhere or explicitly specified. If a template parameter is used only in **non-deduced contexts** and is not explicitly specified, template argument deduction fails.

