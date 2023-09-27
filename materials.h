#ifndef MATERIALS_H
#define MATERIALS_H

//#include <cmath>

struct Collision_Record;

/* Vec3f specular_reflection(const Vec3f &I, const shared_ptr<Vec3f> &N) {
    return I - N * 2.f * dot(I, N);
} */

/*inclusion of this function in material allows shadows to be CAST ON material, not for object TO CAST shadows ...*/
bool cast_shadow_ray(const Lighting_List &lights, size_t index, Vec3f light_direction, Collision_Record &record) {
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
};
/* 
    //Material tempMaterial;
    //if (scene_intersect(shadow_origin, light_direction, *record.world_reference, shadow_hit, shadow_normal, record)) 
    //return true;
    //return false;
*/

/* 
// Vec3f reflection(Collision_Record &record, Vec3f &reflected_ray_direction, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights ) {

//     std::shared_ptr<Vec3f> last_normal_buffer = make_shared<Vec3f>(record.last_normal);
//     std::shared_ptr<Collidable_List> world_reference_buffer = make_shared<Collidable_List>(record.world_reference);
//     std::shared_ptr<Vec3f> last_direction_buffer = make_shared<Vec3f>(record.last_direction);
//     std::shared_ptr<Vec3f> last_hit_buffer = make_shared<Vec3f>(record.last_hit);
//     Vec3f reflected_attenuation;
//     Vec3f reflected_hit;
//     Vec3f reflected_normal;
//     //bool distance_in_range = record.t < 1000;
//     bool reflections_max_reached = record.reflect_depth >= 4;

//     if (record.world_reference->ray_intersect(record.last_hit, reflected_ray_direction, record) && !reflections_max_reached){

//         reflected_hit = last_hit_buffer + reflected_ray_direction*record.t;
//         record.last_hit = make_shared<Vec3f>(reflected_hit);

//         reflected_normal = (reflected_hit - record.last_center).normalize(); 
//         record.last_normal = make_shared<Vec3f>(reflected_normal);

//         record.world_reference = world_reference_buffer;
//         record.last_direction = make_shared<Vec3f>(reflected_ray_direction);
//         ++record.reflect_depth;
//         if (record.mat_ptr->scatter(record, attenuation, diffuse_light_intensity, lights) && distance_in_range) {
//             reflected_attenuation = attenuation;
//         } else {
//             reflected_attenuation = Vec3f(1.,0.,0.);
//         }
//     } else {
//         // color for miss, sould match the one in main() :)
//         reflected_attenuation = Vec3f(1.,1.,1.);
//     }

//     return reflected_attenuation;
// }


// Vec3f cast_reflection_ray(const Vec3f &origin, const Vec3f &direction, shared_ptr<Collidable_List> &world, const Lighting_List &lights, int depth = 0) {

//     Vec3f reflect_hit, reflect_normal, reflect_color;
//     float nearest_distance = 1e10;
//     bool distance_in_range;
//     Collision_Record tempRecord;
//     float diffuse_light_intensity = 0.0;

//     if (world->ray_intersect(origin, direction, tempRecord) && tempRecord.t < nearest_distance) {
//         nearest_distance = tempRecord.t;
//         reflect_hit = origin + direction*nearest_distance;
//         tempRecord.last_hit = make_shared<Vec3f>(reflect_hit);
//         reflect_normal = (reflect_hit - tempRecord.last_center).normalize();
//         tempRecord.last_normal = make_shared<Vec3f>(reflect_normal);
//         tempRecord.last_direction = make_shared<Vec3f>(direction);
//         distance_in_range = nearest_distance<1000;
//     }

//     if (depth > 4 || !distance_in_range) {
//         return Vec3f(1.0,1.0,1.0);
//     }


//     Vec3f reflect_direction = specular_reflection(direction, tempRecord.last_normal);
//     Vec3f reflect_origin = dot(reflect_direction, tempRecord.last_normal) < 0 
//         ? tempRecord.last_hit - tempRecord.last_normal*1e-3 
//         : tempRecord.last_hit + tempRecord.last_normal*1e-3;

//     return reflect_color * cast_reflection_ray(reflect_origin, reflect_direction, world, lights, depth + 1);
    

//     // if (depth > 4 || !distance_in_range) {
//     //     return Vec3f(1.0,1.0,1.0);
//     // } else {
//     //     if (tempRecord.mat_ptr->scatter(tempRecord, reflect_color, diffuse_light_intensity, lights)) {
//     //         Vec3f reflect_direction = specular_reflection(direction, tempRecord.last_normal);
//     //         Vec3f reflect_origin = dot(reflect_direction, tempRecord.last_normal) < 0 
//     //             ? tempRecord.last_hit - tempRecord.last_normal*1e-3 
//     //             : tempRecord.last_hit + tempRecord.last_normal*1e-3;

//     //         return reflect_color *= cast_reflection_ray(reflect_origin, reflect_direction, world, lights, depth + 1);

//     //     }
//     // }


// }
 */
