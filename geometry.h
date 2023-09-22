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



// class Sphere : public Collidable {/*sphere class based off ssloy's tinyraytracer lessons*/

//     public:
//         Sphere(const Vec3f &c, const float &r, shared_ptr<Material> m) : center(c), radius(r) , mat_ptr(m){distance_from_origin = distance(Vec3f(0,0,0), center);}

//         virtual bool ray_intersect(const Vec3f &origin, const Vec3f &direction, Collision_Record &record) const override{
//             Vec3f L = center - origin;
//             float tca = dot(L, direction);
//             auto a = direction.length_squared();
//             auto c = L.length_squared() - radius*radius;
//             float d2 = dot(L, L) - tca*tca;
//             if (d2 > radius*radius) return false;
//             float thc = sqrtf(radius*radius - d2);
//             float t0 = tca - thc;
//             float t1 = tca + thc;
//             if (t0 < 0) {
//                 t0 = t1;
//                 return false;
//             }
//             auto discriminant = tca*tca - a*c;
//             auto sqrt_discriminant = sqrt(discriminant);
//             auto root = (-tca - sqrt_discriminant) / a;

//             record.t = root;
//             record.point = make_shared<Vec3f>(origin + record.t*direction);
//             Vec3f outward_normal = (record.point - center) / radius;
//             record.set_face_normal(direction, outward_normal);
//             record.mat_ptr = mat_ptr;
//             record.sphere_distance = distance_from_origin;
//             //record.point = 
//             //record.sphere_distance =
//             record.last_center = make_shared<Vec3f>(center);
//             return true;
//         } 

//     public:
//         Vec3f center;
//         float radius;
//         float distance_from_origin;
//         shared_ptr<Material> mat_ptr;

// };



#endif