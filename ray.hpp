#pragma once

#include "vector.hpp"

class Ray {
    public:
        Vector origin;
        Vector direction;
        explicit Ray(Vector O, Vector u){
            origin = O;
            direction = normalization(u);
        }
};