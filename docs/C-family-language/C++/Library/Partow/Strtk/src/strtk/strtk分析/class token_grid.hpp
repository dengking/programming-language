   class token_grid
   {
   public:

      typedef const unsigned char* iterator_t;
      typedef unsigned int index_t;
      typedef std::pair<iterator_t,iterator_t> range_t;
      typedef std::deque<range_t> token_list_t;
      typedef std::pair<index_t,index_t> row_index_range_t;
      typedef std::deque<row_index_range_t> row_index_t;
      typedef std::pair<index_t,index_t> row_range_t;
      typedef std::pair<index_t,index_t> col_range_t;

   private:

      struct store
      {
         store()
         : max_column(0)
         {}

         token_list_t token_list;
         row_index_t   row_index;
         std::size_t  max_column;

         inline void clear()
         {
            token_list.clear();
            row_index.clear();
         }

         inline range_t operator()(const std::size_t& col, const std::size_t& row) const
         {
            if (row < row_index.size())
            {
               const row_index_range_t& r = row_index[row];

               if (col < (r.second - r.first + 1))
                  return *(token_list.begin() + (r.first + col));
               else
                  return null_range();
            }
            else
               return null_range();
         }

         inline bool remove_row(const std::size_t& row)
         {
            if (row >= row_index.size()) return false;

            row_index_range_t& r = row_index[row];

            const std::size_t number_of_tokens = r.second - r.first + 1;

            token_list_t::iterator remove_begin = token_list.begin() + r.first;
            token_list_t::iterator remove_end = token_list.begin() + r.first + number_of_tokens;

            token_list.erase(remove_begin,remove_end);

            row_index.erase(row_index.begin() + row);

            for (std::size_t i = row; i < row_index.size(); ++i)
            {
               row_index_range_t& curr_row = row_index[i];

               curr_row.first  -= static_cast<unsigned int>(number_of_tokens);
               curr_row.second -= static_cast<unsigned int>(number_of_tokens);
            }

            return true;
         }

         inline std::size_t token_count(const row_index_range_t& r) const
         {
            return (r.second - r.first + 1);
         }

         inline std::size_t token_count(const std::size_t& index) const
         {
            return token_count(row_index[index]);
         }

         inline bool remove_row_range(const std::size_t& r0, const std::size_t& r1)
         {
            if (r0 > r1)
               return false;
            else if (r0 >= row_index.size())
               return false;
            else if (r1 >= row_index.size())
               return false;

            std::size_t number_of_tokens = 0;

            for (std::size_t i = r0; i <= r1; ++i)
            {
               row_index_range_t& r = row_index[i];
               number_of_tokens += token_count(r);
            }

            row_index_range_t rr0 = row_index[r0];

            token_list_t::iterator remove_begin = token_list.begin() + rr0.first;
            token_list_t::iterator remove_end   = token_list.begin() + rr0.first + number_of_tokens;

            token_list.erase(remove_begin,remove_end);

            row_index.erase(row_index.begin() + r0,row_index.begin() + r0 + (r1 - r0 + 1));

            for (std::size_t i = r0; i < row_index.size(); ++i)
            {
               row_index_range_t& r = row_index[i];
               r.first  -= static_cast<unsigned int>(number_of_tokens);
               r.second -= static_cast<unsigned int>(number_of_tokens);
            }

            return true;
         }

         struct remove_column_impl
         {
            std::size_t column;
            std::size_t counter;
            std::size_t remainder;
            std::size_t current_row;

            inline void update(store& idx)
            {
               current_row++;

               while (current_row < idx.row_index.size())
               {
                  const std::size_t number_of_tokens = idx.token_count(current_row);

                  if (number_of_tokens > column)
                     break;

                  counter += number_of_tokens;

                  ++current_row;
               }

               if (current_row < idx.row_index.size())
               {
                  counter += column + remainder;
                  row_index_range_t& r = idx.row_index[current_row];
                  remainder = (r.second - r.first) - column;
               }
               else
                  counter = std::numeric_limits<std::size_t>::max();
            }

            inline void process(store& idx)
            {
               token_list_t::iterator itr1 = idx.token_list.begin();
               token_list_t::iterator itr2 = idx.token_list.begin();
               token_list_t::iterator end  = idx.token_list.end  ();

               counter     = 0;
               remainder   = 0;
               current_row = static_cast<std::size_t>(-1);

               update(idx);

               while (end != itr1)
               {
                  while ((end != itr1) && (0 != counter))
                  {
                     if (itr1 != itr2)
                     {
                        (*itr2) = (*itr1);
                     }

                     ++itr1;
                     ++itr2;
                     --counter;
                  }

                  if (0 == counter)
                  {
                     update(idx);
                     ++itr1;
                  }
               }

               std::size_t remove_count = 0;

               idx.max_column = std::numeric_limits<std::size_t>::min();

               for (std::size_t i = 0; i < idx.row_index.size(); ++i)
               {
                  row_index_range_t& r = idx.row_index[i];
                  std::size_t token_count = (r.second - r.first + 1);

                  r.first -= static_cast<unsigned int>(remove_count);

                  if (token_count > column)
                  {
                     ++remove_count;
                  }

                  r.second -= static_cast<unsigned int>(remove_count);
                  token_count = (r.second - r.first + 1);

                  if (token_count > idx.max_column)
                     idx.max_column = token_count;
               }

               idx.token_list.resize(idx.token_list.size() - remove_count);
            }
         };

         inline bool remove_column(const std::size_t& column)
         {
            if (column >= max_column) return false;
            remove_column_impl rc;
            rc.column = column;
            rc.process(*this);
            return true;
         }

         inline static range_t null_range()
         {
            static const range_t null_range_ = range_t(reinterpret_cast<const unsigned char*>(0),
                                                       reinterpret_cast<const unsigned char*>(0));
            return null_range_;
         }

      };

      template <typename DelimiterPredicate = multiple_char_delimiter_predicate>
      struct row_processor
      {
         row_processor(store& idx,
                       DelimiterPredicate& tp,
                       const split_options::type split_mode = split_options::compress_delimiters)
         : idx_(idx),
           row_start_index_(0),
           row_end_index_(0),
           token_predicate_(tp),
           split_mode_(split_mode)
         {
            idx_.max_column = std::numeric_limits<std::size_t>::min();
         }

         inline void operator()(const range_t& range)
         {
            if (0 == std::distance(range.first,range.second))
               return;

            row_start_index_ = static_cast<index_t>(idx_.token_list.size());

            std::size_t token_count = split(token_predicate_,
                                            range.first, range.second,
                                            std::back_inserter(idx_.token_list),
                                            split_mode_);

            row_end_index_ = static_cast<index_t>(row_start_index_ + token_count - 1);

            idx_.row_index.push_back(std::make_pair(row_start_index_,row_end_index_));

            if (token_count > idx_.max_column)
               idx_.max_column = token_count;
         }

         row_processor<DelimiterPredicate> operator=(const row_processor<DelimiterPredicate>&);

         store& idx_;
         index_t row_start_index_;
         index_t row_end_index_;
         DelimiterPredicate& token_predicate_;
         split_options::type split_mode_;
      };

   public:

      inline row_range_t range(index_t lower_bound,
                               index_t upper_bound = std::numeric_limits<index_t>::max()) const
      {
         if (upper_bound == std::numeric_limits<index_t>::max())
         {
            upper_bound = static_cast<index_t>(dsv_index_.token_list.size());
         }
         else if (upper_bound > dsv_index_.token_list.size())
         {
            return row_range_t(std::numeric_limits<index_t>::max(),std::numeric_limits<index_t>::max());
         }
         else if (lower_bound > upper_bound)
         {
            return row_range_t(std::numeric_limits<index_t>::max(),std::numeric_limits<index_t>::max());
         }

         return row_range_t(lower_bound,upper_bound);
      }

      struct options
      {
         options()
         : row_split_option(split_options::compress_delimiters),
           column_split_option(split_options::compress_delimiters),
           row_delimiters("\n\r"),
           column_delimiters(",|;\t "),
           support_dquotes(false),
           trim_dquotes(false)
         {}

         options(split_options::type sro,
                 split_options::type sco,
                 const std::string& rd,
                 const std::string& cd,
                 const bool support_dq = false,
                 const bool trim_dq = false)
         : row_split_option(sro),
           column_split_option(sco),
           row_delimiters(rd),
           column_delimiters(cd),
           support_dquotes(support_dq),
           trim_dquotes(trim_dq)
         {}

         inline options& set_column_split_option(const split_options::type& option)
         {
            column_split_option = option;
            return *this;
         }

         inline options& set_row_split_option(const split_options::type& option)
         {
            row_split_option = option;
            return *this;
         }

         inline options& set_column_delimiters(const std::string& delimiters)
         {
            column_delimiters = delimiters;
            return *this;
         }

         inline options& set_row_delimiters(const std::string& delimiters)
         {
            row_delimiters = delimiters;
            return *this;
         }

         split_options::type row_split_option;
         split_options::type column_split_option;
         std::string row_delimiters;
         std::string column_delimiters;
         bool support_dquotes;
         bool trim_dquotes;
      };

      class row_type
      {
      private:

         typedef std::pair<bool,row_type*> row_pair_type;

      public:

         row_type()
         : index_(std::numeric_limits<std::size_t>::max()),
           size_(0)
         {}

         row_type(const std::size_t& index,
                  const store& dsv_index)
         : index_(index),
           size_ (dsv_index.token_count(index)),
           begin_(dsv_index.token_list.begin() +  dsv_index.row_index[index].first)
         {}

         inline bool is_null(const std::size_t& index) const
         {
            const range_t& range = *(begin_ + index);
            return (0 == std::distance(range.first,range.second));
         }

         template <typename T>
         inline T operator[](const std::size_t& index) const
         {
            const range_t& range = *(begin_ + index);
            return string_to_type_converter<T>(range.first,range.second);
         }

         template <typename T>
         inline T get(const std::size_t& index) const
         {
            return operator[]<T>(index);
         }

         inline col_range_t all_columns() const
         {
            return col_range_t(0,static_cast<index_t>(size()));
         }

         inline range_t range() const
         {
            return range_t((*begin_).first,(*(begin_ + (size_ - 1))).second);
         }

         inline range_t token(const std::size_t& index) const
         {
            return *(begin_ + index);
         }

         inline std::size_t index() const
         {
            return index_;
         }

         inline std::size_t size() const
         {
            return size_;
         }

         inline std::size_t raw_length() const
         {
            std::size_t result = 0;
            token_list_t::const_iterator itr = begin_;

            for (std::size_t i = 0; i < size_; ++i, ++itr)
            {
               const range_t& range = (*itr);
               result += std::distance(range.first,range.second);
            }

            return result;
         }

         inline std::size_t raw_length(const std::size_t& column_index) const
         {
            const range_t& range = *(begin_ + column_index);
            return std::distance(range.first,range.second);
         }

         inline std::string as_string() const
         {
            std::string result;
            result.reserve(std::distance(begin_->first,(begin_ + (size_ - 1))->second));
            token_list_t::const_iterator itr = begin_;

            for (std::size_t i = 0; i < size_; ++i, ++itr)
            {
               const range_t& range = (*itr);
               result.append(range.first,range.second);
            }

            return result;
         }

         inline void as_string(std::string& out) const
         {
            out = as_string();
         }

         template <typename T0, typename T1, typename T2, typename T3,
                   typename T4, typename T5, typename T6, typename T7,
                   typename T8, typename T9>
         inline bool parse_with_index(const std::size_t& col0, const std::size_t& col1,
                                      const std::size_t& col2, const std::size_t& col3,
                                      const std::size_t& col4, const std::size_t& col5,
                                      const std::size_t& col6, const std::size_t& col7,
                                      const std::size_t& col8, const std::size_t& col9,
                                      T0& t0, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5,
                                      T6& t6, T7& t7, T8& t8, T9& t9) const
         {
            if (!process(*(begin_ + col0),t0)) return false;
            if (!process(*(begin_ + col1),t1)) return false;
            if (!process(*(begin_ + col2),t2)) return false;
            if (!process(*(begin_ + col3),t3)) return false;
            if (!process(*(begin_ + col4),t4)) return false;
            if (!process(*(begin_ + col5),t5)) return false;
            if (!process(*(begin_ + col6),t6)) return false;
            if (!process(*(begin_ + col7),t7)) return false;
            if (!process(*(begin_ + col8),t8)) return false;
            if (!process(*(begin_ + col9),t9)) return false;
            return true;
         }

         template <typename T0, typename T1, typename T2, typename T3,
                   typename T4, typename T5, typename T6, typename T7,
                   typename T8>
         inline bool parse_with_index(const std::size_t& col0, const std::size_t& col1,
                                      const std::size_t& col2, const std::size_t& col3,
                                      const std::size_t& col4, const std::size_t& col5,
                                      const std::size_t& col6, const std::size_t& col7,
                                      const std::size_t& col8,
                                      T0& t0, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5,
                                      T6& t6, T7& t7, T8& t8) const
         {
            if (!process(*(begin_ + col0),t0)) return false;
            if (!process(*(begin_ + col1),t1)) return false;
            if (!process(*(begin_ + col2),t2)) return false;
            if (!process(*(begin_ + col3),t3)) return false;
            if (!process(*(begin_ + col4),t4)) return false;
            if (!process(*(begin_ + col5),t5)) return false;
            if (!process(*(begin_ + col6),t6)) return false;
            if (!process(*(begin_ + col7),t7)) return false;
            if (!process(*(begin_ + col8),t8)) return false;
            return true;
         }

         template <typename T0, typename T1, typename T2, typename T3,
                   typename T4, typename T5, typename T6, typename T7>
         inline bool parse_with_index(const std::size_t& col0, const std::size_t& col1,
                                      const std::size_t& col2, const std::size_t& col3,
                                      const std::size_t& col4, const std::size_t& col5,
                                      const std::size_t& col6, const std::size_t& col7,
                                      T0& t0, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5,
                                      T6& t6, T7& t7) const
         {
            if (!process(*(begin_ + col0),t0)) return false;
            if (!process(*(begin_ + col1),t1)) return false;
            if (!process(*(begin_ + col2),t2)) return false;
            if (!process(*(begin_ + col3),t3)) return false;
            if (!process(*(begin_ + col4),t4)) return false;
            if (!process(*(begin_ + col5),t5)) return false;
            if (!process(*(begin_ + col6),t6)) return false;
            if (!process(*(begin_ + col7),t7)) return false;
            return true;
         }

         template <typename T0, typename T1, typename T2, typename T3,
                   typename T4, typename T5, typename T6>
         inline bool parse_with_index(const std::size_t& col0, const std::size_t& col1,
                                      const std::size_t& col2, const std::size_t& col3,
                                      const std::size_t& col4, const std::size_t& col5,
                                      const std::size_t& col6,
                                      T0& t0, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5,
                                      T6& t6) const
         {
            if (!process(*(begin_ + col0),t0)) return false;
            if (!process(*(begin_ + col1),t1)) return false;
            if (!process(*(begin_ + col2),t2)) return false;
            if (!process(*(begin_ + col3),t3)) return false;
            if (!process(*(begin_ + col4),t4)) return false;
            if (!process(*(begin_ + col5),t5)) return false;
            if (!process(*(begin_ + col6),t6)) return false;
            return true;
         }

         template <typename T0, typename T1, typename T2, typename T3,
                   typename T4, typename T5>
         inline bool parse_with_index(const std::size_t& col0, const std::size_t& col1,
                                      const std::size_t& col2, const std::size_t& col3,
                                      const std::size_t& col4, const std::size_t& col5,
                                      T0& t0, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5) const
         {
            if (!process(*(begin_ + col0),t0)) return false;
            if (!process(*(begin_ + col1),t1)) return false;
            if (!process(*(begin_ + col2),t2)) return false;
            if (!process(*(begin_ + col3),t3)) return false;
            if (!process(*(begin_ + col4),t4)) return false;
            if (!process(*(begin_ + col5),t5)) return false;
            return true;
         }

         template <typename T0, typename T1, typename T2, typename T3, typename T4>
         inline bool parse_with_index(const std::size_t& col0, const std::size_t& col1,
                                      const std::size_t& col2, const std::size_t& col3,
                                      const std::size_t& col4,
                                      T0& t0, T1& t1, T2& t2, T3& t3, T4& t4) const
         {
            if (!process(*(begin_ + col0),t0)) return false;
            if (!process(*(begin_ + col1),t1)) return false;
            if (!process(*(begin_ + col2),t2)) return false;
            if (!process(*(begin_ + col3),t3)) return false;
            if (!process(*(begin_ + col4),t4)) return false;
            return true;
         }

         template <typename T0, typename T1, typename T2, typename T3>
         inline bool parse_with_index(const std::size_t& col0, const std::size_t& col1,
                                      const std::size_t& col2, const std::size_t& col3,
                                      T0& t0, T1& t1, T2& t2, T3& t3) const
         {
            if (!process(*(begin_ + col0),t0)) return false;
            if (!process(*(begin_ + col1),t1)) return false;
            if (!process(*(begin_ + col2),t2)) return false;
            if (!process(*(begin_ + col3),t3)) return false;
            return true;
         }

         template <typename T0, typename T1, typename T2>
         inline bool parse_with_index(const std::size_t& col0, const std::size_t& col1,
                                      const std::size_t& col2,
                                      T0& t0, T1& t1, T2& t2) const
         {
            if (!process(*(begin_ + col0),t0)) return false;
            if (!process(*(begin_ + col1),t1)) return false;
            if (!process(*(begin_ + col2),t2)) return false;
            return true;
         }

         template <typename T0, typename T1>
         inline bool parse_with_index(const std::size_t& col0, const std::size_t& col1,
                                      T0& t0, T1& t1) const
         {
            if (!process(*(begin_ + col0),t0)) return false;
            if (!process(*(begin_ + col1),t1)) return false;
            return true;
         }

         template <typename T>
         inline bool parse_with_index(const std::size_t& col, T& t) const
         {
            return process(*(begin_ + col),t);
         }

         template <typename T0, typename T1, typename T2, typename T3,
                   typename T4, typename T5, typename T6, typename T7,
                   typename T8, typename T9 >
         inline bool parse(T0& t0, T1& t1, T2& t2, T3& t3,
                           T4& t4, T5& t5, T6& t6, T7& t7,
                           T8& t8, T9& t9) const
         {
            if (!process(*(begin_ + 0),t0)) return false;
            if (!process(*(begin_ + 1),t1)) return false;
            if (!process(*(begin_ + 2),t2)) return false;
            if (!process(*(begin_ + 3),t3)) return false;
            if (!process(*(begin_ + 4),t4)) return false;
            if (!process(*(begin_ + 5),t5)) return false;
            if (!process(*(begin_ + 6),t6)) return false;
            if (!process(*(begin_ + 7),t7)) return false;
            if (!process(*(begin_ + 8),t8)) return false;
            if (!process(*(begin_ + 9),t9)) return false;
            return true;
         }

         template <typename T0, typename T1, typename T2, typename T3,
                   typename T4, typename T5, typename T6, typename T7,
                   typename T8>
         inline bool parse(T0& t0, T1& t1, T2& t2, T3& t3,
                           T4& t4, T5& t5, T6& t6, T7& t7,
                           T8& t8) const
         {
            if (!process(*(begin_ + 0),t0)) return false;
            if (!process(*(begin_ + 1),t1)) return false;
            if (!process(*(begin_ + 2),t2)) return false;
            if (!process(*(begin_ + 3),t3)) return false;
            if (!process(*(begin_ + 4),t4)) return false;
            if (!process(*(begin_ + 5),t5)) return false;
            if (!process(*(begin_ + 6),t6)) return false;
            if (!process(*(begin_ + 7),t7)) return false;
            if (!process(*(begin_ + 8),t8)) return false;
            return true;
         }

         template <typename T0, typename T1, typename T2, typename T3,
                   typename T4, typename T5, typename T6, typename T7>
         inline bool parse(T0& t0, T1& t1, T2& t2, T3& t3,
                           T4& t4, T5& t5, T6& t6, T7& t7) const
         {
            if (!process(*(begin_ + 0),t0)) return false;
            if (!process(*(begin_ + 1),t1)) return false;
            if (!process(*(begin_ + 2),t2)) return false;
            if (!process(*(begin_ + 3),t3)) return false;
            if (!process(*(begin_ + 4),t4)) return false;
            if (!process(*(begin_ + 5),t5)) return false;
            if (!process(*(begin_ + 6),t6)) return false;
            if (!process(*(begin_ + 7),t7)) return false;
            return true;
         }

         template <typename T0, typename T1, typename T2, typename T3,
                   typename T4, typename T5, typename T6>
         inline bool parse(T0& t0, T1& t1, T2& t2, T3& t3,
                           T4& t4, T5& t5, T6& t6) const
         {
            if (!process(*(begin_ + 0),t0)) return false;
            if (!process(*(begin_ + 1),t1)) return false;
            if (!process(*(begin_ + 2),t2)) return false;
            if (!process(*(begin_ + 3),t3)) return false;
            if (!process(*(begin_ + 4),t4)) return false;
            if (!process(*(begin_ + 5),t5)) return false;
            if (!process(*(begin_ + 6),t6)) return false;
            return true;
         }

         template <typename T0, typename T1, typename T2, typename T3,
                   typename T4, typename T5>
         inline bool parse(T0& t0, T1& t1, T2& t2, T3& t3,
                           T4& t4, T5& t5) const
         {
            if (!process(*(begin_ + 0),t0)) return false;
            if (!process(*(begin_ + 1),t1)) return false;
            if (!process(*(begin_ + 2),t2)) return false;
            if (!process(*(begin_ + 3),t3)) return false;
            if (!process(*(begin_ + 4),t4)) return false;
            if (!process(*(begin_ + 5),t5)) return false;
            return true;
         }

         template <typename T0, typename T1,
                   typename T2, typename T3,typename T4>
         inline bool parse(T0& t0, T1& t1, T2& t2, T3& t3, T4& t4) const
         {
            if (!process(*(begin_ + 0),t0)) return false;
            if (!process(*(begin_ + 1),t1)) return false;
            if (!process(*(begin_ + 2),t2)) return false;
            if (!process(*(begin_ + 3),t3)) return false;
            if (!process(*(begin_ + 4),t4)) return false;
            return true;
         }

         template <typename T0, typename T1,
                   typename T2, typename T3>
         inline bool parse(T0& t0, T1& t1, T2& t2, T3& t3) const
         {
            if (!process(*(begin_ + 0),t0)) return false;
            if (!process(*(begin_ + 1),t1)) return false;
            if (!process(*(begin_ + 2),t2)) return false;
            if (!process(*(begin_ + 3),t3)) return false;
            return true;
         }

         template <typename T0, typename T1, typename T2>
         inline bool parse(T0& t0, T1& t1, T2& t2) const
         {
            if (!process(*(begin_ + 0),t0)) return false;
            if (!process(*(begin_ + 1),t1)) return false;
            if (!process(*(begin_ + 2),t2)) return false;
            return true;
         }

         template <typename T0, typename T1>
         inline bool parse(T0& t0, T1& t1) const
         {
            if (!process(*(begin_ + 0),t0)) return false;
            if (!process(*(begin_ + 1),t1)) return false;
            return true;
         }

         template <typename T0>
         inline bool parse(T0& t) const
         {
            return process(*begin_,t);
         }

         template <typename T, typename OutputIterator>
         inline void parse(OutputIterator out) const
         {
            token_list_t::const_iterator itr = begin_;
            const token_list_t::const_iterator end = begin_ + size_;

            while (end != itr)
            {
               const range_t& range = (*itr);
               *(out++) = string_to_type_converter<T>(range.first,range.second);
               ++itr;
            }
         }

         bool validate_column_range(const col_range_t& range) const
         {
            if ((range.first > size()) || (range.second > size()))
               return false;
            else if (range.first > range.second)
               return false;
            else
               return true;
         }

         col_range_t range(const index_t& lower_bound,
                           const index_t& upper_bound = std::numeric_limits<index_t>::max()) const
         {
            if (std::numeric_limits<index_t>::max() != upper_bound)
               return col_range_t(lower_bound,upper_bound);
            else
              return col_range_t(lower_bound,static_cast<index_t>(size()));
         }

         template <typename T,
                   typename Allocator,
                   template <typename,typename> class Sequence>
         inline bool parse(const col_range_t& range,
                           Sequence<T,Allocator>& sequence) const
         {
            if (!validate_column_range(range))
               return false;

            token_list_t::const_iterator itr = (begin_ + range.first);
            token_list_t::const_iterator end = (begin_ + range.second);
            T t;

            while (end != itr)
            {
               const range_t& curr_range = (*itr);
               if (string_to_type_converter(curr_range.first, curr_range.second,t))
                  sequence.push_back(t);
               else
                  return false;
               ++itr;
            }

            return true;
         }

         template <typename T,
                   typename Comparator,
                   typename Allocator>
         inline bool parse(const col_range_t& range,
                           std::set<T,Comparator,Allocator>& set) const
         {
            if (!validate_column_range(range))
               return false;

            token_list_t::const_iterator itr = (begin_ + range.first);
            token_list_t::const_iterator end = (begin_ + range.second);
            T t;

            while (end != itr)
            {
               const range_t& range = (*itr);
               if (string_to_type_converter(range.first,range.second,t))
                  set.insert(t);
               else
                  return false;
               ++itr;
            }

            return true;
         }

         template <typename T,
                   typename Comparator,
                   typename Allocator>
         inline bool parse(const col_range_t& range,
                           std::multiset<T,Comparator,Allocator>& multiset) const
         {
            if (!validate_column_range(range))
               return false;

            token_list_t::const_iterator itr = (begin_ + range.first);
            token_list_t::const_iterator end = (begin_ + range.second);
            T t;

            while (end != itr)
            {
               const range_t& range = (*itr);
               if (string_to_type_converter(range.first,range.second,t))
                  multiset.insert(t);
               else
                  return false;
               ++itr;
            }

            return true;
         }

         template <typename T, typename Container>
         inline bool parse(const col_range_t& range,
                           std::queue<T,Container>& queue) const
         {
            if (!validate_column_range(range))
               return false;

            token_list_t::const_iterator itr = (begin_ + range.first);
            token_list_t::const_iterator end = (begin_ + range.second);
            T t;

            while (end != itr)
            {
               const range_t& range = (*itr);
               if (string_to_type_converter(range.first,range.second,t))
                  queue.push(t);
               else
                  return false;
               ++itr;
            }

            return true;
         }

         template <typename T, typename Container>
         inline bool parse(const col_range_t& range,
                           std::stack<T,Container>& stack) const
         {
            if (!validate_column_range(range))
               return false;

            token_list_t::const_iterator itr = (begin_ + range.first);
            token_list_t::const_iterator end = (begin_ + range.second);
            T t;

            while (end != itr)
            {
               const range_t& range = (*itr);
               if (string_to_type_converter(range.first,range.second,t))
                  stack.push(t);
               else
                  return false;
               ++itr;
            }

            return true;
         }

         template <typename T,
                   typename Container,
                   typename Comparator>
         inline bool parse(const col_range_t& range,
                           std::priority_queue<T,Container,Comparator>& priority_queue) const
         {
            if (!validate_column_range(range))
               return false;

            token_list_t::const_iterator itr = (begin_ + range.first);
            token_list_t::const_iterator end = (begin_ + range.second);
            T t;

            while (end != itr)
            {
               const range_t& range = (*itr);
               if (string_to_type_converter(range.first,range.second,t))
                  priority_queue.push(t);
               else
                  return false;
               ++itr;
            }

            return true;
         }

         template <typename T,
                   typename Allocator,
                   template <typename,typename> class Sequence>
         inline bool parse(Sequence<T,Allocator>& sequence) const
         {
            return parse(range(0),sequence);
         }

         template <typename T,
                   typename Comparator,
                   typename Allocator>
         inline bool parse(std::set<T,Comparator,Allocator>& set) const
         {
            return parse(range(0),set);
         }

         template <typename T,
                   typename Comparator,
                   typename Allocator>
         inline bool parse(std::multiset<T,Comparator,Allocator>& multiset) const
         {
            return parse(range(0),multiset);
         }

         template <typename T, typename Container>
         inline bool parse(std::queue<T,Container>& queue) const
         {
            return parse(range(0),queue);
         }

         template <typename T, typename Container>
         inline bool parse(std::stack<T,Container>& stack) const
         {
            return parse(range(0),stack);
         }

         template <typename T,
                   typename Container,
                   typename Comparator>
         inline bool parse(std::priority_queue<T,Container,Comparator>& priority_queue) const
         {
            return parse(range(0),priority_queue);
         }

         template <typename T,
                   typename Allocator,
                   template <typename,typename> class Sequence>
         inline std::size_t parse_n(const std::size_t& n, Sequence<T,Allocator>& sequence) const
         {
            if (0 == n) return 0;

            T t;
            std::size_t count = 0;
            token_list_t::const_iterator itr = begin_;
            const token_list_t::const_iterator end = begin_ + size_;

            while (end != itr)
            {
               const range_t& range = (*itr);

               if (!string_to_type_converter(range.first,range.second,t))
                  return false;
               else
                  sequence.push_back(t);

               if (n == (++count))
                  break;
               ++itr;
            }

            return count;
         }

         template <typename T, typename OutputIterator>
         inline void parse_checked(OutputIterator out) const
         {
            T value;
            token_list_t::const_iterator itr = begin_;
            const token_list_t::const_iterator end = begin_ + size_;

            while (end != itr)
            {
               const range_t& range = (*itr);

               if (string_to_type_converter(range.first,range.second,value))
               {
                  *(out++) = value;
               }

               ++itr;
            }
         }

         template <typename T,
                   typename Allocator,
                   template <typename,typename> class Sequence>
         inline void parse_checked(Sequence<T,Allocator>& sequence) const
         {
            parse_checked<T>(std::back_inserter(sequence));
         }

         template <typename T,
                   typename Comparator,
                   typename Allocator>
         inline void parse_checked(std::set<T,Comparator,Allocator>& set) const
         {
            parse_checked<T>(std::inserter(set,set.end()));
         }

         template <typename T,
                   typename Comparator,
                   typename Allocator>
         inline void parse_checked(std::multiset<T,Comparator,Allocator>& multiset) const
         {
            parse_checked<T>(std::inserter(multiset,multiset.end()));
         }

         template <typename T, typename Container>
         inline void parse_checked(std::queue<T,Container>& queue) const
         {
            parse_checked<T>(push_inserter(queue));
         }

         template <typename T, typename Container>
         inline void parse_checked(std::stack<T,Container>& stack) const
         {
            parse_checked<T>(push_inserter(stack));
         }

         template <typename T,
                   typename Container,
                   typename Comparator>
         inline void parse_checked(std::priority_queue<T,Container,Comparator>& priority_queue) const
         {
            parse_checked<T>(push_inserter(priority_queue));
         }

         template <typename Function>
         inline std::size_t for_each_column(const col_range_t& range, Function f) const
         {
            if (!validate_column_range(range))
               return false;

            token_list_t::const_iterator itr = begin_ + range.first;
            token_list_t::const_iterator end = begin_ + range.second;
            std::size_t col_count = 0;

            while (end != itr)
            {
               const range_t& range = (*itr);
               f(range);
               ++itr;
               ++col_count;
            }

            return col_count;
         }

         template <typename Function>
         inline std::size_t for_each_column(Function f) const
         {
            return for_each_column(all_columns(),f);
         }

      private:

         template <typename T>
         inline bool process(const range_t& range, T& t) const
         {
            return string_to_type_converter(range.first,range.second,t);
         }

      private:

         std::size_t index_;
         std::size_t size_;
         token_list_t::const_iterator begin_;
      };

      token_grid()
      : file_name_(""),
        buffer_(0),
        buffer_size_(0),
        min_column_count_(0),
        max_column_count_(0),
        load_from_file_(false),
        state_(false)
      {}

      token_grid(const std::string& file_name,
                 const token_grid::options& options)
      : file_name_(file_name),
        buffer_(0),
        buffer_size_(0),
        min_column_count_(0),
        max_column_count_(0),
        options_(options),
        load_from_file_(true),
        state_(load())
      {}

      token_grid(const unsigned char* input_buffer,
                 const std::size_t& input_buffer_size,
                 const token_grid::options& options)
      : file_name_(""),
        buffer_(const_cast<unsigned char*>(input_buffer)),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        options_(options),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const char* input_buffer,
                 const std::size_t& input_buffer_size,
                 const token_grid::options& options)
      : file_name_(""),
        buffer_(reinterpret_cast<unsigned char*>(const_cast<char*>(input_buffer))),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        options_(options),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const std::string& input_buffer,
                 const std::size_t& input_buffer_size,
                 const token_grid::options& options)
      : file_name_(""),
        buffer_(reinterpret_cast<unsigned char*>(const_cast<char*>(to_ptr(input_buffer)))),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        options_(options),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const std::string& file_name,
                 const std::string& column_delimiters = ",|;\t",
                 const std::string& row_delimiters = "\n\r")
      : file_name_(file_name),
        buffer_(0),
        buffer_size_(0),
        min_column_count_(0),
        max_column_count_(0),
        options_(split_options::compress_delimiters,
                 split_options::compress_delimiters,
                 row_delimiters,
                 column_delimiters),
        load_from_file_(true),
        state_(load())
      {}

      token_grid(const unsigned char* input_buffer,
                 const std::size_t& input_buffer_size,
                 const std::string& column_delimiters = ",|;\t",
                 const std::string& row_delimiters = "\n\r")
      : file_name_(""),
        buffer_(const_cast<unsigned char*>(input_buffer)),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        options_(split_options::compress_delimiters,
                 split_options::compress_delimiters,
                 row_delimiters,
                 column_delimiters),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const char* input_buffer,
                 const std::size_t& input_buffer_size,
                 const std::string& column_delimiters = ",|;\t",
                 const std::string& row_delimiters = "\n\r")
      : file_name_(""),
        buffer_(reinterpret_cast<unsigned char*>(const_cast<char*>(input_buffer))),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        options_(split_options::compress_delimiters,
                 split_options::compress_delimiters,
                 row_delimiters,
                 column_delimiters),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const std::string& input_buffer,
                 const std::size_t& input_buffer_size,
                 const std::string& column_delimiters = ",;|\t ",
                 const std::string& row_delimiters = "\n\r")
      : file_name_(""),
        buffer_(reinterpret_cast<unsigned char*>(const_cast<char*>(to_ptr(input_buffer)))),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        options_(split_options::compress_delimiters,
                 split_options::compress_delimiters,
                 row_delimiters,
                 column_delimiters),
        load_from_file_(false),
        state_(load())
      {}

     ~token_grid()
      {
         if ((load_from_file_) && (0 != buffer_))
         {
            delete [] buffer_;
            buffer_ = 0;
         }
      }

      inline bool operator!() const
      {
         return !state_;
      }

      inline std::string source_file() const
      {
         return file_name_;
      }

      inline std::size_t row_count() const
      {
         return dsv_index_.row_index.size();
      }

      inline std::size_t min_column_count() const
      {
         return min_column_count_;
      }

      inline std::size_t max_column_count() const
      {
         return max_column_count_;
      }

      inline range_t token(const std::size_t& row, const std::size_t& col) const
      {
         return dsv_index_(col,row);
      }

      template <typename T>
      inline T get(const std::size_t& row, const std::size_t& col)
      {
         range_t r = token(row,col);
         return string_to_type_converter<T>(r.first,r.second);
      }

      inline row_type row(const std::size_t& row_index) const
      {
         return row_type(row_index,dsv_index_);
      }

      inline row_range_t all_rows() const
      {
         return row_range_t(0,static_cast<index_t>(dsv_index_.row_index.size()));
      }

      template <typename OutputIterator>
      inline bool extract_column_checked(const row_range_t& row_range,
                                         const std::size_t& index,
                                         OutputIterator out) const
      {
         if (index > max_column_count_)
            return false;
         else if (row_range_invalid(row_range))
            return false;

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            const row_index_range_t& row = dsv_index_.row_index[i];

            if (index < dsv_index_.token_count(row))
            {
               dsv_index_.token_list.begin() + (row.first + index);
               process_token_checked(*(dsv_index_.token_list.begin() + (row.first + index)),out);
            }
         }

         return true;
      }

      template <typename OutputIterator>
      inline bool extract_column_checked(const std::size_t& index,
                                         OutputIterator out) const
      {
         return extract_column_checked(all_rows(),index,out);
      }

      template <typename T,
                typename Allocator,
                template <typename,typename> class Sequence>
      inline void extract_column_checked(const std::size_t& index,
                                          Sequence<T,Allocator>& sequence) const
      {
         extract_column_checked(index,back_inserter_with_valuetype(sequence));
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline void extract_column_checked(const std::size_t& index,
                                         std::set<T,Comparator,Allocator>& set) const
      {
         extract_column_checked(index,inserter_with_valuetype(set));
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline void extract_column_checked(const std::size_t& index,
                                         std::multiset<T,Comparator,Allocator>& multiset) const
      {
         extract_column_checked(index,inserter_with_valuetype(multiset));
      }

      template <typename OutputIterator>
      inline bool extract_column(const row_range_t& row_range,
                                 const std::size_t& index,
                                 OutputIterator out) const
      {

         if (index > max_column_count_)
            return false;
         else if (row_range_invalid(row_range))
            return false;

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            const row_index_range_t& row = dsv_index_.row_index[i];

            if (index < dsv_index_.token_count(row))
            {
               process_token(*(dsv_index_.token_list.begin() + (row.first + index)),out);
            }
         }

         return true;
      }

      template <typename OutputIterator>
      inline bool extract_column(const std::size_t& index,
                                 OutputIterator out) const
      {
         return extract_column(all_rows(),index,out);
      }

      template <typename OutputIterator0, typename OutputIterator1>
      inline bool extract_column(const row_range_t& row_range,
                                 const std::size_t& index0,
                                 const std::size_t& index1,
                                 OutputIterator0 out0,
                                 OutputIterator1 out1) const
      {
         if (
              (index0 > max_column_count_) ||
              (index1 > max_column_count_)
            )
            return false;
         else if (row_range_invalid(row_range))
            return false;

         std::size_t max_index = std::max(index0,index1);

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            const row_index_range_t& row = dsv_index_.row_index[i];

            if (max_index < dsv_index_.token_count(row))
            {
               process_token(*(dsv_index_.token_list.begin() + (row.first + index0)),out0);
               process_token(*(dsv_index_.token_list.begin() + (row.first + index1)),out1);
            }
         }

         return true;
      }

      template <typename OutputIterator0, typename OutputIterator1, typename OutputIterator2>
      inline bool extract_column(const row_range_t& row_range,
                                 const std::size_t& index0,
                                 const std::size_t& index1,
                                 const std::size_t& index2,
                                 OutputIterator0 out0,
                                 OutputIterator1 out1,
                                 OutputIterator2 out2) const
      {
         if (
              (index0 > max_column_count_) ||
              (index1 > max_column_count_) ||
              (index2 > max_column_count_)
            )
            return false;
         else if (row_range_invalid(row_range))
            return false;

         std::size_t max_index = std::max(index0,std::max(index1,index2));

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            const row_index_range_t& row = dsv_index_.row_index[i];

            if (max_index < dsv_index_.token_count(row))
            {
               process_token(*(dsv_index_.token_list.begin() + (row.first + index0)),out0);
               process_token(*(dsv_index_.token_list.begin() + (row.first + index1)),out1);
               process_token(*(dsv_index_.token_list.begin() + (row.first + index2)),out2);
            }
         }

         return true;
      }

      template <typename OutputIterator0, typename OutputIterator1,
                typename OutputIterator2, typename OutputIterator3>
      inline bool extract_column(const row_range_t& row_range,
                                 const std::size_t& index0,
                                 const std::size_t& index1,
                                 const std::size_t& index2,
                                 const std::size_t& index3,
                                 OutputIterator0 out0,
                                 OutputIterator1 out1,
                                 OutputIterator2 out2,
                                 OutputIterator3 out3) const
      {
         if (
              (index0 > max_column_count_) ||
              (index1 > max_column_count_) ||
              (index2 > max_column_count_) ||
              (index3 > max_column_count_)
            )
            return false;
         else if (row_range_invalid(row_range))
            return false;

         std::size_t max_index = std::max(std::max(index0,index1),std::max(index2,index3));

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            const row_index_range_t& row = dsv_index_.row_index[i];

            if (max_index < dsv_index_.token_count(row))
            {
               process_token(*(dsv_index_.token_list.begin() + (row.first + index0)),out0);
               process_token(*(dsv_index_.token_list.begin() + (row.first + index1)),out1);
               process_token(*(dsv_index_.token_list.begin() + (row.first + index2)),out2);
               process_token(*(dsv_index_.token_list.begin() + (row.first + index3)),out3);
            }
         }

         return true;
      }

      template <typename OutputIterator0, typename OutputIterator1,
                typename OutputIterator2, typename OutputIterator3,
                typename OutputIterator4>
      inline bool extract_column(const row_range_t& row_range,
                                 const std::size_t& index0,
                                 const std::size_t& index1,
                                 const std::size_t& index2,
                                 const std::size_t& index3,
                                 const std::size_t& index4,
                                 OutputIterator0 out0,
                                 OutputIterator1 out1,
                                 OutputIterator2 out2,
                                 OutputIterator3 out3,
                                 OutputIterator4 out4) const
      {
         if (
              (index0 > max_column_count_) ||
              (index1 > max_column_count_) ||
              (index2 > max_column_count_) ||
              (index3 > max_column_count_) ||
              (index4 > max_column_count_)
            )
            return false;
         else if (row_range_invalid(row_range))
            return false;

         std::size_t max_index = std::max(index4,std::max(std::max(index0,index1),std::max(index2,index3)));

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            const row_index_range_t& row = dsv_index_.row_index[i];

            if (max_index < dsv_index_.token_count(row))
            {
               process_token(*(dsv_index_.token_list.begin() + (row.first + index0)),out0);
               process_token(*(dsv_index_.token_list.begin() + (row.first + index1)),out1);
               process_token(*(dsv_index_.token_list.begin() + (row.first + index2)),out2);
               process_token(*(dsv_index_.token_list.begin() + (row.first + index3)),out3);
               process_token(*(dsv_index_.token_list.begin() + (row.first + index4)),out4);
            }
         }

         return true;
      }

      inline void remove_row(const std::size_t& index)
      {
         if (index < dsv_index_.row_index.size())
         {
            dsv_index_.remove_row(index);
         }
      }

      template <typename Predicate>
      inline bool remove_row_if(const row_range_t& row_range, Predicate predicate)
      {
         if (row_range_invalid(row_range))
            return false;

         std::size_t removed_token_count = 0;

         std::deque<std::size_t> remove_token_list;
         std::deque<std::size_t> remove_row_list;

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            row_index_range_t& r = dsv_index_.row_index[i];
            std::size_t temp_r_first = r.first - removed_token_count;
            row_type row(i,dsv_index_);

            if (predicate(row))
            {
               remove_row_list.push_back(i);

               for (std::size_t j = r.first; j <= r.second; ++j)
               {
                  remove_token_list.push_back(j);
               }

               removed_token_count += row.size();
            }

            r.first   = static_cast<unsigned int>(temp_r_first);
            r.second -= static_cast<unsigned int>(removed_token_count);
         }

         for (std::size_t i = row_range.second; i < dsv_index_.row_index.size(); ++i)
         {
            row_index_range_t& r = dsv_index_.row_index[i];

            r.first  -= static_cast<unsigned int>(removed_token_count);
            r.second -= static_cast<unsigned int>(removed_token_count);
         }

         if (!remove_row_list.empty())
         {
            remove_inplace(index_remover(remove_row_list),dsv_index_.row_index);
         }

         if (!remove_token_list.empty())
         {
            remove_inplace(index_remover(remove_token_list),dsv_index_.token_list);
         }

         return true;
      }

      template <typename Predicate>
      inline bool remove_row_if(Predicate predicate)
      {
         return remove_row_if(all_rows(),predicate);
      }

      template <typename Predicate>
      inline std::size_t remove_token_if(const row_range_t& row_range, Predicate predicate)
      {
         if (row_range_invalid(row_range))
            return 0;

         std::size_t removed_token_count = 0;
         std::deque<std::size_t> remove_token_list;
         std::deque<std::size_t> remove_row_list;

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            row_index_range_t& r = dsv_index_.row_index[i];
            std::size_t temp_r_first = r.first - removed_token_count;
            row_type row(i,dsv_index_);

            for (std::size_t j = 0; j < row.size(); ++j)
            {
               if (predicate(row.token(j)))
               {
                  remove_token_list.push_back(r.first + j);
                  ++removed_token_count;
               }
            }

            r.first   = static_cast<unsigned int>(temp_r_first);
            r.second -= static_cast<unsigned int>(removed_token_count);

            if (0 == dsv_index_.token_count(r))
            {
               remove_row_list.push_back(i);
            }
         }

         for (std::size_t i = row_range.second; i < dsv_index_.row_index.size(); ++i)
         {
            row_index_range_t& r = dsv_index_.row_index[i];
            r.first  -= static_cast<unsigned int>(removed_token_count);
            r.second -= static_cast<unsigned int>(removed_token_count);
         }

         if (!remove_row_list.empty())
         {
            remove_inplace(index_remover(remove_row_list),dsv_index_.row_index);
         }

         if (!remove_token_list.empty())
         {
            remove_inplace(index_remover(remove_token_list),dsv_index_.token_list);
         }

         if (!remove_token_list.empty())
         {
            update_minmax_columns();
         }

         return remove_token_list.size();
      }

      inline std::size_t remove_empty_tokens(const row_range_t& range)
      {
         return remove_token_if(range,is_empty_token());
      }

      inline std::size_t remove_empty_tokens()
      {
         return remove_empty_tokens(all_rows());
      }

      inline void enforce_column_count(const row_range_t& row_range,
                                       const std::size_t& column_count)
      {
         if (row_range_invalid(row_range))
            return;

         remove_row_if(insufficient_number_of_columns(column_count));

         min_column_count_ = column_count;
         max_column_count_ = column_count;
      }

      inline void enforce_column_count(const std::size_t& column_count)
      {
         enforce_column_count(all_rows(),column_count);
      }

      inline void enforce_min_max_column_count(const row_range_t& row_range,
                                               const std::size_t& min_column_count,
                                               const std::size_t& max_column_count)
      {
         if (row_range_invalid(row_range))
            return;

         remove_row_if(insufficient_number_of_minmax_columns(min_column_count,max_column_count));

         min_column_count_ = min_column_count;
         max_column_count_ = max_column_count;
      }

      inline void enforce_min_max_column_count(const std::size_t& min_column_count,
                                               const std::size_t& max_column_count)
      {
         enforce_min_max_column_count(all_rows(),min_column_count,max_column_count);
      }

      inline void clear(const bool force_delete_buffer = false)
      {
         if (load_from_file_ || force_delete_buffer)
            delete[] buffer_;

         buffer_           = 0;
         buffer_size_      = 0;
         min_column_count_ = 0;
         max_column_count_ = 0;
         state_            = false;
         file_name_        = "";
         dsv_index_.clear();
      }

      inline std::size_t column_width(const std::size_t& col,
                                      const row_range_t& row_range) const
      {
         if (col > max_column_count_)
            return 0;
         else if (row_range_invalid(row_range))
            return 0;

         std::size_t result = 0;

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            const row_index_range_t& r = dsv_index_.row_index[i];

            if (col < dsv_index_.token_count(r))
            {
               const range_t& range = *(dsv_index_.token_list.begin() + r.first + col);

               result = std::max<std::size_t>(std::distance(range.first,range.second),result);
            }
         }

         return result;
      }

      inline std::size_t column_width(const std::size_t& col) const
      {
         return column_width(col,all_rows());
      }

      template <typename Allocator,
                template <typename,typename> class Sequence>
      inline void get_column_widths(Sequence<std::size_t,Allocator>& columns)
      {
         for (std::size_t c = 0; c < max_column_count(); ++c)
         {
            columns.push_back(column_width(c));
         }
      }

      template <typename Allocator,
                template <typename,typename> class Sequence>
      inline void get_column_widths(Sequence<std::pair<std::size_t,std::size_t>,Allocator>& columns)
      {
         for (std::size_t c = 0; c < max_column_count(); ++c)
         {
            columns.push_back(std::make_pair(c,column_width(c)));
         }
      }

      template <typename T>
      inline std::size_t accumulate_row(const std::size_t& row, T& result) const
      {
         if (row >= dsv_index_.row_index.size())
            return 0;

         const row_index_range_t& r = dsv_index_.row_index[row];

         token_list_t::const_iterator itr = dsv_index_.token_list.begin() + r.first;
         token_list_t::const_iterator end = dsv_index_.token_list.begin() + r.second + 1;

         std::size_t process_count = 0;
         T current_value = T();

         while (end != itr)
         {
            if (string_to_type_converter((*itr).first,(*itr).second,current_value))
            {
              result += current_value;
              ++process_count;
            }
            else
               return 0;

            ++itr;
         }

         return process_count;
      }

      template <typename T>
      inline std::size_t accumulate_column(const std::size_t& col,
                                           const row_range_t& row_range,
                                           T& result) const
      {
         if (col > max_column_count_)
            return 0;
         else if (row_range_invalid(row_range))
            return 0;

         std::size_t process_count = 0;
         T current_value = T();

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            const row_index_range_t& r = dsv_index_.row_index[i];

            if (col < dsv_index_.token_count(r))
            {
               const range_t& range = *(dsv_index_.token_list.begin() + r.first + col);

               if (string_to_type_converter(range.first,range.second,current_value))
                  result += current_value;
               else
                  return 0;
            }

            ++process_count;
         }

         return process_count;
      }

      template <typename T>
      inline std::size_t accumulate_column(const std::size_t& col, T& result) const
      {
         return accumulate_column(col,all_rows(),result);
      }

      template <typename T, typename Predicate>
      inline std::size_t accumulate_column(const std::size_t& col,
                                           const row_range_t& row_range,
                                           Predicate p,
                                           T& result) const
      {
         if (col > max_column_count_)
            return 0;
         else if (row_range_invalid(row_range))
            return 0;

         std::size_t process_count = 0;
         T current_value = T();

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            const row_index_range_t& r = dsv_index_.row_index[i];

            if (col < dsv_index_.token_count(r))
            {
               row_type row = row_type(i,dsv_index_);

               if (p(row))
               {
                  const range_t& range = row.token(col);

                  if (string_to_type_converter(range.first,range.second,current_value))
                  {
                     result += current_value;
                     ++process_count;
                  }
                  else
                     return 0;
               }
            }
         }

         return process_count;
      }

      template <typename T, typename Predicate>
      inline std::size_t accumulate_column(const std::size_t& col,
                                           Predicate p,
                                           T& result) const
      {
         return accumulate_column(col,all_rows(),p,result);
      }

      inline bool join_row(const std::size_t& row,
                           const std::string& delimiter,
                           std::string& result)
      {
         if (row >= dsv_index_.row_index.size())
            return false;

         const row_index_range_t& r = dsv_index_.row_index[row];

         token_list_t::const_iterator itr = dsv_index_.token_list.begin() + r.first;
         token_list_t::const_iterator end = dsv_index_.token_list.begin() + r.second + (row < (dsv_index_.row_index.size() - 1) ? 1 : 0);

         result.reserve(delimiter.size() * dsv_index_.token_count(r) + std::distance(itr->first,end->second));

         bool appended = false;

         while (end != itr)
         {
            if (!delimiter.empty() && appended)
               result.append(delimiter);

            appended = false;

            if ((*itr).first != (*itr).second)
            {
               result.append((*itr).first,(*itr).second);
               appended = true;
            }

            ++itr;
         }

         return true;
      }

      template <typename Predicate>
      inline bool join_row(const std::size_t& row,
                           Predicate predicate,
                           const std::string& delimiter,
                           std::string& result)
      {
         if (row >= dsv_index_.row_index.size())
            return false;

         const row_index_range_t& r = dsv_index_.row_index[row];

         token_list_t::const_iterator itr = (dsv_index_.token_list.begin() + r.first);
         token_list_t::const_iterator end = dsv_index_.token_list.begin() + r.second + (row < (dsv_index_.row_index.size() - 1) ? 1 : 0);

         result.reserve(delimiter.size() * dsv_index_.token_count(r) + std::distance(itr->first,end->second));

         bool appended = false;

         while (end != itr)
         {
            if (!delimiter.empty() && appended)
               result.append(delimiter);

            appended = false;

            if ((*itr).first != (*itr).second)
            {
               if (predicate(*itr))
               {
                  result.append((*itr).first,(*itr).second);
                  appended = true;
               }
            }

            ++itr;
         }

         return true;
      }

      template <typename Predicate>
      inline bool join_row(const std::size_t& row,
                           Predicate predicate,
                           const char* delimiter,
                           std::string& result)
      {
         return join_row(row,predicate,std::string(delimiter),result);
      }

      inline bool join_column(const std::size_t& col,
                              const row_range_t& row_range,
                              const std::string& delimiter,
                              std::string& result) const
      {
         if (col > max_column_count_)
            return false;
         else if (row_range_invalid(row_range))
            return false;

         bool appended = false;

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            const row_index_range_t& r = dsv_index_.row_index[i];

            if (col < dsv_index_.token_count(r))
            {
               row_type row = row_type(i,dsv_index_);

               const range_t& range = row.token(col);

               if (!delimiter.empty() && appended)
                  result.append(delimiter);

               appended = false;

               if (range.first != range.second)
               {
                  result.append(range.first,range.second);
                  appended = true;
               }
            }
         }

         return true;
      }

      inline bool join_column(const std::size_t& col,
                              const std::string& delimiter,
                              std::string& result) const
      {
         return join_column(col,all_rows(),delimiter,result);
      }

      template <typename Predicate>
      inline bool join_column(const std::size_t& col,
                              const row_range_t& row_range,
                              Predicate predicate,
                              const std::string& delimiter,
                              std::string& result) const
      {
         if (col > max_column_count_)
            return false;
         else if (row_range_invalid(row_range))
            return false;

         bool appended = false;

         const std::size_t pre_end_index = row_range.second - 1;

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            const row_index_range_t& r = dsv_index_.row_index[i];

            if (col < dsv_index_.token_count(r))
            {
               row_type row = row_type(i,dsv_index_);
               const range_t& range = row.token(col);

               if (!delimiter.empty() && appended && (pre_end_index != i))
                  result.append(delimiter);

               appended = false;

               if (range.first != range.second)
               {
                  if (predicate(row))
                  {
                     result.append(range.first,range.second);
                     appended = true;
                  }
               }
            }
         }

         return true;
      }

      template <typename Predicate>
      inline bool join_column(const std::size_t& col,
                              Predicate p,
                              const std::string& delimiter,
                              std::string& result) const
      {
         return join_column(col,all_rows(),p,delimiter,result);
      }

      template <typename TransitionPredicate, typename Function>
      inline bool sequential_partition(const row_range_t& row_range,
                                       TransitionPredicate p,
                                       Function f)
      {
         if (row_range_invalid(row_range))
            return false;

         row_range_t r(row_range.first,row_range.first);

         for (row_range_t::first_type i = row_range.first; i < row_range.second; ++i)
         {
            if (p(row_type(i,dsv_index_)))
            {
               if (r.first != r.second)
               {
                  r.second = i;
                  if (!f(*this,r))
                     return false;
               }

               r.first = r.second;
            }
            else
               r.second = i;
         }

         if (r.first != row_range.second)
         {
            r.second = row_range.second;
            if (!f(*this,r))
               return false;
         }

         return true;
      }

      template <typename TransitionPredicate, typename Function>
      inline bool sequential_partition(TransitionPredicate p, Function f)
      {
         return sequential_partition(all_rows(),p,f);
      }

      static inline token_grid::options default_options()
      {
         return options();
      }

      template <typename Function>
      inline std::size_t for_each_row(const row_range_t& row_range, Function f) const
      {
         if (row_range_invalid(row_range))
            return 0;

         std::size_t row_count = 0;

         for (std::size_t i = row_range.first; i < row_range.second; ++i)
         {
            f(row_type(i,dsv_index_));
            ++row_count;
         }

         return row_count;
      }

      template <typename Function>
      inline std::size_t for_each_row(Function f) const
      {
         return for_each_row(all_rows(),f);
      }

      bool load(const std::string& file_name,
                const token_grid::options& options)
      {
         file_name_ = file_name;

         if ((load_from_file_) && (0 != buffer_))
         {
            delete [] buffer_;
            buffer_ = 0;
         }

         buffer_size_      = 0;
         min_column_count_ = 0;
         max_column_count_ = 0;
         options_          = options;
         load_from_file_   = true;
         state_            = load();

         if (state_)
            return true;
         else
         {
            file_name_ = "";

            if (load_from_file_ && buffer_)
            {
               delete [] buffer_;
               buffer_ = 0;
            }

            return false;
         }
      }

      bool load(unsigned char* buffer,
                const std::size_t buffer_size,
                const token_grid::options& options)
      {
         file_name_ = "";

         if (load_from_file_ && buffer_)
         {
            delete [] buffer_;
            buffer_ = 0;
         }

         min_column_count_ = 0;
         max_column_count_ = 0;
         options_          = options;
         load_from_file_   = false;
         buffer_           = buffer;
         buffer_size_      = buffer_size;
         state_            = load();

         if (state_)
            return true;
         else
         {
            file_name_ = "";

            if (load_from_file_ && buffer_)
            {
               delete [] buffer_;
               buffer_ = 0;
            }

            return false;
         }
      }

   private:

      token_grid(const token_grid& tg);
      token_grid& operator=(const token_grid& tg);

      struct is_empty_token
      {
         inline bool operator()(const range_t& r) const
         {
            return r.first == r.second;
         }
      };

      struct insufficient_number_of_columns
      {
         insufficient_number_of_columns(const std::size_t& noc)
         : num_of_cols(noc)
         {}

         inline bool operator()(const row_type& row) const
         {
            return (num_of_cols != row.size());
         }

         std::size_t num_of_cols;
      };

      struct insufficient_number_of_minmax_columns
      {
         insufficient_number_of_minmax_columns(const std::size_t& min_col, const std::size_t& max_col)
         : min_column_count(min_col),
           max_column_count(max_col)
         {}

         inline bool operator()(const row_type& row) const
         {
            return (row.size() < min_column_count) || (max_column_count < row.size());
         }

         std::size_t min_column_count;
         std::size_t max_column_count;
      };

      class double_quotes_predicate
      {
      public:

         double_quotes_predicate(const std::string& delimiters)
         : in_bracket_range_(false),
           mdp_(delimiters)
         {}

         inline bool operator()(const unsigned char c) const
         {
            if ('"' == c)
            {
               in_bracket_range_ = !in_bracket_range_;
               return false;
            }
            else if (in_bracket_range_)
               return false;
            else
               return mdp_(c);
         }

         inline void reset()
         {
            in_bracket_range_ = false;
         }

      private:

         mutable bool in_bracket_range_;
         mutable strtk::multiple_char_delimiter_predicate mdp_;
      };

      inline bool load()
      {
         if (load_from_file_ && !load_buffer_from_file())
            return false;

         dsv_index_.token_list.clear();
         dsv_index_.row_index.clear();

         multiple_char_delimiter_predicate text_newline_predicate(options_.row_delimiters);

         if (!options_.support_dquotes)
         {
            multiple_char_delimiter_predicate token_predicate(options_.column_delimiters);

            strtk::split(text_newline_predicate,
                         buffer_, buffer_ + buffer_size_,
                         strtk::functional_inserter(
                            row_processor<multiple_char_delimiter_predicate>
                              (dsv_index_,token_predicate,options_.column_split_option)),
                         strtk::split_options::compress_delimiters);
         }
         else
         {
            double_quotes_predicate token_predicate_dblq(options_.column_delimiters);

            strtk::split(text_newline_predicate,
                         buffer_, buffer_ + buffer_size_,
                         strtk::functional_inserter(
                            row_processor<double_quotes_predicate>
                              (dsv_index_,token_predicate_dblq,options_.column_split_option)),
                        strtk::split_options::compress_delimiters);

            if (options_.trim_dquotes)
            {
               for (std::size_t i = 0; i < dsv_index_.token_list.size(); ++i)
               {
                  if (
                      ((*(dsv_index_.token_list[i].first     )) == '"') &&
                      ((*(dsv_index_.token_list[i].second - 1)) == '"')
                     )
                  {
                      ++dsv_index_.token_list[i].first;
                      --dsv_index_.token_list[i].second;
                  }
               }
            }
         }

         update_minmax_columns();

         return true;
      }

      inline bool load_buffer_from_file()
      {
         std::ifstream stream(file_name_.c_str(),std::ios::binary);

         if (!stream)
            return false;

         stream.seekg (0,std::ios::end);
         buffer_size_ = static_cast<std::size_t>(stream.tellg());

         if (0 == buffer_size_)
            return false;

         stream.seekg (0,std::ios::beg);
         buffer_ = new unsigned char[buffer_size_];
         stream.read(reinterpret_cast<char*>(buffer_),static_cast<std::streamsize>(buffer_size_));
         stream.close();

         return true;
      }

      template <typename OutputIterator>
      inline void process_token(const range_t& range, OutputIterator out) const
      {
         typedef typename std::iterator_traits<OutputIterator>::value_type output_type;
         (*out) = string_to_type_converter<output_type>(range.first,range.second);
         ++out;
      }

      template <typename OutputIterator>
      inline void process_token_checked(const range_t& range, OutputIterator out) const
      {
         typedef typename std::iterator_traits<OutputIterator>::value_type output_type;

         output_type value;

         if (string_to_type_converter(range.first,range.second,value))
         {
            (*out) = value;
            ++out;
         }
      }

      inline bool row_range_invalid(const row_range_t& row_range) const
      {
         if (row_range.first > dsv_index_.row_index.size())
            return true;
         else if (row_range.second > dsv_index_.row_index.size())
            return true;
         else if (row_range.first > row_range.second)
            return true;
         else
            return false;
      }

      inline void update_minmax_columns()
      {
         min_column_count_ = std::numeric_limits<std::size_t>::max();
         max_column_count_ = std::numeric_limits<std::size_t>::min();

         for (std::size_t i = 0; i < dsv_index_.row_index.size(); ++i)
         {
            const row_index_range_t& r = dsv_index_.row_index[i];
            const std::size_t number_of_tokens = dsv_index_.token_count(r);

            if (number_of_tokens > max_column_count_)
               max_column_count_ = number_of_tokens;
            if (number_of_tokens < min_column_count_)
               min_column_count_ = number_of_tokens;
         }
      }

   private:

      store dsv_index_;
      std::string file_name_;
      unsigned char* buffer_;
      std::size_t buffer_size_;
      std::size_t min_column_count_;
      std::size_t max_column_count_;
      options options_;
      bool load_from_file_;
      bool state_;
   };