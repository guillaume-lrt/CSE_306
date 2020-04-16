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

    Vector reflection(Vector N,double d,Vector albedo, double Visibility,Vector omega){
        Vector res = this->intensity/(4*PI*pow(d,2)) * albedo/PI * Visibility * dot(N,omega);
        return res;
    }
};