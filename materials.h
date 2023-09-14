#ifndef MATERIALS_H
#define MATERIALS_H

//#include <cmath>

struct Collision_Record;

/*inclusion of this function in material allows shadows to be CAST ON material, not for object TO CAST shadows ...*/
bool shadow(const Lighting_List &lights, size_t index, Vec3f light_direction, Collision_Record &record) {
    float light_distance = (lights.objects[index]->position - record.last_hit).magnitude();

    Vec3f shadow_origin = dot(light_direction, record.last_normal) < 0 ? record.last_hit - record.last_normal*1e-3 : record.last_hit + record.last_normal*1e-3;
    Vec3f shadow_hit, shadow_normal;

    /*this code is repeated... looking into beter solution :) */
    float nearest_distance = 1e10;
    bool distance_in_range;
    // had to use a temp record here, or segfault ... possibly because I dont "record.world_reference = make_shared<Collidable_List>(world);" here too?
    Collision_Record tempRecord;
    if (record.world_reference->ray_intersect(shadow_origin, light_direction, tempRecord) && tempRecord.t < nearest_distance) {
        nearest_distance = tempRecord.t;
        shadow_hit = shadow_origin + light_direction*nearest_distance;
        tempRecord.last_hit = make_shared<Vec3f>(shadow_hit);
        shadow_normal = (shadow_hit - tempRecord.last_center).normalize();
        tempRecord.last_normal = make_shared<Vec3f>(shadow_normal);
        tempRecord.last_direction = make_shared<Vec3f>(light_direction);
        distance_in_range = nearest_distance<1000;
    }
    return distance_in_range;
    //Material tempMaterial;
    //if (scene_intersect(shadow_origin, light_direction, *record.world_reference, shadow_hit, shadow_normal, record)) 
    //return true;
    //return false;
};

class Material {
    public:
    virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const = 0;
};

class Flat : public Material {
    public:
        Flat(const Vec3f &color) : albedo(color) {}

        virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
            for (size_t i=0; i<lights.objects.size(); i++) {
                Vec3f light_direction = (lights.objects[i]->position - record.last_hit).normalize();
                
                if (shadow(lights, i, light_direction, record) && diffuse_light_intensity < 1){
                    diffuse_light_intensity = 0;
                } else {
                    diffuse_light_intensity = 1;
                }
            }
            attenuation = albedo * diffuse_light_intensity;
            return true;
        };

    public:
        Vec3f albedo;
};

// lambertian
class Lambertian : public Material {
    public:
        Lambertian(const Vec3f &color): albedo(color) {}

        virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
            for(size_t i=0; i<lights.objects.size(); i++) {
                Vec3f light_direction = (lights.objects[i]->position - record.last_hit).normalize();

                if (shadow(lights, i, light_direction, record)) continue;

                diffuse_light_intensity += lights.objects[i]->intensity * std::max(0.f, dot(light_direction, record.last_normal));
            }
            attenuation = albedo * diffuse_light_intensity;
            return true;
        };

    public:
        Vec3f albedo;
};

class Phong : public Material {

    Vec3f reflect( Vec3f &I, const shared_ptr<Vec3f> &N) const {
        return I - N * 2.f * dot(I, N);
    }

    public:
        Phong(const Vec3f &color, const Vec3f &albedo, const float &specular) : albedo(albedo), diffuse_color(color), specular_exponent(specular) {}
        
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

class Mirror : public Material {

    Vec3f reflect (Vec3f &I, const shared_ptr<Vec3f> &N) const {
        return I - N * 2.f * dot(I, N);
    }

    public:
        Mirror(const Vec3f &color, const Vec3f &albedo, const float &specular) : albedo(albedo), diffuse_color(color), specular_exponent(specular) {}

        virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
            float specular_light_intensity = 0;
            for (size_t i=0; i<lights.objects.size(); i++) {
                Vec3f light_direction = (lights.objects[i]->position - record.last_hit).normalize();

                if (shadow(lights, i, light_direction, record)) continue;

                diffuse_light_intensity += lights.objects[i]->intensity * std::max(0.f, dot(light_direction, record.last_normal));
                specular_light_intensity += powf(std::max(0.f, dot(reflect(light_direction, record.last_normal), record.last_direction)), specular_exponent) * lights.objects[i]->intensity;
            }
            attenuation = diffuse_color * diffuse_light_intensity * albedo.e[0] + Vec3f(1.0, 1.0, 1.0) * specular_light_intensity * albedo.e[1];
            return true;
        }

    public:
        Vec3f albedo;
        Vec3f diffuse_color;
        float specular_exponent;
};

#endif