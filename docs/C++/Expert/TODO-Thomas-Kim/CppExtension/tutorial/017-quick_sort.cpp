#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

// most optimized sequential quick sort algorithm
template<typename ElementType>
std::vector<ElementType> sequential_quick_sort( std::vector<ElementType> container)
{
    if(container.empty()) return container;

	auto move_iterator = std::make_move_iterator<decltype(container.begin())>;
	using container_t = std::vector<ElementType>;
	
	container_t result{ container.back() };
	
	// removing last element from the std::vector is very efficient
	container.pop_back(); // removed last element
	
	
	auto predicate = [pivot = result.front()](const auto& e)
		{
			return e < pivot;
		};
		
	auto itr = std::partition(container.begin(), container.end(), predicate);
	
	auto sorted_lower_part = sequential_quick_sort(
		container_t{ move_iterator(container.begin()), move_iterator(itr)});
			
	auto sorted_higher_part = sequential_quick_sort(
		container_t{ move_iterator(itr), move_iterator(container.end())});
		
	result.insert(result.begin(), 
		move_iterator(sorted_lower_part.begin()),
		move_iterator(sorted_lower_part.end()));
				
	result.insert(result.end(), 
		move_iterator(sorted_higher_part.begin()),
		move_iterator(sorted_higher_part.end()));
	
	return result;
}

void test_sequential_quick_sort()
{
    std::vector<int> container {5, 7, 3, 4, 1, 9, 2, 8, 10, 6};

    stream << "Before Sort: " << container << endl;

    stream << "After Sort: " << sequential_quick_sort(container) << endl;
}

void test_sequential_quick_sort_string()
{
    std::vector<std::string> container {"banana", "dango", "cherry", "echo", "apple", "fox",
        "hotel", "golf"};

    stream << "Before Sort: " << container << endl;

    stream << "After Sort: " << sequential_quick_sort(container) << endl;
}

int main()
{
    // test_sequential_quick_sort();
    test_sequential_quick_sort_string();
}
