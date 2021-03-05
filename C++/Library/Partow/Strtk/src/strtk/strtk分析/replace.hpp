   template <typename Iterator>
   inline void replace(const typename std::iterator_traits<Iterator>::value_type& c1,
                       const typename std::iterator_traits<Iterator>::value_type& c2,
                       const Iterator begin,
                       const Iterator end)
   {
      for (Iterator itr = begin; end != itr; ++itr)
      {
         if (c1 == (*itr))
         {
            (*itr) = c2;
         }
      }
   }

   inline void replace(const std::string::value_type& c0,
                       const std::string::value_type& c1,
                       std::string& s)
   {
      replace(c0, c1,
              const_cast<char*>(to_ptr(s)),
              const_cast<char*>(to_ptr(s) + s.size()));
   }

   template <typename T>
   inline void replace(const T& c1, const T& c2, const range::adapter<T>& r)
   {
      replace(c1, c2, r.begin(), r.end());
   }

   inline void replace_pattern(const std::string& s, // input
                               const std::string& p, // pattern
                               const std::string& r, // replacement
                               std::string& n)
   {
      if (p.empty() || (p == r))
      {
         n.assign(s);
         return;
      }

      const std::size_t p_size = p.size();
      const std::size_t r_size = r.size();

      int inc = static_cast<int>(r_size) - static_cast<int>(p_size);

      std::size_t pos = 0;

      std::vector<std::size_t> delta_list;
      delta_list.reserve(std::min<std::size_t>(32,(s.size() / p_size) + 1));

      while (std::string::npos != (pos = s.find(p,pos)))
      {
         delta_list.push_back(pos);
         pos += p_size;
      }

      if (delta_list.empty())
      {
         n.assign(s);
         return;
      }

      n.resize(delta_list.size() * inc + s.size(), 0x00);

      char* n_itr = const_cast<char*>(to_ptr(n));

      const char* s_end   = to_ptr(s) + s.size();
      const char* s_itr   = to_ptr(s);
      const char* r_begin = to_ptr(r);
      const char* r_end   = to_ptr(r) + r_size;

      const std::size_t delta_list_size = delta_list.size();

      std::size_t i = 0;
      std::size_t delta = delta_list[0];

      for ( ; ; )
      {
         std::copy(s_itr, s_itr + delta, n_itr);
         s_itr += p_size + delta;
         n_itr += delta;
         std::copy(r_begin, r_end, n_itr);
         n_itr += r_size;

         if (++i >= delta_list_size)
            break;

         delta =  delta_list[i] - (delta_list[i - 1] + p_size);
      }

      if (s_end != s_itr)
      {
         std::copy(s_itr, s_end, n_itr);
      }
   }

   template <typename InputIterator, typename OutputIterator>
   inline std::size_t replace_pattern(const InputIterator s_begin, const InputIterator s_end, // Input
                                      const InputIterator p_begin, const InputIterator p_end, // Pattern
                                      const InputIterator r_begin, const InputIterator r_end, // Replacement
                                      OutputIterator out)
   {
      InputIterator s_itr  = s_begin;
      InputIterator r_itr  = r_begin;
      InputIterator p_itr  = p_begin;

      const std::size_t p_size = std::distance(p_begin,p_end);
      const std::size_t r_size = std::distance(r_begin,r_end);

      if (
           (0 == p_size) ||
           (
             (p_size == r_size) &&
             std::equal(p_begin,p_end,r_begin)
           )
         )
      {
         std::copy(s_begin,s_end,out);
         return std::distance(s_begin,s_end);
      }

      std::size_t pos = 0;
      std::size_t prev_pos = 0;
      std::size_t count = 0;
      std::size_t new_size = std::distance(s_begin,s_end);
      int inc = r_size - p_size;

      InputIterator temp_s_itr = s_itr;

      while (s_end != s_itr)
      {
         /*
            Need to replace the following search code with
            Knuth-Pratt-Morris or Boyer-Moore string search
            algorithms.
         */
         bool found = true;
         p_itr = p_begin;
         temp_s_itr = s_itr;

         while ((p_end != p_itr) && (s_end != temp_s_itr))
         {
            if (*(temp_s_itr++) != *(p_itr++))
            {
               found = false;
               break;
            }
         }

         if (found && (p_itr == p_end))
         {
            ++count;
            new_size += inc;
            s_itr = temp_s_itr;
         }
         else
            ++s_itr;
      }

      s_itr = s_begin;
      p_itr = p_begin;

      pos = 0;
      prev_pos = 0;

      temp_s_itr = s_itr;

      while (0 < count)
      {
         p_itr = p_begin;
         bool found = true;
         InputIterator pattern_start = temp_s_itr;

         while ((p_end != p_itr) && (s_end != temp_s_itr))
         {
            if (*(temp_s_itr++) != *(p_itr++))
            {
               found = false;
               temp_s_itr = pattern_start;
               ++temp_s_itr;
               break;
            }
         }

         if (!found || (p_itr != p_end)) continue;

         pos = std::distance(s_begin,temp_s_itr) - p_size;
         int diff = pos - prev_pos;

         std::copy(s_itr,s_itr + diff, out);
         s_itr = temp_s_itr;
         std::copy(r_itr,r_end, out);

         pos += p_size;
         prev_pos = pos;
         --count;
      }

      std::copy(s_itr,s_end,out);

      return new_size;
   }

   inline void remove_pattern(const std::string& s,
                              const std::string& p,
                              std::string& n)
   {
      static const std::string r("");
      replace_pattern(s,p,r,n);
   }
