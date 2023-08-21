#ifndef MATERIALS_H
#define MATERIALS_H

struct Collision_Record;

class Material {
    public:
    virtual bool scatter(const Collision_Record &record, Vec3f &attenuation) const = 0;
};

class Flat_Color : public Material {
    public:
        Flat_Color(const Vec3f &color) : albedo(color) {}

        virtual bool scatter(const Collision_Record &record, Vec3f &attenuation) const override {
            attenuation = albedo;
            return true;
        };

    public:
        Vec3f albedo;
};

#endif