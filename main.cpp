#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "geometry.h"
#include "lighting_list.h"
#include "materials.h"
#include "sphere.h"
//#include "collidable_list.h"


bool scene_intersect(const Vec3f &origin, const Vec3f &direction, const Collidable_List &world, Vec3f &hit, Vec3f &normal, Collision_Record &record) {
    //Collision_Record record;

    //float spheres_distance = std::numeric_limits<float>::max();
    
    float nearest_distance = 1e10;
    if (world.ray_intersect(origin, direction, record) && record.t < nearest_distance) {
        //std::cerr << "distance_i: " << distance_i << " \n";
        nearest_distance = record.t;
        hit = origin + direction*nearest_distance;
        normal = (hit - record.last_center).normalize();
        // std::cerr << "hit0: " << hit.e[0] << " hit1: " << hit.e[1] << " hit2: " << hit.e[2] << " \n";
        // std::cerr << "rp0: " << record.point->e[0] << " rp1: " << record.point->e[1] << " rp2: " << record.point->e[2] << " \n";
    }
    

    return nearest_distance<1000;

}

Vec3f cast_ray(const Vec3f &origin, const Vec3f &direction, const Collidable_List &world, int depth, const Lighting_List &lights) {
    Collision_Record record;

    //if (depth <= 0) {return Vec3f(0,0,0);}
    Vec3f hit, normal;
    //float sphere_distance = std::numeric_limits<float>::max();
    if (scene_intersect(origin, direction, world, hit, normal, record)) {
        Vec3f attenuation;
        
        float diffuse_light_intensity = 0;
        for (size_t i=0; i<lights.objects.size(); i++) {
            Vec3f light_direction = (lights.objects[i]->position - hit).normalize();
            //std::cerr << "e0: " << (lights.objects[i]->position - hit).e[0] << " e1: " << (lights.objects[i]->position - hit).e[1] << " e2: " << (lights.objects[i]->position - hit).e[2] << " \n";
            //std::cerr << "e0: " << light_direction.e[0] << " e1: " << light_direction.e[1] << " e2: " << light_direction.e[2] << " \n";
            //std::cerr << "e0: " << lights.objects[i]->position.e[0] << " e1: " << lights.objects[i]->position.e[1] << " e2: " << lights.objects[i]->position.e[2] << " \n";
            //std::cerr << "e0: " << hit.e[0] << " e1: " << hit.e[1] << " e2: " << hit.e[2] << " \n";
            //diffuse_light_intensity += lights.objects[i]->intensity * std::max(0.f, -dot((lights.objects[i]->position - hit).normalize(),normal));
            diffuse_light_intensity += lights.objects[i]->intensity * std::max(0.f, dot(light_direction,normal));
            //std::cerr << "dot: " << dot(light_direction,normal) << "\n";
            //std::cerr << "dot: " << dot(normal, light_direction) << "\n";
            //std::cerr << "norm0: " << normal.e[0] << " norm1: " << normal.e[1] << " norm2: " << normal.e[2] << " \n";
            //std::cerr << "ld0: " << light_direction.e[0] << " ld1: " << light_direction.e[1] << " ld2: " << light_direction.e[2] << " \n";
        }
        if (record.mat_ptr->scatter(record, attenuation)) {
            //std::cerr << "\rdiffuse_light_intensity: " << diffuse_light_intensity << ' ' << std::flush;
            return attenuation * diffuse_light_intensity;
        }
        //return Vec3f(0.0, 0.0, 0.0);
    }
    // color for miss
    return Vec3f(1.0, 1.0, 1.0);
}

void render(const Collidable_List &world, const Lighting_List &lights) {
    //const int width  = 1024;
    const int height = 1080;
    const float fov  = 3.14159/2.;
    const int width = 1920;
    const int max_depth = 50;

    // vector of type Vec3f named framebuffer[]
    std::vector<Vec3f> framebuffer(width*height);
    int scanline_count = height;

    for (size_t j = 0; j < height; j++) {

        std::cerr << "\rScanlines remaining: " << scanline_count << ' ' << std::flush;
        scanline_count--;


        for (size_t i = 0; i < width; i++) {
            float x =  (2*(i + 0.5)/(float)width  - 1)*tan(fov/2.)*width/(float)height;
            float y = -(2*(j + 0.5)/(float)height - 1)*tan(fov/2.);
            auto u = i / (width-1);
            auto v = j / (height-1);
            Vec3f direction = Vec3f(x, y, -1).normalize();
            framebuffer[i+j*width] = cast_ray(Vec3f(0,0,0), direction, world, max_depth, lights);
        }
    }

    std::ofstream ofs;
    ofs.open("./out.ppm");
    ofs << "P3\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height*width; i++) {
        for (size_t j = 0; j < 3; j++) {
            ofs << (int)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
            ofs << ' ';
        }
        ofs << '\n';
    }
    ofs.close();
}

int main() {

    auto world = Collidable_List();

    auto RED  = Vec3f(1.0, 0.0, 0.0);
    auto GREEN= Vec3f(0.0, 1.0, 0.0);
    auto BLUE = Vec3f(0.0, 0.0, 1.0);

    auto flat_red  = make_shared<Flat_Color>(RED);
    auto flat_green= make_shared<Flat_Color>(GREEN);
    auto flat_blue = make_shared<Flat_Color>(BLUE);

    auto lights = Lighting_List();
    lights.add(make_shared<Light>(Vec3f( -20, 20, 20), 1.5));

    world.add(make_shared<Sphere>(Vec3f(-3,      0, -16), 2, flat_blue));
    world.add(make_shared<Sphere>(Vec3f(-1.0, -1.5, -12), 2, flat_red));
    world.add(make_shared<Sphere>(Vec3f( 1.5, -0.5, -18), 3, flat_red));
    world.add(make_shared<Sphere>(Vec3f( 2,      5, -30), 4, flat_blue));

    // world.add(make_shared<Sphere>(Vec3f(-3, 3, -7), 1, flat_green));
    // world.add(make_shared<Sphere>(Vec3f( 0, 3, -7), 1, flat_blue));
    // world.add(make_shared<Sphere>(Vec3f( 3, 3, -7), 1, flat_red));

    // world.add(make_shared<Sphere>(Vec3f(-3, 0, -7), 1, flat_red));
    // world.add(make_shared<Sphere>(Vec3f( 0, 0, -7), 1, flat_green));
    // world.add(make_shared<Sphere>(Vec3f( 3, 0, -7), 1, flat_blue));

    // world.add(make_shared<Sphere>(Vec3f(-3,-3, -7), 1, flat_blue));
    // world.add(make_shared<Sphere>(Vec3f( 0,-3, -7), 1, flat_red));
    // world.add(make_shared<Sphere>(Vec3f( 3,-3, -7), 1, flat_green));

    render(world, lights);

    return 0;
}