class Material {
    public:
    virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const = 0;
};

class Flat : public Material {
    public:
        Flat(const Vec3f &color) : albedo(color) {}

        virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
            for (size_t i=0; i<lights.objects.size(); i++) {
/* 
                // Vec3f l = lights.objects[i]->position;
                // std::shared_ptr<Vec3f> h = record.last_hit;

                // float le0 = l.e[0];
                // float le1 = l.e[1];
                // float le2 = l.e[2];

                // float he0 = h->e[0];
                // float he1 = h->e[1];
                // float he2 = h->e[2];
 */

                Vec3f light_direction = (lights.objects[i]->position - record.last_hit).normalize();
                //Vec3f light_direction = (l - h).normalize();

                if (cast_shadow_ray(lights, i, light_direction, record) && diffuse_light_intensity < 1){
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

                if (cast_shadow_ray(lights, i, light_direction, record)) continue;

                diffuse_light_intensity += lights.objects[i]->intensity * std::max(0.f, dot(light_direction, record.last_normal));
            }
            attenuation = albedo * diffuse_light_intensity;
            return true;
        };

    public:
        Vec3f albedo;
};

class Phong : public Material {

    // Vec3f specular_reflection( Vec3f &I, const shared_ptr<Vec3f> &N) const {
    //     return I - N * 2.f * dot(I, N);
    // }

    Vec3f specular_reflection(const Vec3f &I, const shared_ptr<Vec3f> &N) const {
        return I - N * 2.f * dot(I, N);
    }

    public:
        Phong(const Vec3f &color, const Vec3f &albedo, const float &specular) : albedo(albedo), diffuse_color(color), specular_exponent(specular) {}
        
        virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
            float specular_light_intensity = 0;
            for (size_t i=0; i<lights.objects.size(); i++) {
                Vec3f light_direction = (lights.objects[i]->position - record.last_hit).normalize();
                
                if (cast_shadow_ray(lights, i, light_direction, record)) continue;
                
                diffuse_light_intensity += lights.objects[i]->intensity * std::max(0.f, dot(light_direction, record.last_normal));
                specular_light_intensity += powf(std::max(0.f, dot(specular_reflection(light_direction, record.last_normal),record.last_direction)), specular_exponent) * lights.objects[i]->intensity;
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

    // Vec3f specular_reflection(Vec3f &I, const shared_ptr<Vec3f> &N) const {
    //     return I - N * 2.f * dot(I, N);
    // }

    public:
        Mirror(const Vec3f &color, const Vec3f &albedo, const float &specular) :  diffuse_color(color), albedo(albedo), specular_exponent(specular) {}

        Vec3f specular_reflection(const Vec3f &I, const shared_ptr<Vec3f> &N) const {
            return I - N * 2.f * dot(I, N);
        }

        Vec3f reflection(Collision_Record &record, Vec3f &reflected_ray_direction, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights ) const {

            std::shared_ptr<Vec3f> last_normal_buffer = make_shared<Vec3f>(record.last_normal);
            std::shared_ptr<Collidable_List> world_reference_buffer = make_shared<Collidable_List>(record.world_reference);
            std::shared_ptr<Vec3f> last_direction_buffer = make_shared<Vec3f>(record.last_direction);
            std::shared_ptr<Vec3f> last_hit_buffer = make_shared<Vec3f>(record.last_hit);
            Vec3f reflected_attenuation;
            Vec3f reflected_hit;
            Vec3f reflected_normal;
            bool distance_in_range = record.t < 1000;
            bool reflections_max_reached = record.reflect_depth >= 4;

            if (record.world_reference->ray_intersect(record.last_hit, reflected_ray_direction, record) && !reflections_max_reached){

                reflected_hit = last_hit_buffer + reflected_ray_direction*record.t;
                record.last_hit = make_shared<Vec3f>(reflected_hit);

                reflected_normal = (reflected_hit - record.last_center).normalize(); 
                record.last_normal = make_shared<Vec3f>(reflected_normal);

                record.world_reference = world_reference_buffer;
                record.last_direction = make_shared<Vec3f>(reflected_ray_direction);
                ++record.reflect_depth;
                if (record.mat_ptr->scatter(record, attenuation, diffuse_light_intensity, lights) && distance_in_range) {
                    reflected_attenuation = attenuation;
                } else {
                    reflected_attenuation = Vec3f(1.,0.,1.);
                }
            } else {
                // color for miss, sould match the one in main() :)
                reflected_attenuation = Vec3f(1.,1.,1.);
            }

            return reflected_attenuation;
        }

        virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
            float specular_light_intensity = 0;
/*             
            // Vec3f hit = Vec3f(record.last_hit);
            // Vec3f direction = Vec3f(record.last_direction);

            //Vec3f reflection_color = cast_reflection_ray(record.last_hit, record.last_direction, record.world_reference, lights, 0);

            // std::shared_ptr<Vec3f> h = record.last_hit;

            //     float he0 = h->e[0];
            //     float he1 = h->e[1];
            //     float he2 = h->e[2];
 */

            for (size_t i=0; i<lights.objects.size(); i++) {
                Vec3f light_direction = (lights.objects[i]->position - record.last_hit).normalize();

                //if (cast_shadow_ray(lights, i, light_direction, record)) continue;

                diffuse_light_intensity += lights.objects[i]->intensity * std::max(0.f, dot(light_direction, record.last_normal));
                specular_light_intensity += powf(std::max(0.f, dot(specular_reflection(light_direction, record.last_normal), record.last_direction)), specular_exponent) * lights.objects[i]->intensity;
            }

            Vec3f reflected_ray_direction = specular_reflection(record.last_direction, record.last_normal);

            Vec3f reflected_attenuation = reflection(record, reflected_ray_direction, attenuation, diffuse_light_intensity, lights);
/* 
            // std::shared_ptr<Vec3f> last_normal_buffer = make_shared<Vec3f>(record.last_normal);
            // std::shared_ptr<Collidable_List> world_reference_buffer = make_shared<Collidable_List>(record.world_reference);
            // std::shared_ptr<Vec3f> last_direction_buffer = make_shared<Vec3f>(record.last_direction);
            // std::shared_ptr<Vec3f> last_hit_buffer = make_shared<Vec3f>(record.last_hit);
            // Vec3f reflected_attenuation;
            // Vec3f reflected_hit;
            // Vec3f reflected_normal;
            // bool distance_in_range = record.t < 1000;
            // bool reflections_max_reached = record.reflect_depth >= 4;

            // if (record.world_reference->ray_intersect(record.last_hit, reflected_ray_direction, record) && !reflections_max_reached){

            //     reflected_hit = last_hit_buffer + reflected_ray_direction*record.t;
            //     record.last_hit = make_shared<Vec3f>(reflected_hit);

            //     reflected_normal = (reflected_hit - record.last_center).normalize(); 
            //     record.last_normal = make_shared<Vec3f>(reflected_normal);

            //     record.world_reference = world_reference_buffer;
            //     record.last_direction = make_shared<Vec3f>(reflected_ray_direction);
            //     record.reflect_depth++;
            //     if (record.mat_ptr->scatter(record, attenuation, diffuse_light_intensity, lights) && distance_in_range) {
            //         reflected_attenuation = attenuation;
            //     } else {
            //         reflected_attenuation = Vec3f(1.,0.,0.);
            //     }
            // } else {
            //     // color for miss, sould match the one in main() :)
            //     reflected_attenuation = Vec3f(1.,1.,1.);
            // }


            // reflected_attenuation = reflection(record, reflected_ray_direction, attenuation, diffuse_light_intensity, lights);
            // bool distance_in_range = record.t < 1000;
            // bool reflection_depth_max_reached = record.reflect_depth >= 4;
            // std::cout << record.reflect_depth << '\n';
            // if (record.mat_ptr->scatter(record, attenuation, diffuse_light_intensity, lights) && distance_in_range && !reflection_depth_max_reached) {
            //     reflected_attenuation = attenuation;
            // }
            // } else {
            //     // color for miss, sould match the one in main() :)
            //     reflected_attenuation = Vec3f(1.,1.,1.);
            // }


            
            //std::cout << "red channel: " << diffuse_color.e[0] << '\n' << "green channel: " << diffuse_color.e[1] << '\n' << "blue channel: " << diffuse_color.e[2] << '\n';
            //std::cout << "diffuse light intensity: " << diffuse_light_intensity << '\n';
            //std::cout << "specular light intensity" << specular_light_intensity << '\n';
            //std::cout << "red: " << reflected_attenuation.e[0] << '\n' << "green: " << reflected_attenuation.e[1] << '\n' << "blue: " << reflected_attenuation.e[2] << '\n';

 */
            attenuation = diffuse_color * diffuse_light_intensity * albedo.e[0] + Vec3f(1.0, 1.0, 1.0) * specular_light_intensity * albedo.e[1] + reflected_attenuation * albedo.e[2];
            return true;
        }

