#pragma once

#include "vector.hpp"
#include "ray.hpp"
// #include "sphere.cpp"

#include "utils.hpp"


class Intersection {
    public:
        bool is_intersection;
        Vector position;
        Vector normal;
        double distance;
        int index;
        bool hollow;
        Intersection(bool b = false, Vector P = Vector(0, 0, 0), Vector N = Vector(0, 0, 0), double d = inf, int i = 0){
            is_intersection = b;
            position = P;
            normal = N;
            distance = d;
            index = i;
            // hollow = h;
            // if (hollow) normal = -N;
        }
};

class Sphere {
    public:
        Vector center;
        double radius;
        Vector albedo;
        int index;
        bool mirror = false;
        bool transparent = false;
        double refract_index;
        Intersection intersect(const Ray &r);
        bool hollow;

        explicit Sphere(Vector C, double R,Vector A =Vector(0,0,0),std::string surface = "diffuse",double n = 0, bool h = false){
            center = C;
            radius = R;
            albedo = A;
            refract_index = n;
            if (surface == "mirror") mirror = true;
            if (surface == "transparent") transparent = true;
            hollow = h;
        }
};