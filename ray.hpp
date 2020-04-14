#pragma once
#include "vector.hpp"

class Ray {
    public:
        Vector origin;
        Vector direction;
        explicit Ray(Vector O, Vector u){
            origin = O;
            if (norm(u) != 1) {
                std::cout << "Ray.hpp, u not a unit vector" << std::endl;
                throw "not a unit vector";
            }
            direction = u;
        }
};