    public:
        Vec3f albedo;
        Vec3f diffuse_color;
        float specular_exponent;
};

class Glass : public Material {

    public:
        Glass(const Vec3f &color, const Vec3f &albedo, const float &specular) :  diffuse_color(color), albedo(albedo), specular_exponent(specular) {}

        Vec3f specular_reflection(const Vec3f &I, const shared_ptr<Vec3f> &N) const {
            return I - N * 2.f * dot(I, N);
        }

        Vec3f reflection(Collision_Record &record, Vec3f &reflected_ray_direction, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights ) const {

            std::shared_ptr<Vec3f> last_normal_buffer = make_shared<Vec3f>(record.last_normal);
            std::shared_ptr<Collidable_List> world_reference_buffer = make_shared<Collidable_List>(record.world_reference);
            std::shared_ptr<Vec3f> last_direction_buffer = make_shared<Vec3f>(record.last_direction);
            std::shared_ptr<Vec3f> last_hit_buffer = make_shared<Vec3f>(record.last_hit);
            Vec3f reflected_attenuation;
            Vec3f reflected_hit;
            Vec3f reflected_normal;
            bool distance_in_range = record.t < 1000;
            bool reflections_max_reached = record.reflect_depth >= 4;

            if (record.world_reference->ray_intersect(record.last_hit, reflected_ray_direction, record) && !reflections_max_reached){

                reflected_hit = last_hit_buffer + reflected_ray_direction*record.t;
                record.last_hit = make_shared<Vec3f>(reflected_hit);

                reflected_normal = (reflected_hit - record.last_center).normalize(); 
                record.last_normal = make_shared<Vec3f>(reflected_normal);

                record.world_reference = world_reference_buffer;
                record.last_direction = make_shared<Vec3f>(reflected_ray_direction);
                ++record.reflect_depth;
                if (record.mat_ptr->scatter(record, attenuation, diffuse_light_intensity, lights) && distance_in_range) {
                    reflected_attenuation = attenuation;
                } else {
                    reflected_attenuation = Vec3f(1.,0.,1.);
                }
            } else {
                // color for miss, sould match the one in main() :)
                reflected_attenuation = Vec3f(1.,1.,1.);
            }

            return reflected_attenuation;
        }

        virtual bool scatter(Collision_Record &record, Vec3f &attenuation, float &diffuse_light_intensity, const Lighting_List &lights) const override {
            float specular_light_intensity = 0;


            for (size_t i=0; i<lights.objects.size(); i++) {
                Vec3f light_direction = (lights.objects[i]->position - record.last_hit).normalize();

                //if (cast_shadow_ray(lights, i, light_direction, record)) continue;

                diffuse_light_intensity += lights.objects[i]->intensity * std::max(0.f, dot(light_direction, record.last_normal));
                specular_light_intensity += powf(std::max(0.f, dot(specular_reflection(light_direction, record.last_normal), record.last_direction)), specular_exponent) * lights.objects[i]->intensity;
            }

            Vec3f reflected_ray_direction = specular_reflection(record.last_direction, record.last_normal);

            Vec3f reflected_attenuation = reflection(record, reflected_ray_direction, attenuation, diffuse_light_intensity, lights);

            attenuation = diffuse_color * diffuse_light_intensity * albedo.e[0] + Vec3f(1.0, 1.0, 1.0) * specular_light_intensity * albedo.e[1] + reflected_attenuation * albedo.e[2];
            return true;
        }

    public:
        Vec3f albedo;
        Vec3f diffuse_color;
        float specular_exponent;
};

#endif