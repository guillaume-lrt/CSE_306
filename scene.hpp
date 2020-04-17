#pragma once
#include <vector>
#include "sphere.hpp"
#include "light.hpp"

class Scene{
    public:
        std::vector<Sphere> spheres;
        Intersection intersection(const Ray& r);
        Vector intensity_reflexion(Light lght, const Ray &r);
        explicit Scene(std::vector<Sphere> S){
            spheres = S;
        }
};