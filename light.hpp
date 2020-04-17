#pragma once
#include "vector.hpp"
#define PI 3.14159265

class Light{
public:
    Vector origin;
    double intensity;
    explicit Light(Vector S, double I){
        origin = S;
        intensity = I;
    }
};