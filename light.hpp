#pragma once

#include "vector.hpp"
#include "utils.hpp"

class Light{
public:
    Vector origin;
    double intensity;
    explicit Light(Vector S, double I){
        origin = S;
        intensity = I;
    }
};