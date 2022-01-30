#include <tpf_output.hpp>
#include <tpf_chrono_random.hpp>
#include <execution> // for Parallel Algorithm

tpf::sstream stream;
auto nl = tpf::nl;
auto endl = tpf::endl;

template<typename ElementType>
class vector_3d
{
    public:
        using element_type = ElementType;

    private:
        element_type m_x, m_y, m_z, m_color;

    public:
        vector_3d() {}
        vector_3d(element_type x, element_type y, element_type z):
            m_x{x}, m_y{y}, m_z{z} { }

        // vector cross product
        friend vector_3d operator*(const vector_3d& L, const vector_3d& R)
        {
            // auto x = L.m_y * R.m_z - L.m_z * R.m_y;
            // auto y = L.m_z * R.m_x - L.m_x * R.m_z;
            // auto z = L.m_x * R.m_y - L.m_y * R.m_x;

            return { L.m_y * R.m_z - L.m_z * R.m_y,
                     L.m_z * R.m_x - L.m_x * R.m_z,
                      L.m_x * R.m_y - L.m_y * R.m_x};
        }

        friend std::ostream& operator<<(std::ostream& os, const vector_3d& v)
        {
            os << "< " << v.m_x << ", " << v.m_y <<", " << v.m_z <<" >";
            return os;
        }
};


void test_vector_3d()
{
    vector_3d<double> x{1, 0, 0}, y{0, 1, 0}, z{0, 0, 1};

    auto v1 = y * z; // < 1, 0, 0 >
    auto v2 = z * x; // < 0, 1, 0 >
    auto v3 = x * y; // < 0, 0, 1 >

    stream << "x = " << x<<", y = " << y <<", z = " << z << nl;
    stream << "v1 = " << v1 <<", v2 = " << v2 <<", v3 = " << v3 << endl;
}

template<typename ExecutionPolicy>
void naive_dangerous_parallel_algorithm(ExecutionPolicy policy, size_t test_count, size_t element_count)
{
    std::vector<vector_3d<float>> points; // 3 dimensional points
    points.reserve(element_count);

    // random number generator generating floats
    // ranging from -10 to +10
    auto generator = tpf::chrono_random::random_generator<float>(-10, +10);
    
    vector_3d<float> rhs{generator(), generator(), generator()};

    for(size_t i = 0; i < element_count; ++i)
        // generate 3 floats and initialize vector_3d elements
        points.emplace_back(generator(), generator(), generator());

    // stream <<"Before Cross Product: \n" << points << nl;

    tpf::chrono_random::stop_watch sw;

    for(size_t i = 0; i < test_count; ++i)
    {
        sw.reset();
         std::transform(policy, points.begin(), points.end(), 
                 points.begin(),
                [&rhs](const auto& lhs){ return lhs * rhs; }
            );

        stream << "Elapsed - " << sw.elapsed_time() << nl;
    }

    // without endl, we cannot see anything
    stream << endl;

    // stream <<"\nAfter Cross Product: \n" << points << endl;
}

void test_naive_dangerous_parallel_algorithm()
{
    size_t element_count = 10'000'000;

    stream <<"Sequential policy: \n";
    naive_dangerous_parallel_algorithm(std::execution::seq, 5, element_count);

    stream <<"\nParallel policy: \n";
    naive_dangerous_parallel_algorithm(std::execution::par, 5, element_count);
    
    stream <<"\nParallel Unsequenced policy: \n";
    naive_dangerous_parallel_algorithm(std::execution::par_unseq, 5, element_count);
}

////////////////////////
template<typename ExecutionPolicy>
void why_naive_dangerous_parallel_algorithm(ExecutionPolicy policy, size_t test_count, size_t element_count)
{
    std::vector<vector_3d<float>> points; // 3 dimensional points
    points.reserve(element_count);

    // random number generator generating floats
    // ranging from -10 to +10
    auto generator = tpf::chrono_random::random_generator<float>(-10, +10);
    
    vector_3d<float> rhs{generator(), generator(), generator()};

    for(size_t i = 0; i < element_count; ++i)
        // generate 3 floats and initialize vector_3d elements
        points.emplace_back(generator(), generator(), generator());

    // stream <<"Before Cross Product: \n" << points << nl;

    tpf::chrono_random::stop_watch sw;

    for(size_t i = 0; i < test_count; ++i)
    {
        sw.reset();
         std::transform(policy, points.begin(), points.end(), 
                 points.begin(),

                 // When we use Parallel Algorithm,
                 // we should be extremely careful not to leak
                 // exceptions from the parallel callback functions.
                [&rhs](const auto& lhs)
                { 
                    throw 1; // this cause system crash.

                    return lhs * rhs; 
                }
            );

        stream << "Elapsed - " << sw.elapsed_time() << nl;
    }

    // without endl, we cannot see anything
    stream << endl;

    // stream <<"\nAfter Cross Product: \n" << points << endl;
}

void test_why_naive_dangerous_parallel_algorithm()
{
    size_t element_count = 10'000'000;

    try
    {
        stream <<"Sequential policy: \n";
        why_naive_dangerous_parallel_algorithm(std::execution::seq, 5, element_count);

        stream <<"You cannot see this message" << endl;
    }
    catch(...)
    {
        stream << "This does not work - you cannot see this message either" << endl;
    }

    // stream <<"\nParallel policy: \n";
    // naive_dangerous_parallel_algorithm(std::execution::par, 5, element_count);
    
    // stream <<"\nParallel Unsequenced policy: \n";
    // naive_dangerous_parallel_algorithm(std::execution::par_unseq, 5, element_count);
}

int main()
{
    // test_vector_3d();
    // test_naive_dangerous_parallel_algorithm();
    test_why_naive_dangerous_parallel_algorithm();
}