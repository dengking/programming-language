struct Geometry
{
    enum GeomType
    {
        POINT,
        CIRCLE
    };
    virtual ~Geometry() {}
    virtual int draw() = 0;
    //
    // Factory method for all the Geometry objects
    //
    static Geometry *create(GeomType i);
};
struct Point : Geometry
{
    int draw() { return 1; }
    double width() { return 1.0; }
};
struct Circle : Geometry
{
    int draw() { return 2; }
    double radius() { return 1.5; }
};

Geometry *Geometry::create(GeomType type)
{
    switch (type)
    {
    case POINT:
        return new Point();
    case CIRCLE:
        return new Circle();
    default:
        return 0;
    }
}

// swig -c++ -python geometry.i
// g++ --std=c++11 -fpic -shared -L. -I. -I/usr/include/python3.8/ geometry_wrap.cxx -o _geometry.so 
// -lexample 一定要放到最后，否则会导致错误