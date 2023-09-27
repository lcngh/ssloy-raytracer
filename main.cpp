#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "geometry.h"
#include "lighting_list.h"
#include "sphere.h"
#include "materials.h"
/* 
// bool scene_intersect(const Vec3f &origin, const Vec3f &direction, const Collidable_List &world, Vec3f &hit, Vec3f &normal, Collision_Record &record) {
    
//     float nearest_distance = 1e10;
//     if (world.ray_intersect(origin, direction, record) && record.t < nearest_distance) {
//         nearest_distance = record.t;
//         hit = origin + direction*nearest_distance;
//         record.last_hit = make_shared<Vec3f>(hit);
//         normal = (hit - record.last_center).normalize();
//         record.last_normal = make_shared<Vec3f>(normal);
//         record.last_direction = make_shared<Vec3f>(direction);
//         record.world_reference = make_shared<Collidable_List>(world);
//     }
    

//     return nearest_distance<1000;

// }
 */

Vec3f cast_ray(const Vec3f &origin, const Vec3f &direction, Collidable_List &world, const Lighting_List &lights) {
    Collision_Record record;
    // for now, this assignment affects ALL reflective mirrors, individual material depth can be changed in materials.h
    record.reflect_depth = 0;


    Vec3f hit, normal, attenuation;
    float diffuse_light_intensity = 0.0;
    bool distance_in_range;

    float nearest_distance = 1e10;
    if (world.ray_intersect(origin, direction, record) && record.t < nearest_distance) {
        nearest_distance = record.t;
        //std::cout << record.t << '\n';
        hit = origin + direction*nearest_distance;
        record.last_hit = make_shared<Vec3f>(hit);
        normal = (hit - record.last_center).normalize();
        record.last_normal = make_shared<Vec3f>(normal);
        record.last_direction = make_shared<Vec3f>(direction);
        record.world_reference = make_shared<Collidable_List>(world);
        distance_in_range = nearest_distance<1000;
        if (distance_in_range) {
        //std::cout << record.mat_ptr << '\n';
            if (record.mat_ptr->scatter(record, attenuation, diffuse_light_intensity, lights)) {
                return attenuation;
            }
        }
    }

    // if (distance_in_range) {
    //     //std::cout << record.mat_ptr << '\n';
    //     if (record.mat_ptr->scatter(record, attenuation, diffuse_light_intensity, lights)) {
    //         return attenuation;
    //     }
    // }
/* 
    // if (scene_intersect(origin, direction, world, hit, normal, record)) {
    //     if (record.mat_ptr->scatter(record, attenuation, diffuse_light_intensity, lights)) {
    //         return attenuation;
    //     }
    // }
 */
    // color for miss
    return Vec3f(1.0, 1.0, 1.0);
}



