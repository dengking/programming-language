# Named Constructor



## More C++ Idioms [Named Constructor](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Named_Constructor)



## isocpp What is the “Named Constructor Idiom”? [¶](https://isocpp.org/wiki/faq/ctors#named-ctor-idiom) [Δ](https://isocpp.org/wiki/faq/ctors#)

A technique that provides more intuitive and/or safer construction operations for users of your class.

```c++
#include <cmath>               // To get std::sin() and std::cos()

/*
 // 反面例子
 class Point {
 public:
 Point(float x, float y);     // Rectangular coordinates
 Point(float r, float a);     // Polar coordinates (radius and angle)
 // ERROR: Overload is Ambiguous: Point::Point(float,float)
 };
 int main()
 {
 Point p = Point(5.7, 1.2);   // Ambiguous: Which coordinate system?
 // ...
 }

 */
class Point
{
public:
	static Point rectangular(float x, float y);      // Rectangular coord's 直角坐标系
	static Point polar(float radius, float angle);   // Polar coordinates 极坐标系
	// These static methods are the so-called "named constructors"
	// ...
private:
	Point(float x, float y);     // Rectangular coordinates
	float x_, y_;
};
inline Point::Point(float x, float y)
		: x_(x), y_(y)
{
}
inline Point Point::rectangular(float x, float y)
{
	return Point(x, y);
}
inline Point Point::polar(float radius, float angle)
{
	return Point(radius * std::cos(angle), radius * std::sin(angle));
}
int main()
{
	Point p1 = Point::rectangular(5.7, 1.2);   // Obviously rectangular
	Point p2 = Point::polar(5.7, 1.2);         // Obviously polar
	// ...
}

```

