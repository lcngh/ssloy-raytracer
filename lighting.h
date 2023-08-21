#ifndef LIGHTING_H
#define LIGHTING_H

class Light {
    public:
        Light() {};
        Light(const Vec3f &point, const float &i) : position(point), intensity(i) {}
    
    public:
        Vec3f position;
        float intensity;

};

#endif