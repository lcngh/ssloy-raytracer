#ifndef LIGHTING_LIST_H
#define LIGHTING_LIST_H

#include <vector>
#include <memory>

#include "lighting.h"

using std::shared_ptr;
using std::make_shared;

class Lighting_List : public Light {

    public:
        Lighting_List() {};
        Lighting_List(shared_ptr<Light> object) {add(object);}

        void clear() {objects.clear();}
        void add(shared_ptr<Light> object) {objects.push_back(object);}

    public:
        std::vector<shared_ptr<Light>> objects;



};

#endif