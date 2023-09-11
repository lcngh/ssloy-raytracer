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

bool scene_intersect(const Vec3f &origin, const Vec3f &direction, const Collidable_List &world, Vec3f &hit, Vec3f &normal, Collision_Record &record) {
    
    float nearest_distance = 1e10;
    if (world.ray_intersect(origin, direction, record) && record.t < nearest_distance) {
        nearest_distance = record.t;
        hit = origin + direction*nearest_distance;
        record.last_hit = make_shared<Vec3f>(hit);
        normal = (hit - record.last_center).normalize();
        record.last_normal = make_shared<Vec3f>(normal);
    }
    

    return nearest_distance<1000;

}

Vec3f cast_ray(const Vec3f &origin, const Vec3f &direction, const Collidable_List &world, int depth, const Lighting_List &lights) {
    Collision_Record record;
    record.last_direction = make_shared<Vec3f>(direction);
    Vec3f hit, normal, attenuation;
    float diffuse_light_intensity = 0;

    if (scene_intersect(origin, direction, world, hit, normal, record)) {
        if (record.mat_ptr->scatter(record, attenuation, diffuse_light_intensity, lights)) {
            return attenuation * diffuse_light_intensity;
        }
    }
    // color for miss
    return Vec3f(1.0, 1.0, 1.0);
}

void render(const Collidable_List &world, const Lighting_List &lights) {
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

    auto simple_red = make_shared<Simple_Lighting>(RED);
    auto simple_green = make_shared<Simple_Lighting>(GREEN);
    auto simple_blue = make_shared<Simple_Lighting>(BLUE);

    auto lights = Lighting_List();
    lights.add(make_shared<Light>(Vec3f( -20, 20, 20), 1.5));

    world.add(make_shared<Sphere>(Vec3f(-3, -3, -10), 1, flat_red));
    world.add(make_shared<Sphere>(Vec3f( 0, -3, -10), 1, flat_green));
    world.add(make_shared<Sphere>(Vec3f( 3, -3, -10), 1, flat_blue));

    world.add(make_shared<Sphere>(Vec3f(-3, 3, -10), 1, simple_red));
    world.add(make_shared<Sphere>(Vec3f( 0, 3, -10), 1, simple_green));
    world.add(make_shared<Sphere>(Vec3f( 3, 3, -10), 1, simple_blue));

    render(world, lights);

    return 0;
}