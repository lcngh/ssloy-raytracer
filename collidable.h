#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include <memory>

class Vec3f;
class Material;

struct Collision_Record {

    //float point;
    float sphere_distance;
    std::shared_ptr<Vec3f> last_direction;
    std::shared_ptr<Vec3f> last_hit;
    std::shared_ptr<Vec3f> last_center;
    std::shared_ptr<Vec3f> last_normal;
    std::shared_ptr<Material> mat_ptr;
    double t;
    bool front_face;

    // inline void set_face_normal(const Vec3f &direction, const Vec3f &outward_normal) {
    //     front_face = dot(direction, outward_normal) < 0;
    //     normal = std::make_shared<Vec3f>(front_face ? outward_normal : -outward_normal);
    // }

};

class Collidable {
    public: 
        virtual bool ray_intersect(const Vec3f &origin, const Vec3f &direction, Collision_Record &record) const = 0;

};

#endif
