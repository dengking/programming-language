#include <tpf_output.hpp>
#include <tpf_matrix.hpp>
#include <tpf_chrono_random.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

struct alignas(16) vector_3d_pod
{
    size_t m_x, m_y, m_z, m_color; 
    void set(size_t x, size_t y, size_t z, size_t c)
    {
        this->m_x = x; this->m_y = y; this->m_z = z; this->m_color = c;
    }
};

struct alignas(16) vector_3d_class
{
    size_t m_x, m_y, m_z, m_color;
    void set(size_t x, size_t y, size_t z, size_t c)
    {
        this->m_x = x; this->m_y = y; this->m_z = z; this->m_color = c;
    }
    ~vector_3d_class() { }
};

std::ostream& operator<<(std::ostream& os, const vector_3d_pod& obj)
{
    os << "< " << obj.m_x <<", " <<obj.m_y << ", " << obj.m_z << " >";
    return os;
}

std::ostream& operator<<(std::ostream& os, const vector_3d_class& obj)
{
    os << "< " << obj.m_x <<", " <<obj.m_y << ", " << obj.m_z << " >";
    return os;
}

template<typename ElementType>
void performance_test()
{
    size_t test_count = 100;
    size_t array_size = 1'000'000;

    stream << "Testing std::vector<int> - " << array_size << " elements - " 
        << test_count << " times " << endl;

    tpf::chrono_random::stop_watch sw;
    for(size_t i = 0; i < test_count; ++i)
    {
        std::vector<ElementType> v(array_size);
        for(size_t j = 0; j < array_size; ++j)
           v[j].set(j, j, j, j);

        //auto vv = v; 

        // stream << vv << endl;
    }
    stream << " - Elapsed: " << sw.elapsed_time() << endl;

    stream << "\nTesting tpf::dynamic_array<int> - " << array_size << " elements - " 
        << test_count << " times " << endl;
    for(size_t i = 0; i < test_count; ++i)
    {
        tpf::matrix::dynamic_array<ElementType> array{array_size};

        for(size_t j = 0; j < array_size; ++j)
           array[j].set(j, j, j, j);

        //auto aa = array;

        // stream << aa << endl;   
    }
    stream << " - Elapsed: " << sw.elapsed_time() << endl;
}

int main()
{
    performance_test<vector_3d_pod>();
    stream << endl << endl;
    performance_test<vector_3d_class>();
    stream << endl << endl;

}