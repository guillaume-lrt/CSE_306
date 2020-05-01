#pragma once

#include <vector>
#include <iostream>

#include "sphere.hpp"
#include "light.hpp"

class Scene{
    public:
        std::vector<Sphere> spheres;
        Light light;
        void add_sphere(Sphere S);
        Intersection intersection(const Ray& r);
        Vector random_cos(const Vector &N);
        Vector getColor(const Ray &r, int ray_depth);//std::vector<double> index = {1});
        explicit Scene(std::vector<Sphere> S, Light L){
            spheres = S;
            spheres.push_back(L.sphere);
            light = L;
            for (int i = 0; i < spheres.size(); i++){
                spheres[i].index = i;
            }
        }
};