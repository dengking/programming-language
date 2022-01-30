#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;
auto nl = tpf::nl;

struct Point
{
    int m_x, m_y;
};

struct Circle
{
    Point m_center;
    int m_radius;

    void display() const
    {
        stream << " Circle - center: (" <<
            m_center.m_x << ", " << m_center.m_y 
            << "), radius: " << m_radius << " ";
    }
};

struct Square
{
    Point m_center;
    int m_side;

    void display() const
    {
        stream << " Square - center: ("
            << m_center.m_x <<", " << m_center.m_y
            <<") - Side: " << m_side << " " <<endl;
    }
};

struct Shape: public Circle, public Square
{
    Shape(Circle c, Square s): Circle{c}, Square{s} { }

    void display() const
    {
        // auto& base_circle = static_cast<const Circle&>(*this);
        // base_circle.display();

        Circle::display();

        // auto& base_square = static_cast<const Square&>(*this);
        // base_square.display();

        Square::display();
    }
};

void test_shape()
{
    Shape s{Circle{ {3, 5}, 6} , Square{{3, 3}, 7} };

    s.display();
}

template<typename... BaseClasses>
struct Mixin: public BaseClasses...
{
    // template<typename... Shapes>
    Mixin(BaseClasses... shape): BaseClasses{ shape }... { }

    // using BaseClasses::display() const...;

    using base_types = tpf::types::type_list_t<BaseClasses...>;

    void display() const
    {
        // Graph in C++ #8: Type List, std::tuple, container of tuples
        // https://www.youtube.com/watch?v=HqHy8DVDtw4&t=410s

        // 24 - Type List and Compile-Time Loop 1/N
        // https://www.youtube.com/watch?v=snfLE8aYv3M&list=PL1_C6uWTeBDE6KepUEI9XeOpLc4hgDVOd&index=24
        
        using first_base = tpf::types::select_first_type_t<base_types>;
        first_base::display();

        using second_base = tpf::types::select_nth_type_t<1, base_types>;
        second_base::display();
    }
};

// this is called Class Template Parameter Type Deduction Guide
template<typename... Shapes>
Mixin(Shapes...)->Mixin<Shapes...>;

void test_mixin()
{
    Mixin m{ Circle{ {0, 0}, 5 }, Square{{3, 3}, 10 } };

    m.display();
}

int main()
{
    // test_shape();

    test_mixin();
}