#include <tpf_output.hpp>
#include <tpf_chrono_random.hpp>
#include <execution>

tpf::sstream stream;
auto nl = tpf::nl;
auto endl = tpf::endl;

namespace fcp
{
    template<typename ElementType>
    class vector_3d
    {
        public:
            using element_type = ElementType;

        private:
            element_type m_x, m_y, m_z, m_color;

        public:
            vector_3d() { }

            vector_3d(element_type x, element_type y, element_type z):
            m_x{x}, m_y{y}, m_z{z} { }
           
            friend vector_3d operator* (const vector_3d& L, const vector_3d& R)
            {
                return { L.m_y * R.m_z - L.m_z * R.m_y, 
                         L.m_z * R.m_x - L.m_x * R.m_z,
                         L.m_x * R.m_y - L.m_y * R.m_x };
            }

            friend std::ostream& operator<<(std::ostream& os, const vector_3d& v)
            {
                os << "< " << v.m_x <<", " << v.m_y <<", " << v.m_z <<" >";
                return os;
            }
    };

} // end of fcp

void test_vector_operation()
{
    fcp::vector_3d<double> v1{1, 0, 0}, v2{0, 1, 0}, v3{0, 0, 1};

    stream << "v1 = " << v1 << endl;
    stream << "v2 = " << v2 << endl;
    stream << "v3 = " << v3 << endl;
    
    auto a1 = v1 * v2;

    auto a2 = v2 * v3;
    auto a3 = v3 * v1;

    stream << "a1 = " << a1 << endl;
    stream << "a2 = " << a2 << endl;
    stream << "a3 = " << a3 << endl;
}

template<typename Policy>
void performance_test(Policy policy, size_t test_count, size_t element_count)
{
    std::vector<fcp::vector_3d<float>> points; 
    points.reserve(element_count);

    auto generator = tpf::chrono_random::random_generator<float>(-10, +10);

    fcp::vector_3d<float> r {generator(), generator(), generator()};

    for(size_t i = 0; i < element_count; ++i)
        points.emplace_back(generator(), generator(), generator());

    for(size_t i = 0; i < test_count; ++i)
    {
        tpf::chrono_random::stop_watch sw;
        
        std::transform(policy, points.begin(), points.end(), points.begin(), 
            [&r](const auto& l){ return l * r; });

        stream << "Elapsed: "<< sw.elapsed_time() << nl << endl;
    }
}

int main()
{
    size_t element_count = 100'000'000;

    stream <<"Sequential - \n";
    performance_test(std::execution::seq, 5, element_count);

    stream <<"Parallel - \n";
    performance_test(std::execution::par, 5, element_count);

    stream <<"Parallel Vectorized - \n";
    performance_test(std::execution::par_unseq, 5, element_count);
    
}