void render( Collidable_List &world, const Lighting_List &lights) {
    const int height = 1080;
    const float fov  = 3.14159/2.;
    const int width = 1920;
    //const int max_depth = 50;

    // vector of type Vec3f named framebuffer[]
    std::vector<Vec3f> framebuffer(width*height);
    int scanline_count = height;

    for (size_t j = 0; j < height; j++) {

        std::cerr << "\rScanlines remaining: " << scanline_count << ' ' << std::flush;
        scanline_count--;

        for (size_t i = 0; i < width; i++) {
            float x =  (2*(i + 0.5)/(float)width  - 1)*tan(fov/2.)*width/(float)height;
            float y = -(2*(j + 0.5)/(float)height - 1)*tan(fov/2.);
            Vec3f direction = Vec3f(x, y, -1).normalize();
            framebuffer[i+j*width] = cast_ray(Vec3f(0,0,0), direction, world, lights);
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
    auto WHITE= Vec3f(1.0, 1.0, 1.0);

    auto default_albedo = Vec3f(0.6,  0.3, 0.0);
    auto mirror_albedo  = Vec3f(0.0,  10.0, 0.8);

    auto flat_red   = make_shared<Flat>(RED);
    auto flat_green = make_shared<Flat>(GREEN);
    auto flat_blue  = make_shared<Flat>(BLUE);

    auto lambertian_red   = make_shared<Lambertian>(RED);
    auto lambertian_green = make_shared<Lambertian>(GREEN);
    auto lambertian_blue  = make_shared<Lambertian>(BLUE);

    auto phong_red   = make_shared<Phong>(RED,   default_albedo, 50.0);
    auto phong_green = make_shared<Phong>(GREEN, default_albedo, 50.0);
    auto phong_blue  = make_shared<Phong>(BLUE,  default_albedo, 50.0);
    auto phong_white = make_shared<Phong>(WHITE, default_albedo, 50.0);

    // placeholder values for now
    auto glass = make_shared<Glass>(WHITE, mirror_albedo, 1425.0);

    auto mirror = make_shared<Mirror>(WHITE, mirror_albedo, 1425.0);

    auto lights = Lighting_List();
    
    lights.add(make_shared<Light>(Vec3f( -20, 20,  20), 1.5));
    lights.add(make_shared<Light>(Vec3f(  30, 50, -25), 1.0));
    lights.add(make_shared<Light>(Vec3f(  30, 20,  30), 1.7));
    //lights.add(make_shared<Light>(Vec3f(0,0,-30), 2));

    // world.add(make_shared<Sphere>(Vec3f(  -2,       0,  -16),   2, flat_blue));
    // world.add(make_shared<Sphere>(Vec3f(  -1,    -1.5,  -12),   2, mirror));
    // world.add(make_shared<Sphere>(Vec3f( 1.5,    -0.5,  -20),   3, flat_red));
    // world.add(make_shared<Sphere>(Vec3f(   7,       5,  -18),   4, mirror));
    // world.add(make_shared<Sphere>(Vec3f(   0,  -104.5,    0), 100, flat_green));

    // world.add(make_shared<Sphere>(Vec3f(  -2,       0,  -16),   2, lambertian_blue));
    // world.add(make_shared<Sphere>(Vec3f(  -1,    -1.5,  -12),   2, mirror));
    // world.add(make_shared<Sphere>(Vec3f( 1.5,    -0.5,  -20),   3, lambertian_red));
    // world.add(make_shared<Sphere>(Vec3f(   7,       5,  -18),   4, mirror));
    // world.add(make_shared<Sphere>(Vec3f(   0,  -104.5,    0), 100, lambertian_green));

    world.add(make_shared<Sphere>(Vec3f(  -2,       0,  -16),   2, phong_blue));
    world.add(make_shared<Sphere>(Vec3f(  -1,    -1.5,  -12),   2, mirror));
    world.add(make_shared<Sphere>(Vec3f( 1.5,    -0.5,  -20),   3, phong_red));
    world.add(make_shared<Sphere>(Vec3f(   7,       5,  -18),   4, mirror));
    world.add(make_shared<Sphere>(Vec3f( 3.5,       0,  -10),   3, glass));
    world.add(make_shared<Sphere>(Vec3f(   0,  -104.5,    0), 100, phong_green));
/*
    // world.add(make_shared<Sphere>(Vec3f(-1  , -1.5, -12), 2, mirror));
    // world.add(make_shared<Sphere>(Vec3f(-5  ,    0, -16), 2, phong_blue));
    // world.add(make_shared<Sphere>(Vec3f( 7  ,    5, -18), 4, mirror));
    // world.add(make_shared<Sphere>(Vec3f(-1.5, -0.5, -22), 3, phong_red));
    
    // world.add(make_shared<Sphere>(Vec3f(-1.5, -0.5, -22), 3, phong_red));
    // world.add(make_shared<Sphere>(Vec3f( 7  ,    5, -18), 4, mirror));
    // world.add(make_shared<Sphere>(Vec3f(-5  ,    0, -16), 2, phong_blue));
    // world.add(make_shared<Sphere>(Vec3f(-1  , -1.5, -12), 2, mirror));

    

    // world.add(make_shared<Sphere>(Vec3f( 0, 0, -10), 2, phong_red));
    // world.add(make_shared<Sphere>(Vec3f( 0, 0, -14), 2, mirror));
    // world.add(make_shared<Sphere>(Vec3f( 0, 0, -16), 2, phong_green));
    // world.add(make_shared<Sphere>(Vec3f( 0, 0, -18), 2, phong_blue));

    
    
    
    // world.add(make_shared<Sphere>(Vec3f( 0, 0, -18), 2, phong_blue));
    // world.add(make_shared<Sphere>(Vec3f( -3, 0, -16), 2, phong_green));
    // world.add(make_shared<Sphere>(Vec3f( -6, 0, -14), 2, mirror));
    // world.add(make_shared<Sphere>(Vec3f( -9, 0, -10), 2, phong_red));
    


    //lights.add(make_shared<Light>(Vec3f(  7,  0, -10), 1.5));

    // world.add(make_shared<Sphere>(Vec3f(-3, 3, -10), 1, flat_red));
    // world.add(make_shared<Sphere>(Vec3f( 0, 3, -10), 1, flat_green));
    // world.add(make_shared<Sphere>(Vec3f( 3, 3, -10), 1, flat_blue));

    // world.add(make_shared<Sphere>(Vec3f(-3, 0, -10), 1, lambertian_red));
    // world.add(make_shared<Sphere>(Vec3f( 0, 0, -10), 1, lambertian_green));
    // world.add(make_shared<Sphere>(Vec3f( 3, 0, -10), 1, lambertian_blue));

    // world.add(make_shared<Sphere>(Vec3f(-3, -3, -10), 1, phong_red));
    // world.add(make_shared<Sphere>(Vec3f( 0, -3, -10), 1, phong_green));
    // world.add(make_shared<Sphere>(Vec3f( 3, -3, -10), 1, phong_blue));

    // world.add(make_shared<Sphere>(Vec3f(8, 0, -15), 3, mirror));
*/

    render(world, lights);

    return 0;
}