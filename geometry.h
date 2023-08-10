#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>


class Vec3f {/*vec3 class based off of peter shirley's implementation in Ray Tracing Weekend Book 1*/
    
    public:
        // constructor(parameters) : initialization list {constructor body}
        Vec3f() : e{0.f,0.f,0.f} {}
        Vec3f(float x, float y) : e{x,y,0.f} {}
        Vec3f(float x, float y, float z) : e{x,y,z} {}
        
        float operator[](int i) const {return e[i];}
        float& operator[](int i) {return e[i];}
        
        Vec3f operator-(const Vec3f &u, const Vec3f &v) {
            return Vec3f(u[0]-v[0], u[1]-v[1], u[2]-v[2]);
        }
    public:

        // array for {x,y,z} etc ...
        float e[3];    
};



class Sphere {/*sphere class based off ssloy's tinyraytracer lessons*/
    Vec3f center;
    float radius;

    public:
        Sphere(const Vec3f &c, const float &r) : center(c), radius(r) {}

        bool ray_intersect(const Vec3f &origin, const Vec3f &direction, float &t0) const {
            Vec3f p_to_c = center - origin;
        } 
};

#endif