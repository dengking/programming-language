   namespace details
   {
      class container_adder
      {
      private:

         class container_adder_base
         {
         public:

            typedef const char* itr_type;

            virtual ~container_adder_base(){}

            template <typename InputIterator>
            inline bool add(const InputIterator begin, const InputIterator end) const
            {
               return add_impl(begin,end);
            }

            template <typename InputIterator>
            inline bool add(const std::pair<InputIterator,InputIterator>& range) const
            {
               return add(range.first,range.second);
            }

         protected:

            virtual bool add_impl(const itr_type begin, const itr_type end) const = 0;

         };

         template <typename T,
                   typename Allocator,
                   template <typename,typename> class Sequence>
         class sequence_adder_impl : public container_adder_base
         {
         public:

            typedef Sequence<T,Allocator> sequence_t;

            sequence_adder_impl(sequence_t& seq)
            : sequence_(seq)
            {}

         protected:

            bool add_impl(const itr_type begin, const itr_type end) const
            {
               T t;
               if (!string_to_type_converter(begin, end, t)) return false;
               sequence_.push_back(t);
               return true;
            }

         private:

            sequence_adder_impl operator=(const sequence_adder_impl&);

            sequence_t& sequence_;
         };

         template <typename T,
                   typename Comparator,
                   typename Allocator,
                   template <typename,typename,typename> class Set>
         class set_adder_impl : public container_adder_base
         {
         public:

            set_adder_impl(Set<T,Comparator,Allocator>& set)
            : set_(set)
            {}

            bool add_impl(const itr_type begin, const itr_type end) const
            {
               T t;
               if (!string_to_type_converter(begin, end, t)) return false;
               set_.insert(t);
               return true;
            }

         private:

            set_adder_impl operator=(const set_adder_impl&);

            Set<T,Comparator,Allocator>& set_;
         };

         template <typename T,
                   typename Container,
                   typename Comparator>
         class pq_adder_impl : public container_adder_base
         {
         public:

            pq_adder_impl(std::priority_queue<T,Container,Comparator>& pq)
            : pq_(pq)
            {}

            bool add_impl(const itr_type begin, const itr_type end) const
            {
               T t;
               if (!string_to_type_converter(begin, end, t)) return false;
               pq_.push(t);
               return true;
            }

         private:

            pq_adder_impl operator=(const pq_adder_impl&);

            std::priority_queue<T,Container,Comparator>& pq_;
         };

         template <typename T,
                   typename Container,
                   template <typename,typename> class SContainer>
         class stack_queue_adder_impl : public container_adder_base
         {
         public:

            stack_queue_adder_impl(SContainer<T,Container>& container)
            : container_(container)
            {}

            bool add_impl(const itr_type begin, const itr_type end) const
            {
               T t;
               if (!string_to_type_converter(begin, end, t)) return false;
               container_.push(t);
               return true;
            }

         private:

            stack_queue_adder_impl operator=(const stack_queue_adder_impl&);

            SContainer<T,Container>& container_;
         };

      public:

         template <typename T, typename Allocator>
         container_adder(std::vector<T,Allocator>& vec)
         : container_adder_base_(new(buffer)sequence_adder_impl<T,Allocator,std::vector>(vec))
         {}

         template <typename T, typename Allocator>
         container_adder(std::deque<T,Allocator>& deq)
         : container_adder_base_(new(buffer)sequence_adder_impl<T,Allocator,std::deque>(deq))
         {}

         template <typename T, typename Allocator>
         container_adder(std::list<T,Allocator>& list)
         : container_adder_base_(new(buffer)sequence_adder_impl<T,Allocator,std::list>(list))
         {}

         template <typename T, typename Comparator, typename Allocator>
         container_adder(std::set<T,Comparator,Allocator>& set)
         : container_adder_base_(new(buffer)set_adder_impl<T,Comparator,Allocator,std::set>(set))
         {}

         template <typename T, typename Comparator, typename Allocator>
         container_adder(std::multiset<T,Comparator,Allocator>& multiset)
         : container_adder_base_(new(buffer)set_adder_impl<T,Comparator,Allocator,std::multiset>(multiset))
         {}

         template <typename T, typename Container, typename Comparator>
         container_adder(std::priority_queue<T,Container,Comparator>& pq)
         : container_adder_base_(new(buffer)pq_adder_impl<T,Container,Comparator>(pq))
         {}

         template <typename T, typename Container>
         container_adder(std::queue<T,Container>& queue)
         : container_adder_base_(new(buffer)stack_queue_adder_impl<T,Container,std::queue>(queue))
         {}

         template <typename T, typename Container>
         container_adder(std::stack<T,Container>& stack)
         : container_adder_base_(new(buffer)stack_queue_adder_impl<T,Container,std::stack>(stack))
         {}

         template <typename InputIterator>
         inline bool add(InputIterator begin, InputIterator end) const
         {
            return container_adder_base_->add(begin,end);
         }

         template <typename InputIterator>
         inline bool add(std::pair<InputIterator,InputIterator>& range) const
         {
            return add(range.first,range.second);
         }

         template <typename InputIterator>
         inline bool operator()(const InputIterator begin, const InputIterator end)
         {
            InputIterator itr = begin;

            while (end != itr)
            {
               if (!add(*itr)) return false;
               ++itr;
            }

            return true;
         }

      private:

         mutable container_adder_base* container_adder_base_;
         unsigned char buffer[64];
      };

      template <typename T,typename is_stl_container_result>
      struct ca_type { typedef T& type; };

      template <typename T>
      struct ca_type <T,details::yes_t> { typedef  details::container_adder type; };

   }
