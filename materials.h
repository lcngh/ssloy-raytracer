#ifndef MATERIALS_H
#define MATERIALS_H

//#include <cmath>

struct Collision_Record;

bool shadow(const Lighting_List &lights, size_t index, Vec3f light_direction, Collision_Record &record) {
    float light_distance = (lights.objects[index]->position - record.last_hit).magnitude();

    Vec3f shadow_origin = dot(light_direction, record.last_normal) < 0 ? record.last_hit - record.last_normal*1e-3 : record.last_hit + record.last_normal*1e-3;
    Vec3f shadow_hit, shadow_normal;
    //Material tempMaterial;
    //if (scene_intersect(shadow_origin, light_direction, *record.world_reference, shadow_hit, shadow_normal, record)) 
    //return true;
    return false;
};

class Material {
    public:
    virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const = 0;
};

class Flat_Color : public Material {
    public:
        Flat_Color(const Vec3f &color) : albedo(color) {}

        virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
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

        virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
            for(size_t i=0; i<lights.objects.size(); i++) {
                Vec3f light_direction = (lights.objects[i]->position - record.last_hit).normalize();
                diffuse_light_intensity += lights.objects[i]->intensity * std::max(0.f, dot(light_direction, record.last_normal));
            }
            attenuation = albedo * diffuse_light_intensity;
            return true;
        };

    public:
        Vec3f albedo;
};

class Phong_Lighting : public Material {

    Vec3f reflect( Vec3f &I, const shared_ptr<Vec3f> &N) const {
        //return I - N*2.f*(I*N); 
        return I - N * 2.f * dot(I, N);
        
    }

    public:
        Phong_Lighting(const Vec3f &color, const Vec3f &albedo, const float &specular) : albedo(albedo), diffuse_color(color), specular_exponent(specular) {}
        
        virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
            float specular_light_intensity = 0;
            for (size_t i=0; i<lights.objects.size(); i++) {
                Vec3f light_direction = (lights.objects[i]->position - record.last_hit).normalize();
                if (shadow(lights, i, light_direction, record)) continue;
                diffuse_light_intensity += lights.objects[i]->intensity * std::max(0.f, dot(light_direction, record.last_normal));
                specular_light_intensity += powf(std::max(0.f, dot(reflect(light_direction, record.last_normal),record.last_direction)), specular_exponent) * lights.objects[i]->intensity;
            }
            attenuation = diffuse_color * diffuse_light_intensity * albedo.e[0] + Vec3f(1.0, 1.0, 1.0) * specular_light_intensity * albedo.e[1];
            return true;
        };
        
    public:
        Vec3f albedo;
        Vec3f diffuse_color;
        float specular_exponent;
};

#endif