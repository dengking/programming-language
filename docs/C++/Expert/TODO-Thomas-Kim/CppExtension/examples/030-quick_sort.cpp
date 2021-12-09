#include <tpf_output.hpp>
#include <tpf_chrono_random.hpp>

tpf::sstream stream;
template<typename Type>
using list_t = std::list<Type>;

template<typename Type>
using vector_t = std::vector<Type>;

template<typename Type>
list_t<Type> sequential_quick_sort(list_t<Type> list)
{
    if(list.empty())
        return list;

    auto move_itr = 
        std::make_move_iterator<decltype(list.begin())>;

    list_t<Type> result{ list.front() };
    list.erase(list.begin());

    auto predicate = [pivot = result.front()] (auto e)
        {
            return e < pivot;
        };

    auto itr = std::partition(list.begin(), list.end(), predicate);

    auto lower = sequential_quick_sort(list_t<Type>{list.begin(), itr});
    auto higher = sequential_quick_sort(list_t<Type>{itr, list.end()}); 

    result.insert(result.begin(), move_itr(lower.begin()),
                                  move_itr(lower.end()));

    result.insert(result.end(), 
                                  move_itr(higher.begin()),
                                  move_itr(higher.end()));

    return result;
}

template<typename Type>
list_t<Type> sequential_quick_sort_splice(list_t<Type> list)
{
    if(list.empty())
        return list;

    auto move_itr = 
        std::make_move_iterator<decltype(list.begin())>;

    list_t<Type> result;
    result.splice(result.end(), list, list.begin());

    auto predicate = [pivot = result.front()] (auto e)
        {
            return e < pivot;
        };

    auto itr = std::partition(list.begin(), list.end(), predicate);

    list_t<Type> lower_part;
    lower_part.splice(lower_part.end(), list, list.begin(), itr);
    auto& high_part = list; 

    auto lower = sequential_quick_sort( std::move(lower_part) );
    auto higher = sequential_quick_sort( std::move(high_part) ); 

    result.insert(result.begin(), move_itr(lower.begin()),
                                  move_itr(lower.end()));

    result.insert(result.end(), 
                                  move_itr(higher.begin()),
                                  move_itr(higher.end()));

    return result;
}

void example_sequential_quick_sort()
{
    using element_t = int;
    using list_t = std::list<element_t>;

    list_t lst{5, 7, 3, 4, 1, 9, 2, 8, 10, 6};
    
    stream << sequential_quick_sort_splice(lst) << tpf::endl;

}

template<typename ElementType>
vector_t<ElementType> sequential_quick_sort(vector_t<ElementType> container)
{
    if(container.empty())
        return container;

    vector_t<ElementType> result{ container.front() };
    container.erase(container.begin());

    auto predicate = [pivot = result.front()] (const auto& e)
    {
        return e < pivot;
    };

    auto itr = std::partition(container.begin(), container.end(), predicate);


    auto lower_part = vector_t<ElementType> 
        { std::make_move_iterator(container.begin()), std::make_move_iterator(itr)};

    auto higher_part = vector_t<ElementType> 
        { std::make_move_iterator(itr), std::make_move_iterator(container.end()) };

    auto sorted_lower = sequential_quick_sort(std::move(lower_part));
    auto sorted_higher = sequential_quick_sort(std::move(higher_part));

    result.insert(result.begin(), 
        std::make_move_iterator(sorted_lower.begin()),
        std::make_move_iterator(sorted_lower.end()));

    result.insert(result.end(), 
        std::make_move_iterator(sorted_higher.begin()),
        std::make_move_iterator(sorted_higher.end()));

    return result;
}

template<typename ElementType>
vector_t<ElementType> sequential_quick_sort_vector(vector_t<ElementType> container)
{
    if(container.empty())
        return container;

    vector_t<ElementType> result{ container.back() };
    container.pop_back();

    auto predicate = [pivot = result.front()] (const auto& e)
    {
        return e < pivot;
    };

    auto itr = std::partition(container.begin(), container.end(), predicate);

    auto move_iterator = std::make_move_iterator<decltype(container.begin())>;

    auto lower_part = vector_t<ElementType> 
        { move_iterator(container.begin()), move_iterator(itr)};

    auto higher_part = vector_t<ElementType> 
        { move_iterator(itr), move_iterator(container.end()) };

    auto sorted_lower = sequential_quick_sort(std::move(lower_part));
    auto sorted_higher = sequential_quick_sort(std::move(higher_part));

    result.insert(result.begin(), 
        move_iterator(sorted_lower.begin()),
        move_iterator(sorted_lower.end()));

    result.insert(result.end(), 
        move_iterator(sorted_higher.begin()),
        move_iterator(sorted_higher.end()));

    return result;
}

void example_sequential_quick_sort_vector()
{
    vector_t<int> container{5, 7, 3, 4, 1, 9, 2, 8, 10, 6};
    
    stream << sequential_quick_sort_vector(std::move(container)) << tpf::endl;

}

int main()
{
    example_sequential_quick_sort_vector();
}