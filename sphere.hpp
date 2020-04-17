#pragma once
#include "vector.hpp"
#include "ray.hpp"
#include <limits>
// #include "sphere.cpp"

double inf = std::numeric_limits<double>::infinity();


class Intersection {
    public:
        bool is_intersection;
        Vector position;
        Vector normal;
        double distance;
        int index;
        Intersection(bool b = false, Vector P = Vector(0, 0, 0), Vector N = Vector(0, 0, 0), double d = inf, int i = 0)
        {
            is_intersection = b;
            position = P;
            normal = N;
            distance = d;
            index = i;
        }
};

class Sphere {
    public:
        Vector center;
        double radius;
        Vector albedo;
        int index;
        bool mirror = false;
        Intersection intersect(const Ray &r);

        explicit Sphere(Vector C, double R,Vector A =Vector(0,0,0),std::string surface = "diffuse"){
            center = C;
            radius = R;
            albedo = A;
            if (surface == "mirror") mirror = true;
        }
};