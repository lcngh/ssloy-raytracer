#ifndef COLLIDABLE_LIST_H
#define COLLIDABLE_LIST_H

#include <vector>
#include <memory>
#include "collidable.h"

using std::shared_ptr;
using std::make_shared;

class Collidable_List : public Collidable {
    public:
        Collidable_List() {};
        Collidable_List(shared_ptr<Collidable> object) {add(object);}

        void clear() {objects.clear();}
        void add(shared_ptr<Collidable> object) {objects.push_back(object);}

        virtual bool ray_intersect(const Vec3f &origin, const Vec3f &direction, Collision_Record &record) const override;

    public:
        std::vector<shared_ptr<Collidable>> objects;

};

bool Collidable_List::ray_intersect(const Vec3f &origin, const Vec3f &direction, Collision_Record &record) const {

    Collision_Record temp_record;

    bool collision = false;

    for (const auto& object : objects) {
        if (object->ray_intersect(origin, 
        direction, 
        temp_record)) {
            collision = true;
            record = temp_record;
        }
    }

    return collision;
}

#endif