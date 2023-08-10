#ifndef GEOMETRY_H
#define GEOMETRY_H



/*
vec3 class based off of peter shirley's implementation in Ray Tracing Weekend Book 1
*/

class Vec3f {
    public:
        // constructor(parameters) : initialization list {constructor body}
        Vec3f() : e{0,0,0} {}
        Vec3f(double x, double y, double z) : e{x,y,z} {}
        Vec3f(double x, double y) : e{x,y,0.0} {}



    public:

        // array for {x,y,z} etc ...
        double e[3];    
};

#endif