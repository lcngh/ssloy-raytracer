#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "geometry.h"

Vec3f cast_ray(const Vec3f &origin, const Vec3f &direction, const Sphere &sphere) {
    float sphere_distance = std::numeric_limits<float>::max();
    if (!sphere.ray_intersect(origin, direction, sphere_distance)) {
        // background color for ray(s) that miss sphere(s)
        return Vec3f(0.2, 0.7, 0.8);
    }
    // color for ray-sphere intersection
    return Vec3f(0.4, 0.4, 0.3);
}

void render(const Sphere sphere) {
    const int width  = 1024;
    const int height = 768;
    const float fov  = 90.0;

    // vector of type Vec3f named framebuffer[]
    std::vector<Vec3f> framebuffer(width*height);

    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            float x =  (2*(i + 0.5)/(float)width  - 1)*tan(fov/2.)*width/(float)height;
            float y = -(2*(j + 0.5)/(float)height - 1)*tan(fov/2.);
            Vec3f direction = Vec3f(x, y, -1).normalize();
            framebuffer[i+j*width] = cast_ray(Vec3f(0,0,0), direction, sphere);
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
    render(Sphere(Vec3f(0, 0, -3), 1));
    return 0;
}