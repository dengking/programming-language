   namespace std_string
   {
      template <typename DelimiterPredicate = single_delimiter_predicate<std::string::value_type> >
      struct tokenizer
      {
         typedef DelimiterPredicate predicate_type;
         typedef const std::string::value_type* string_iterator_type;
         typedef strtk::tokenizer<string_iterator_type,DelimiterPredicate> type;
         typedef strtk::tokenizer<string_iterator_type,multiple_char_delimiter_predicate> md_type;
         typedef std::pair<string_iterator_type,string_iterator_type> iterator_type;
      };

      typedef tokenizer<>::iterator_type iterator_type;
      typedef tokenizer<>::iterator_type range_t;

      typedef std::vector<iterator_type> token_vector_type;
      typedef std::deque<iterator_type> token_deque_type;
      typedef std::list<iterator_type> token_list_type;

   } // namespace std_string
