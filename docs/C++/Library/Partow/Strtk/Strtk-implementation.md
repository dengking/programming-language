# 20171024
对parse的实现分析，我觉得从以下代码入手：

```C++
  namespace details
   {
      struct not_supported_type_tag   {};
      struct unsigned_type_tag        {};
      struct signed_type_tag          {};
      struct real_type_tag            {};
      struct byte_type_tag            {};
      struct bool_type_tag            {};
      struct hex_number_type_tag      {};
      struct hex_string_type_tag      {};
      struct base64_type_tag          {};
      struct ignore_token_type_tag    {};
      struct stdstring_type_tag       {};
      struct stdstring_range_type_tag {};
      struct value_type_tag           {};
      struct sink_type_tag            {};
      struct stl_seq_type_tag         {};
      struct attribute_type_tag       {};
      struct semantic_action_type_tag {};
      struct expect_type_tag          {};
      struct like_type_tag            {};
      struct inrange_type_tag         {};
      struct trim_type_tag            {};
      struct lcase_type_tag           {};
      struct ucase_type_tag           {};
      struct fillchararray_type_tag   {};
      struct truncint_type_tag        {};
      struct decsink_type_tag         {};
      template <typename T>
      struct supported_conversion_to_type
      {
         typedef not_supported_type_tag type;
      };

      template <typename T>
      struct supported_conversion_from_type
      {
         typedef not_supported_type_tag type;
      };
      };
```
上述使用的是tag dispatch idiom。

看到这些内容的时候，我想到了之前在http://www.cplusplus.com/doc/上查找back_inserter的解释的时候，无意中发现了在c++中广泛使用的[traits](http://www.cplusplus.com/reference/type_traits/)技术。以及各种各样的tag，比如：


Input iterator category
Empty class to identify the category of an iterator as an input iterator:
```
struct input_iterator_tag {};
```

这些tag的用处：
1. inline bool string_to_type_converter_impl(Iterator& itr, const Iterator end, CaseToken& t, ucase_type_tag)
2. type_to_string_converter_impl(const strtk::util::value& v, std::string& result, value_type_tag)
3. 
总结：正如我们平时是使用标签进行分类一般，作者此处也是使用这些struct tag进行分类的。

# pod_type
```
strtk_register_pod_type
```

包括：bool,signed char,char


# tips:由一个模板类生成多个具体类
模板类定义如下：

```
      template <typename T>
      struct is_pod
      {
         typedef no_t result_t;
         enum { result = false };
      };
```
批量生成具体类的方法如下：

```
      //可以看到，在此处省略了模板参数
      #define strtk_register_pod_type(T)                                                           \
      template<> struct is_pod<T>{ typedef yes_t result_t; enum {result = true }; };               \
      template<> struct is_pod<const T>{ typedef yes_t result_t; enum {result = true }; };         \
      template<> struct is_pod<volatile T>{ typedef yes_t result_t; enum {result = true }; };      \
      template<> struct is_pod<const volatile T>{ typedef yes_t result_t; enum {result = true }; };\

      strtk_register_pod_type(bool)
      strtk_register_pod_type(signed char)
      strtk_register_pod_type(char)
      strtk_register_pod_type(short)
      strtk_register_pod_type(int)
      strtk_register_pod_type(long int)
      strtk_register_pod_type(long long int)
      strtk_register_pod_type(unsigned char)
      strtk_register_pod_type(unsigned short)
      strtk_register_pod_type(unsigned int)
      strtk_register_pod_type(unsigned long int)
      strtk_register_pod_type(unsigned long long int)
      strtk_register_pod_type(float)
      strtk_register_pod_type(double)
      strtk_register_pod_type(long double)

      #undef strtk_register_pod_type
```

模板类的定义如下：

```
template <typename>
      struct numeric {};
```
具体类的定义如下：

```
      template<>//可以看到，在此处省略了模板参数
      struct numeric<short>
      {
         static const unsigned int length       =  5;
         static const unsigned int size         = 16;
         static const unsigned int bound_length =  5;
         static const short        m10          = 3276;
         static const short        ldpos        =  7;
         static const short        ldneg        =  8;
      };
      template<>
      struct numeric<unsigned short>
      {
         static const unsigned int length       =  5;
         static const unsigned int size         = 16;
         static const unsigned int bound_length =  5;
         static const unsigned short m10        = 6553;
         static const unsigned short ldpos      =  5;
      };
```

实现的关键在string_to_type_converter，即如何将字符串转换为指定的类型。

# class row_type
该类中也有parse方法，下面是该类的parse方法的调用链分析

```
graph TD
A[parse]-->B[process]
B-->C[process]
```

# string_to_type_converter实现分析

```
graph TD
A[string_to_type_converter]-->B[string_to_type_converter_impl]
B-->
```

## string_to_type_converter_impl的实现
### 实现方式一
```
template <typename Iterator, typename T, typename Tag>
      inline bool string_to_type_converter_impl(Iterator& begin, const Iterator end, T& t, not_supported_type_tag)
      {
         #ifdef strtk_enable_lexical_cast
            try
            {
               t = boost::lexical_cast<T>(std::string(begin,end));
            }
            catch (const boost::bad_lexical_cast&)
            {
               return false;
            }
            begin = end;
            return true;
         #else
            try
            {
               std::stringstream ss(std::string(begin,end));
               ss >> t;
            }
            catch (const std::exception&)
            {
               return false;
            }
            begin = end;
            return true;
         #endif
      }
```

### 实现方式二

```
template <typename Iterator>
      inline bool string_to_type_converter_impl(Iterator& begin, const Iterator end, strtk::util::value& v, value_type_tag)
      {
         return v(begin,end);
      }

      template <typename Iterator>
      inline bool string_to_type_converter_impl(Iterator& begin, const Iterator end, std::string& t, stdstring_type_tag)
      {
         t.assign(begin,end);
         begin = end;
         return true;
      }

      template <typename Iterator, typename Expect>
      inline bool string_to_type_converter_impl(Iterator& itr, const Iterator end, Expect& t, expect_type_tag)
      {
         if (!t(itr,end))
            return false;
         itr = end;
         return true;
      }
```












