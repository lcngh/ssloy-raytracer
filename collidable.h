#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "geometry.h"

class Vec3f;

class Collidable {
    public: 
        virtual bool ray_intersect(const Vec3f &origin, const Vec3f &direction) const = 0;

};

#endif