#include <iostream>
#include <type_traits>
#include <experimental/coroutine>

/*

    Suppose we purchased a new book. And this book has 
    20 chapters. In my case,

    I usually read the Preface, then I read the back cover.

    To understand Chapter 1, you need to understand
    from chapter 2 through chapter 20.

    Again, to understand chapter 2, you need to understand
    from chapter 1, and chapter 3 through chapter 20.

    Don't ever try to understand chapter 1 completely.
    Also never try to understand chapter 2 completely before
    you read all the chapters.

    My usual strategy is...

    I read from chapter 1 to chapter 20 very fast. I don't
    get bogged down on a specific chapter. I think the chapter
    seems difficult, I still read but I read fast and skip
    a few sections. I try to read as much of the chapter
    but I don't get bogged down a specific section.

    Usually at the first round of reading, read your book
    as quickly as fast. After reading from chapter 1 through
    chapter 20, then come back to chapter 1 again.

    Repeat this method to read the book several times.

    By the time, you read the book 2 or 3 times, you can get
    the big picture of the book. Then you delve into more
    specific chapters and sections. This time, try to understand
    all the nitty-gritty details.

    My point is... don't get bogged down, don't try
    to understand all at once. Repeat and think again.
    Always get the big picture, the details become crystal
    clear.

    I bet corountine will be quite challenging for most of us.
    But still don't get bogged down. Keep reading and keep watching
    my videos. In future sessions, I will elaborate all the 
    details one by one.
 */

template<typename SemanticReturnType> class ResumableType;
template<typename SemanticReturnType> class PromiseType;

template<typename SemanticReturnType>
class PromiseType
{
    template<typename Type> friend class ResumableType;

    public:

        // this is not compulsory but for our own use
        using resumable_type = ResumableType<SemanticReturnType>;
        
        using promise_handle_type
            = std::experimental::coroutine_handle<PromiseType>;

    private:
        // the result of operation will be stored
        // in this member variable
        SemanticReturnType m_semantic_return_value;

    public:

        // this member function is compulsory
        // C++ compiler uses this function to create 
        // ResumableType<SemanticReturnType> object
        // for our coroutine
        auto get_return_object()
        {
            return resumable_type{  promise_handle_type::from_promise(*this) };
        }

        // this is static member function 
        // this function is also compulsory
        static auto get_return_object_on_allocation_failure()
        {
            return resumable_type{nullptr};
        }

        // this is also COMPULSORY
        void unhandled_exception()
        {
            std::terminate();
        }

        // this function is also compuslory
        auto initial_suspend()
        {
            return std::experimental::suspend_always{};
        }

        // this function is also compulsory
        auto final_suspend()
        {
            return std::experimental::suspend_always{};
        }

        // for co_return value
        auto return_value(SemanticReturnType value)
        {
            // store the result a+b to our member variable
            this->m_semantic_return_value = value;

            return std::experimental::suspend_always{};
        }

        // for co_yield value
        auto yield_value(SemanticReturnType value)
        {
            // store the result a+b to our member variable
            this->m_semantic_return_value = value;

            return std::experimental::suspend_always{};
        }

};

template<typename SemanticReturnType>
class ResumableType
{
    template<typename Type> friend class PromiseType;

    public:

        // compulsory - this type alias is MUST-HAVE
        // the C++ compiler needs to know
        // promise_type, otherwise coroutine does not work.
        // this promise_type declares to the Compiler that
        // our promise type is PromiseType<SemanticReturnType> 
        using promise_type = PromiseType<SemanticReturnType>;

        // this is not compulsory, but it is very handy for our use
        using promise_handle_type
            = std::experimental::coroutine_handle<promise_type>;

    private:
        // this member bridges between 
        // ResumableType and PromiseType
        promise_handle_type m_promise_handle;

        // this is not publicly accessible
        ResumableType(promise_handle_type handle):
            m_promise_handle{handle} { }

    public:
        // disable copy constructor
        // ResumableType is not copyable
        ResumableType(const ResumableType&) = delete;

        // move constructor
        ResumableType(ResumableType&& rhs)
        {
            this->m_promise_handle = rhs.m_promise_handle;
            
            // invalidate right hand side
            rhs.m_promise_handle = nullptr;
        }

        SemanticReturnType get()
        {
            // PromiseType<SemanticReturnType>
            auto promise = 
                this->m_promise_handle.promise();

            return promise.m_semantic_return_value;
        }

        // we resume our suspended coroutine
        // if m_promise_handle is invalid, then return false
        // otherwise, resume coroutine, and return true if not yet done.
        bool resume()
        {
            if(this->m_promise_handle)
            {
                this->m_promise_handle.resume();

                return !this->m_promise_handle.done();
            }
            else
                return false;
        }

        SemanticReturnType next()
        {
            this->resume();
            return this->get();
        }

    ~ResumableType()
    {
        if(this->m_promise_handle)
            this->m_promise_handle.destroy();
   }
};
