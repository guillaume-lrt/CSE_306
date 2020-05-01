#pragma once

#include "vector.hpp"
#include "utils.hpp"

class Light{
public:
    Vector origin;
    Sphere sphere;
    double intensity;
    double radius;
    explicit Light(Sphere S = Sphere(), double I = pow(10,5)){ //Vector C, double I, double R){
        origin = S.center;
        intensity = I;
        radius = S.radius;
        sphere = S;
        sphere.light = true;
    }
};