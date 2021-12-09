#include <exception>
#include <iostream>
#include <experimental/coroutine>

// class awaiter needs three member functions
template<typename Type>
struct awaiter 
{
private:
        Type m_value;
    
public:
    awaiter(Type value):
        m_value{value} { }

    bool await_ready() noexcept
    {
        // if it returns false
        // co_await suspends the coroutine
        return false;
    }

    void await_suspend(std::experimental::coroutine_handle<>) noexcept 
    { }

    Type await_resume() noexcept 
    { return this->m_value; }
};

template<typename Type>
class awaitable
{

private:
    Type m_value;

public:
    awaitable(Type value):
        m_value{ value } { }

    auto operator co_await()
    {
        return awaiter<Type>{ this->m_value };
    }
};


// this is primary class template
// we assume SemanticReturnType is NOT type void
template<typename SemanticReturnType>
class ResumableType
{
    public:

    template<typename ReturnType>
    class PromiseType
    {
        template<typename Type> friend class ResumableType;

        public:
            using resumable_type = ResumableType<ReturnType>;
            
            using coroutine_frame_type
                = std::experimental::coroutine_handle<PromiseType>;

        private:
            // semantic return value
            ReturnType m_return_value;
        
        public:

            // this function is COMPULSORY
            // this static member is called
            // when C++ compiler fails to dynamically allocate
            // ResumableType
            static auto get_return_object_on_allocation_failure()
            {
                return resumable_type{ nullptr };
            }

            // this function is COMPULSORY
            // this member is used to create
            // ResumableType by C++ compiler.
            auto get_return_object()
            {
                return 
                    resumable_type{ coroutine_frame_type::from_promise(*this) };
            }

            // this function is COMPULSORY
            // it returns a type of awaiter
            // i will talk about this function very soon.
            auto initial_suspend()
            {
                return std::experimental::suspend_always{};
            }

            // this function is also COMPULSORY
            auto final_suspend()
            {
                return std::experimental::suspend_always{};
            }

            // this function is also COMPULSORY
            void unhandled_exception()
            {
                throw;
            }

            // co_await value
            template<typename Type>
            auto await_transform(Type&& value)
            {
                return awaitable<Type>{ std::forward<Type>(value) };
            }

            // co_return value
            void return_value(ReturnType value)
            {
                // std::cout << "value = " << value << std::endl;
                
                this->m_return_value = value;
            }

            // co_yield value
            void yield_value(ReturnType value)
            {
                this->m_return_value = value;
            }
    };

    // this type alias for promise_type is COMPULSORY.
    using promise_type = PromiseType<SemanticReturnType>;

    using coroutine_frame_type = 
        typename promise_type::coroutine_frame_type;

    private:
        // this handle is a bridge between
        // ResumableType and PromiseType
        coroutine_frame_type m_frame_handle;

        ResumableType(coroutine_frame_type handle):
            m_frame_handle{handle} { }

    public:
        ResumableType(const ResumableType& ) = delete;

        ResumableType(ResumableType&& rhs):
            m_frame_handle{rhs.m_frame_handle}
        {
            // we have to invalidate right hand side rhs
            // in the move constructor
            rhs.m_frame_handle = nullptr;
        }

        bool resume()
        {
            if(!this->m_frame_handle)
                return false;
            else
            {
                this->m_frame_handle.resume();

                return !this->m_frame_handle.done();
            }
        }

        SemanticReturnType get()
        {
            if(this->m_frame_handle)
            {
                auto promise =
                    this->m_frame_handle.promise();

                return promise.m_return_value;
            }
            else
                throw std::exception{};
        }

        void destroy()
        {
            if(this->m_frame_handle)
            {
                this->m_frame_handle.destroy();
                this->m_frame_handle = nullptr;
            }
        }

        ~ResumableType()
        {
            this->destroy();
        }

};

/////////////////////////////////////////////////

// this is specialization for type void
template<>
class ResumableType<void>
{
    public:

    class PromiseType
    {
        template<typename Type> friend class ResumableType;

        public:
            using resumable_type = ResumableType<void>;
            
            using coroutine_frame_type
                = std::experimental::coroutine_handle<PromiseType>;

        public:

            // this function is COMPULSORY
            // this static member is called
            // when C++ compiler fails to dynamically allocate
            // ResumableType
            static auto get_return_object_on_allocation_failure()
            {
                return resumable_type{ nullptr };
            }

            // this function is COMPULSORY
            // this member is used to create
            // ResumableType by C++ compiler.
            auto get_return_object()
            {
                return 
                    resumable_type{ coroutine_frame_type::from_promise(*this) };
            }

            // this function is COMPULSORY
            // it returns a type of awaiter
            // i will talk about this function very soon.
            auto initial_suspend()
            {
                return std::experimental::suspend_always{};
            }

            // this function is also COMPULSORY
            auto final_suspend()
            {
                return std::experimental::suspend_always{};
            }

            // this function is also COMPULSORY
            void unhandled_exception()
            {
                throw;
            }

            // co_await value
            template<typename Type>
            auto await_transform(Type&& value)
            {
                return awaitable<Type>{ std::forward<Type>(value) };
            }

            // co_return
            void return_void()
            { }
    };

    // this type alias for promise_type is COMPULSORY.
    using promise_type = PromiseType;

    using coroutine_frame_type = 
        typename promise_type::coroutine_frame_type;

    private:
        // this handle is a bridge between
        // ResumableType and PromiseType
        coroutine_frame_type m_frame_handle;

        ResumableType(coroutine_frame_type handle):
            m_frame_handle{handle} { }

    public:
        ResumableType(const ResumableType& ) = delete;

        ResumableType(ResumableType&& rhs):
            m_frame_handle{rhs.m_frame_handle}
        {
            // we have to invalidate right hand side rhs
            // in the move constructor
            rhs.m_frame_handle = nullptr;
        }

        bool resume()
        {
            if(!this->m_frame_handle)
                return false;
            else
            {
                this->m_frame_handle.resume();

                return !this->m_frame_handle.done();
            }
        }

        void destroy()
        {
            if(this->m_frame_handle)
            {
                this->m_frame_handle.destroy();
                this->m_frame_handle = nullptr;
            }
        }

        ~ResumableType()
        {
            this->destroy();
        }

};

template<typename SemanticReturnType = void>
using resumable_type = ResumableType<SemanticReturnType>;


