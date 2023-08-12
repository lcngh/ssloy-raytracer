#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>


class Vec3f {/*vec3 class based off of peter shirley's implementation in Ray Tracing Weekend Book 1*/
    
    public:
        // constructor(parameters) : initialization list {constructor body}
        Vec3f() : e{0.f,0.f,0.f} {}
        Vec3f(float x, float y) : e{x,y,0.f} {}
        Vec3f(float x, float y, float z) : e{x,y,z} {}

        Vec3f& operator*=(const float f) {
            e[0] *= f;
            e[1] *= f;
            e[2] *= f;

            return *this;
        }

        inline float magnitude() {
            return sqrt((e[0]*e[0]) + (e[1]*e[1]) + (e[2]*e[2]));
        }

        Vec3f& normalize() {
            float v_magnitude = magnitude();
            return *this *= (1 / v_magnitude);
        }

        float operator[](int i) const {return e[i];}
        float& operator[](int i) {return e[i];}
                

    public:

        // array for {x,y,z} etc ...
        float e[3];    
};

inline Vec3f operator-(const Vec3f &u, const Vec3f &v) {
    return Vec3f(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3f operator*(const Vec3f &u, const Vec3f &v) {
    return Vec3f(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3f operator*(float f, const Vec3f &v) {
    return Vec3f(f * v.e[0], f * v.e[1], f * v.e[2]);
}

inline Vec3f operator*(const Vec3f &v, float f) {
    return f * v;
}

inline float dot(const Vec3f &u, const Vec3f &v) {
    return u.e[0] * v.e[0] +
           u.e[1] * v.e[1] +
           u.e[2] * v.e[2];
}



class Sphere {/*sphere class based off ssloy's tinyraytracer lessons*/
    Vec3f center;
    float radius;

    public:
        Sphere(const Vec3f &c, const float &r) : center(c), radius(r) {}

        bool ray_intersect(const Vec3f &origin, const Vec3f &direction, float &t0) const {
            Vec3f L = center - origin;
            float tca = dot(L, direction);
            float d2 = dot(L, L) - tca*tca;
            if (d2 > radius*radius) return false;
            float thc = sqrtf(radius*radius - d2);
            t0 = tca - thc;
            float t1 = tca + thc;
            if (t0 < 0) {
                t0 = t1;
                return false;
            }
            return true;

        } 
};

#endif