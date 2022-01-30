#include <iostream>
#include <new>

constexpr size_t BufferCount = 10;

int gBufferMemory[BufferCount];

void test_placement_new()
{
    auto base_ptr = &gBufferMemory[0];

    int& a = *new( base_ptr + 0 ) int{0};
    int& b = *new( base_ptr + 1 ) int{1};
    int& c = *new( base_ptr + 2 ) int{2};
    int& d = *new( base_ptr + 3 ) int{3};
    int& e = *new( base_ptr + 4 ) int{4};

    std::cout << "a + b + c + d + e = "
        << a + b + c + d + e << std::endl;
}

void test_placement_new_using_static()
{
    constexpr size_t buffer_size = 5;

    int buffer[buffer_size];

    int* ap = new(buffer+0) int{0};
    int* bp = new(buffer+1) int{1};
    int* cp = new(buffer+2) int{2};
    int* dp = new(buffer+3) int{3};
    int* ep = new(buffer+4) int{4};

    std::cout << "ap + bp + cp + dp + ep = "
        << *ap + *bp + *cp + *dp + *ep << std::endl; 
}

int main()
{
    test_placement_new();

    test_placement_new_using_static();
}