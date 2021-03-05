   template <typename InputIterator, typename OutputIterator>
   inline void copy_n(InputIterator itr, std::size_t n, OutputIterator out)
   {
      while (n)
      {
         (*out) = (*itr);
         ++itr;
         ++out;
         --n;
      }
   }

   template <typename Predicate,
             typename InputIterator,
             typename OutputIterator>
   inline void copy_if(Predicate predicate,
                       const InputIterator begin, const InputIterator end,
                       OutputIterator out)
   {
      InputIterator itr = begin;
      while (end != itr)
      {
         if (predicate(*itr))
         {
            *(out++) = (*itr);
         }
         ++itr;
      }
   }

   template <typename Predicate,
             typename InputIterator,
             typename OutputIterator>
   inline InputIterator copy_while(Predicate predicate,
                                   const InputIterator begin, const InputIterator end,
                                   OutputIterator out)
   {
      InputIterator itr = begin;
      while (end != itr)
      {
         if (!predicate(*itr))
            return itr;
         *(out++) = *(itr++);
      }

      return end;
   }

   template <typename Predicate,
             typename InputIterator,
             typename OutputIterator>
   inline InputIterator copy_until(Predicate predicate,
                                   const InputIterator begin, const InputIterator end,
                                   OutputIterator out)
   {
      InputIterator itr = begin;
      while (end != itr)
      {
         if (predicate(*itr))
            return itr;
         *(out++) = *(itr++);
      }

      return end;
   }
