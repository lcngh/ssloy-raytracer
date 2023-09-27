#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
//#include "collidable.h"
//#include "collidable_list.h"

class Material;

using std::shared_ptr;

class Vec3f {/*vec3 class based off of peter shirley's implementation in Ray Tracing Weekend Book 1*/
    
    public:
        // constructor(parameters) : initialization list {constructor body}
        Vec3f() : e{0.f,0.f,0.f} {}
        Vec3f(float x, float y) : e{x,y,0.f} {}
        Vec3f(float x, float y, float z) : e{x,y,z} {}
        Vec3f(shared_ptr<Vec3f> v)  : e{v->e[0], v->e[1], v->e[2]} {}

        Vec3f operator-() const {return Vec3f(-e[0], -e[1], -e[2]);}

        Vec3f& operator*=(const float f) {
            e[0] *= f;
            e[1] *= f;
            e[2] *= f;

            return *this;
        }

        Vec3f& operator*=(const Vec3f v) {
            e[0] *= v.e[0];
            e[1] *= v.e[1];
            e[2] *= v.e[2];

            return *this;
        }
        // === to length of vector
        inline float magnitude() {
            return sqrt((e[0]*e[0]) + (e[1]*e[1]) + (e[2]*e[2]));
        }

        Vec3f& normalize() {
            //float v_magnitude = magnitude();
            return *this *= (1.f / magnitude());
        }

        float length_squared() const {
            return e[0]*e[0] + e[1]*e[1] * e[2]*e[2];
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

inline Vec3f operator-(const shared_ptr<Vec3f> &u, const Vec3f &v) {
    return Vec3f(u->e[0] - v.e[0], u->e[1] - v.e[1], u->e[2] - v.e[2]);
}

inline Vec3f operator-(const Vec3f &u, const shared_ptr<Vec3f> &v) {
    return Vec3f(u.e[0] - v->e[0],
                 u.e[1] - v->e[1], 
                 u.e[2] - v->e[2]);
}

inline Vec3f operator+(const Vec3f &u, const Vec3f &v) {
    return Vec3f(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3f operator+(const shared_ptr<Vec3f> &u, const Vec3f &v) {
    return Vec3f(u->e[0] + v.e[0], u->e[1] + v.e[1], u->e[2] + v.e[2]);
}

inline Vec3f operator*(const Vec3f &u, const Vec3f &v) {
    return Vec3f(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3f operator*(const Vec3f &u, const shared_ptr<Vec3f> &v) {
    return Vec3f(u.e[0] * v->e[0], u.e[1] * v->e[1], u.e[2] * v->e[2]);
}

inline Vec3f operator*(const shared_ptr<Vec3f> &u, const Vec3f &v) {
    return Vec3f(u->e[0] * v.e[0], u->e[1] * v.e[1], u->e[2] * v.e[2]);
}

inline Vec3f operator*(float f, const Vec3f &v) {
    return Vec3f(f * v.e[0], f * v.e[1], f * v.e[2]);
}

inline Vec3f operator*(float f, const shared_ptr<Vec3f> &v) {
    return Vec3f(f * v->e[0], f * v->e[1], f * v->e[2]);
}

inline Vec3f operator*(const shared_ptr<Vec3f> &v, float f) {
    return f * v;
}

inline Vec3f operator*(const Vec3f &v, float f) {
    return f * v;
}

inline Vec3f operator/(const Vec3f &v, float f) {
    return Vec3f(v.e[0] / f, v.e[1] / f, v.e[2] / f);
}

inline float dot(const Vec3f &u, const Vec3f &v) {
    return u.e[0] * v.e[0] +
           u.e[1] * v.e[1] +
           u.e[2] * v.e[2];
}

inline float dot(const Vec3f &u, const shared_ptr<Vec3f> &v) {
    return u.e[0] * v->e[0] +
           u.e[1] * v->e[1] +
           u.e[2] * v->e[2];
}

inline float distance(const Vec3f &u, const Vec3f &v) {
    return sqrt((u.e[0] - v.e[0]) * (u.e[0] - v.e[0]) +
                (u.e[1] - v.e[1]) * (u.e[1] - v.e[1]) +
                (u.e[2] - v.e[2]) * (u.e[2] - v.e[2]));
}

inline float distance(const Vec3f &u, const shared_ptr<Vec3f> &v) {
    return sqrt((u.e[0] - v->e[0]) * (u.e[0] - v->e[0]) +
                (u.e[1] - v->e[1]) * (u.e[1] - v->e[1]) +
                (u.e[2] - v->e[2]) * (u.e[2] - v->e[2]));
}

#endif