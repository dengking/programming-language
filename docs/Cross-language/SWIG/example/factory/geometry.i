%module geometry
%{
#include "geometry.h"
#include <iostream>
%}
%newobject Geometry::create;
%factory(Geometry *Geometry::create, Point, Circle);
%include "geometry.h"