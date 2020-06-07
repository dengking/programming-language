   namespace details
   {
      template <typename DelimiterPredicate,
                typename Iterator,
                typename OutputIterator>
      inline std::size_t split_compress_delimiters(const DelimiterPredicate& delimiter,
                                                   const Iterator begin,
                                                   const Iterator end,
                                                   OutputIterator out)
      {
         std::size_t token_count = 0;
         std::pair<Iterator,Iterator> range(begin,begin);

         while (end != range.second)
         {
            if (delimiter(*range.second))
            {
               (*out) = range;
               ++out;
               while ((end != ++range.second) && delimiter(*range.second));
               range.first = range.second;
               if (end != range.second)
                  ++range.second;
               ++token_count;
            }
            else
               ++range.second;
         }

         if ((range.first != range.second) || delimiter(*(range.second - 1)))
         {
            (*out) = range;
            ++out;
            ++token_count;
         }

         return token_count;
      }
   }
