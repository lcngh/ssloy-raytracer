#ifndef COLLIDABLE_LIST_H
#define COLLIDABLE_LIST_H

#include <vector>
#include <memory>
#include "collidable.h"

using std::shared_ptr;
using std::make_shared;

class Collidable_List : public Collidable {
    public:
        Collidable_List() {};
        Collidable_List(shared_ptr<Collidable> object) {add(object);}

        void clear() {objects.clear();}
        void add(shared_ptr<Collidable> object) {objects.push_back(object);}

        virtual bool ray_intersect(const Vec3f &origin, const Vec3f &direction, Collision_Record &record) const override;

    public:
        std::vector<shared_ptr<Collidable>> objects;

};

bool Collidable_List::ray_intersect(const Vec3f &origin, const Vec3f &direction, Collision_Record &record) const {

    Collision_Record temp_record;//= record;
    //temp_record.t = 0.0;
    // temp_record.reflect_depth = record.reflect_depth;
    //temp_record.t = record.t;
    //temp_record.last_sphere_distance = record.last_sphere_distance;
    
    //double sphere_distance = std::numeric_limits<double>::max();
    
    
    double sphere_distance = 100000000.0;
    // double * sphere_distance_pointer;
    // sphere_distance_pointer = &sphere_distance;

    //std::cout << sphere_distance << '\n';
    bool collision = false;
    temp_record.t = 0;
    //std::vector<Collidable>::iterator it = objects.begin();

    //record = temp_record;
    for (auto iterator = begin(objects); iterator != end(objects); ++iterator) {
        //std::cout << "BEFOREIF: "<< record.t << '\n';
        //exit(0);


        if ((*iterator)->ray_intersect(origin,direction,temp_record) && temp_record.t < sphere_distance) { 
            collision = true;
            sphere_distance = temp_record.t;
            //std::cout << "INNERMOST: "<< temp_record.t << '\n';
            //exit(0);
            record = temp_record;
        }

    ///////
    //  Valuable lesson was learned here...
    //  in the above if block, adding logic to the else block would do funky things
    //  must research behavior of nesting? my program? c++?
    ///////


        // else {
        //     //break;
        // }
        //std::cout << "AFTERIF: "<< record.t << '\n';
        
    }
    sphere_distance = 100000000.0;
    temp_record.t = 0.0;

    return collision;
}
        //std::cout << record.t << '\n';
        //std::cout << temp_record.t << '\n';
        //collision = true;
        //record = temp_record;
        // if (temp_record.t<sphere_distance) {
        //     sphere_distance = temp_record.t;
        //     //continue;
        // } else {
        //     return collision;
        // }
    


    // for (auto iterator = begin(objects); iterator != end(objects); ++iterator) {
    //     //std::cout << temp_record.t << '\n';
    //     std::cout << temp_record.last_sphere_distance << '\n';
    //     if ((*iterator)->ray_intersect(origin,direction,temp_record)) {
    //         if (temp_record.t > sphere_distance) {
    //             sphere_distance = temp_record.t;
    //             //temp_record.last_sphere_distance = 5.f;
    //             collision = true;
    //             record = temp_record;
    //         } else {
    //             collision=true;
    //             record=temp_record;
    //             return collision;
    //         }
    //     }
    //     //sphere_distance = temp_record.t;
    //     temp_record.last_sphere_distance = 5;
    // }


    // for (const auto& object : objects) {
    //     //double sphere_distance;
    //     if (object->ray_intersect(origin,direction,temp_record)){
    //         //std::cout << temp_record.t << '\n';
    //         //std::cout << sphere_distance << '\n';
    //         if (temp_record.t > sphere_distance) {
                
    //             sphere_distance = temp_record.t;
    //             collision = true;
    //             record = temp_record;
    //         } else {
    //             //object->ray_intersect(origin,direction,temp_record);
    //             collision = true;
    //             record = temp_record;
    //             return collision;
    //         }
    //         //sphere_distance = temp_record.t;
    //         //std::cout << sphere_distance << '\n';
    //         // sphere_distance = temp_record.t;
    //         //std::cout << *sphere_distance_pointer << '\n';
    //     }
        
    //     //sphere_distance = temp_record.t;
    //     //std::cout << temp_record.t << '\n';


    //     //record = temp_record;
    // }
        // if (temp_record.t < sphere_distance) {
        //     if (object->ray_intersect(origin,direction,temp_record)){
        //         sphere_distance = temp_record.t;
        //         collision = true;
        //         record = temp_record;
        //         //return collision;
        //     }
        // } else {
        //     // collision = true;
        //     // record = temp_record;
        //     // return collision;
        //     if (object->ray_intersect(origin, direction, temp_record)) {
        //         //sphere_distance = tem//p_record.t;
        //         collision = true;
        //         record = temp_record;
        //         return collision;
        //     }
        // }
    
        // if (object->ray_intersect(origin, direction, temp_record)) {
            
            
        //     //std::cout << temp_record.t << " is less than " << sphere_distance << "? " << (temp_record.t < sphere_distance) << '\n';
        //     collision = true;
            
        //     //sphere_distance = temp_record.t;
        //     //std::cout << "inner: " << sphere_distance << '\n';
        //     //std::cout << "distance of sphere: " << sphere_distance << '\n';
                
        //     record = temp_record;
        //     return collision;
            
        //     //std::cout << "hello\n";
        //     //return collision;
        //     //collision = true;
        //     //sphere_distance = record.last_sphere_distance;
        //     //record = temp_record;

        //     //record.last_hit = make_shared<Vec3f>(origin);
        //     //return collision;
        // }
        //std::cout << sphere_distance << '\n';
    

    //return collision;


#endif