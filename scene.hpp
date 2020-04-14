#pragma once
#include <vector>
#include "sphere.hpp"

class Scene{
    public:
        std::vector<Sphere> spheres;
        explicit Scene(std::vector<Sphere> S){
            spheres = S;
        }
};