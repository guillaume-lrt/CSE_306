#pragma once
#include <vector>
#include "sphere.hpp"
#include "light.hpp"

class Scene{
    public:
        std::vector<Sphere> spheres;
        Intersection intersection(const Ray& r);
        Vector getColor(const Ray &r, int ray_depth, std::vector<double> index = {1});
        Light light = Light(Vector(0,0,0),0);
        explicit Scene(std::vector<Sphere> S, Light L){
            spheres = S;
            light = L;
            for (int i = 0; i < spheres.size(); i++){
                spheres[i].index = i;
            }
        }
};