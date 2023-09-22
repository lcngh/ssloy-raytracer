#ifndef SPHERE_H
#define SPHERE_H
#include <limits>
#include <iostream>
//#include "geometry.h"
#include "collidable_list.h"

const float infinity = std::numeric_limits<float>::infinity();

class Sphere : public Collidable {/*sphere class based off ssloy's tinyraytracer lessons*/

    public:
        Sphere(const Vec3f &c, const float &r, shared_ptr<Material> m) : center(c), radius(r) , mat_ptr(m){distance_from_origin = distance(Vec3f(0,0,0), center);}

        virtual bool ray_intersect(const Vec3f &origin, const Vec3f &direction, Collision_Record &record) const override{
            Vec3f L = center - origin;
            float tca = dot(L, direction);
            float d2 = dot(L, L) - tca*tca;
            if (d2 > radius*radius) return false;
            float thc = sqrtf(radius*radius - d2);
            float t0 = tca - thc;
            record.mat_ptr = mat_ptr;
            record.last_center = make_shared<Vec3f>(center.e[0], center.e[1], center.e[2]);
            //record.last_sphere_distance = distance_from_origin;
            if (t0>0.001) {
                record.t = t0;
                return true;
            }
            float t1 = tca + thc;
            if (t1>0.001) {
                record.t = t0;
                return true;
            }
            record.t = 0.0;
            return false;
            // Vec3f oc = origin - center;
            // auto a = direction.length_squared();
            // auto half_b = dot(oc, direction);
            // auto c = oc.length_squared() - radius*radius;
            // auto discriminant = half_b*half_b - a*c;
            // if (discriminant < 0) return false;
            // auto sqrt_discriminant = sqrt(discriminant);

            // auto root = (-half_b - sqrt_discriminant) / a;
            // if (root < 0.001 || infinity < root) {
            //     root = (-half_b + sqrt_discriminant) / a;
            //     if (root < 0.001 || infinity < root) {
            //         return false;
            //     }
            // }
            // //if (root < 0.001 || root > INFINITY) {root = (-tca + sqrt_discriminant) / a; }

            // record.t = root;
            // //std::cerr << "root: " << root << " \n";
            // record.point = make_shared<Vec3f>(origin + record.t*direction);
            // Vec3f outward_normal = (record.point - center) / radius;
            // record.set_face_normal(direction, outward_normal);
            // record.mat_ptr = mat_ptr;
            // record.sphere_distance = distance_from_origin;
            // //record.sphere_distance = distance(Vec3f(0,0,0),record.point);
            //record.point = 
            //record.sphere_distance =
            //record.last_center = make_shared<Vec3f>(center);

        } 

    public:
        Vec3f center;
        float radius;
        float distance_from_origin;
        shared_ptr<Material> mat_ptr;

};

#endif