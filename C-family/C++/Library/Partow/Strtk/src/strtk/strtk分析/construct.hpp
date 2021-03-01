   template <typename T1, typename T2, typename  T3, typename  T4,
             typename T5, typename T6, typename  T7, typename  T8,
             typename T9, typename T10, typename T11, typename T12>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const  T2&  t2, const  T3&  t3, const  T4&  t4,
                         const T5& t5, const  T6&  t6, const  T7&  t7, const  T8&  t8,
                         const T9& t9, const T10& t10, const T11& t11, const T12& t12)
   {
      output += type_to_string( t1); output += delimiter;
      output += type_to_string( t2); output += delimiter;
      output += type_to_string( t3); output += delimiter;
      output += type_to_string( t4); output += delimiter;
      output += type_to_string( t5); output += delimiter;
      output += type_to_string( t6); output += delimiter;
      output += type_to_string( t7); output += delimiter;
      output += type_to_string( t8); output += delimiter;
      output += type_to_string( t9); output += delimiter;
      output += type_to_string(t10); output += delimiter;
      output += type_to_string(t11); output += delimiter;
      output += type_to_string(t12);
   }

   template <typename T1, typename T2, typename  T3, typename T4,
             typename T5, typename T6, typename  T7, typename T8,
             typename T9, typename T10, typename T11>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const  T2&  t2, const T3& t3, const T4& t4,
                         const T5& t5, const  T6&  t6, const T7& t7, const T8& t8,
                         const T9& t9, const T10& t10, const T11& t11)
   {
      output += type_to_string( t1); output += delimiter;
      output += type_to_string( t2); output += delimiter;
      output += type_to_string( t3); output += delimiter;
      output += type_to_string( t4); output += delimiter;
      output += type_to_string( t5); output += delimiter;
      output += type_to_string( t6); output += delimiter;
      output += type_to_string( t7); output += delimiter;
      output += type_to_string( t8); output += delimiter;
      output += type_to_string( t9); output += delimiter;
      output += type_to_string(t10); output += delimiter;
      output += type_to_string(t11);
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9, typename T10>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                         const T9& t9, const T10& t10)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6); output += delimiter;
      output += type_to_string(t7); output += delimiter;
      output += type_to_string(t8); output += delimiter;
      output += type_to_string(t9); output += delimiter;
      output += type_to_string(t10);
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                         const T9& t9)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6); output += delimiter;
      output += type_to_string(t7); output += delimiter;
      output += type_to_string(t8); output += delimiter;
      output += type_to_string(t9);
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6); output += delimiter;
      output += type_to_string(t7); output += delimiter;
      output += type_to_string(t8);
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6); output += delimiter;
      output += type_to_string(t7);
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5,typename T6>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6);
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5);
   }

   template <typename T1, typename T2, typename T3, typename T4>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4);
   }

   template <typename T1, typename T2, typename T3>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3);
   }

   template <typename T1, typename T2>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2);
   }

   template <typename InputIterator>
   inline void join(std::string& output,
                    const std::string& delimiter,
                    const InputIterator begin,
                    const InputIterator end)
   {
      InputIterator itr = begin;
      while (end != itr)
      {
         output += type_to_string(*itr);
         if (end == (++itr))
            break;
         else
            output += delimiter;
      }
   }
