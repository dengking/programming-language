// file : vector.i 
%module mymodule 
%{
    #include "vector.h" 
%}

%include "vector.h" // Just grab original C header file 
%extend Vector { // Attach these functions to struct Vector 
Vector(double x, double y, double z) 
{ 
    Vector *v; v = (Vector *) malloc(sizeof(Vector)); 
    v->x = x; v->y = y; v->z = z; 
    return v; 
}
~Vector() { free($self); }

};