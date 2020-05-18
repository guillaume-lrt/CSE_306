#pragma once

#include <vector>
#include <iostream>

#include "object.hpp"
#include "object.cpp"

class Scene{
    public:
        std::vector<Geometry*> objects;
        Light light;
        void add_object(Geometry* O);
        Intersection intersection(const Ray& r);
        Vector random_cos(const Vector &N);
        Vector getColor(const Ray &r, int ray_depth);//std::vector<double> index = {1});
        explicit Scene(std::vector<Geometry*> O, Light L){
            objects = O;
            // spheres.push_back(L.sphere);
            light = L;
            for (size_t i = 0; i < objects.size(); i++){
                objects[i]->index = i;
            }
        }
};