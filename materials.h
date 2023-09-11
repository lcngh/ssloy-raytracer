#ifndef MATERIALS_H
#define MATERIALS_H

//#include <cmath>

struct Collision_Record;

class Material {
    public:
    virtual bool scatter(const Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const = 0;
};

class Flat_Color : public Material {
    public:
        Flat_Color(const Vec3f &color) : albedo(color) {}

        virtual bool scatter(const Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
            diffuse_light_intensity = 1;
            attenuation = albedo;
            return true;
        };

    public:
        Vec3f albedo;
};

class Simple_Lighting : public Material {
    public:
        Simple_Lighting(const Vec3f &color): albedo(color) {}

        virtual bool scatter(const Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
            for(size_t i=0; i<lights.objects.size(); i++) {
                Vec3f light_direction = (lights.objects[i]->position - record.last_hit).normalize();
                diffuse_light_intensity += lights.objects[i]->intensity * std::max(0.f, dot(light_direction, record.last_normal));
            }
            attenuation = albedo;
            return true;
        };

    public:
        Vec3f albedo;
};

class Phong_Lighting : public Material {

    Vec3f reflect( Vec3f &I, const shared_ptr<Vec3f> &N) const {
        return I - N*2.f*(I*N);
    }

    public:
        Phong_Lighting(const Vec3f &color, const Vec3f &albedo, const float &specular) : albedo(albedo), diffuse_color(color), specular_exponent(specular) {}
        
        virtual bool scatter(const Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
            float specular_light_intensity = 0;
            for (size_t i=0; i<lights.objects.size(); i++) {
                Vec3f light_direction = (lights.objects[i]->position - record.last_hit).normalize();
                diffuse_light_intensity += lights.objects[i]->intensity * std::max(0.f, dot(light_direction, record.last_normal));
                specular_light_intensity += powf(std::max(0.f, dot(reflect(light_direction, record.last_normal),record.last_direction)), specular_exponent) * lights.objects[i]->intensity;
            }
            return true;
        };
        

    public:
        Vec3f albedo;
        Vec3f diffuse_color;
        float specular_exponent;
};

#endif