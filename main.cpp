#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "geometry.h"

void render() {
    const int width  = 1024;
    const int height = 768;
    // vector of type Vec3f named framebuffer[]
    std::vector<Vec3f> framebuffer(width*height);

    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            framebuffer[i+j*width] = Vec3f(j/float(height), i/float(width), 0);
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
    render();
    return 0;
}